/*******************************************************************************
*   文  件 名：Thread.c
*   功     能：封装线程功能
*   作     者：zhanxc
*   E-Mail   : zhanxc_chpu@sina.com
*   说     明：进程是一个执行实体，操作系统以进程为单位分配资源。在一个执行空间内可以
*             使用多个小型进程并发来完成不同的任务，这种小型进程称之为线程。
*             一个进程中的每个线程都有自己的运行环境上下文，包括线程ID、一组寄存器的
*             值、堆栈、信号屏蔽字等。进程的所有资源都被各线程共享，包括可执行程序代码、
*             全局变量、堆空间、栈空间、文件描述符等等。
*             由于引入了线程的概念，于是操作系统中的执行实体不再是进程，而是线程了。
*             进程只是一个用来分配资源的实体，而真正负责执行的则是线程。单进程可以看
*             作是只有一个线程的进程。
*             目前的Linux内核则是以轻量级进程的方式实现多线程。内核里的每个轻量级进程
*             对应用户空间的一个线程，轻量级进程也有自己的进程控制结构，也是一个进程调度
*             的单位。轻量级进程和普通进程的区别是多个轻量级进程共享某些资源，例如地址
*             空间等。但是轻量级进程在内核中使用的堆栈是独立的。
*             线程优势：由于线程共享进程地址空间内的所有资源，所以线程之间的通信是
*               很方便的；多个线程处理不同的任务，增加了程序的并发性，使程序更高效地执行。
*   创建日期：018-7-02
*   修改历史：无
*******************************************************************************/

/*******************************************************************************
    线程访问资源的限制：
    新线程只执行fPthreadStart()函数（这个函数是线程体），执行之后线程就退出了。在
    执行线程的过程中线程可以访问进程的资源。进程中的地址空间对于它的任意一个线程
    来讲都是开放的。
    现在可以理解为什么线程系列处理函数不设置errno变量，而是采用返回出错号的原因了。
    由于线程可以随意访问进程的环境变量，所以当多个线程出错时，errno变量的值将被多次
    覆盖，进程检查到的只是最后一个出错线程的出错原因。
    归根结底，还是由于线程出错和检查errno变量这两个操作不是原子操作。因此，线程系列
    处理函数值返回错误号，不设置errno变量。

    正确得到线程退出信息的方法：
    在现场结束运行后，Linux内核中保存的只是存储退出信息内存区域的首地址，而并未将
    退出信息实际保存在内核中，因此，在线程结束运行后，其保存退出信息的内存区域仍然
    是有效的，所以不能将退出信息保存在局部变量中，而应该使用动态分配的内存或者全局
    变量。
    实际运行结果显示，使用局部变量保存退出信息结构得到的退出信息是一些无用值，而
    使用malloc()函数分配内存、全局变量和使用main()函数的局部变量的方式存储退出信息，
    可以保证退出信息结构存储区域的有效性。
*******************************************************************************/

/*******************************************************************************
	自旋锁使用注意事项:
	1、自旋锁一种忙等待，当条件不满足时，会一直不断的循环判断条件是否满足，如果满足
		就解锁，运行之后的代码。因此会对linux的系统的性能有些影响。所以在实际编程时，
		需要注意自旋锁不应该长时间的持有。它适合于短时间的的轻量级的加锁机制。
	2、自旋锁不能递归使用，这是因为自旋锁，在设计之初就被设计成在不同进程或者函数之
		间同步。所以不能用于递归使用。

	POSIX threads(简称Pthreads)是在多核平台上进行并行编程的一套常用的API。线程同步
	(Thread Synchronization)是并行编程中非常重要的通讯手段，其中最典型的应用就是用
	Pthreads提供的锁机制(lock)来对多个线程之间共享的临界区(Critical Section)进行保
	护(另一种常用的同步机制是barrier)。
		Pthreads提供了多种锁机制：
		(1) Mutex（互斥量）：pthread_mutex_***
		(2) Spin lock（自旋锁）：pthread_spin_***
		(3) Condition Variable（条件变量）：pthread_con_***
		(4) Read/Write lock（读写锁）：pthread_rwlock_***
	
	Pthreads提供的Mutex锁操作相关的API主要有：
	pthread_mutex_lock (pthread_mutex_t *mutex);
	pthread_mutex_trylock (pthread_mutex_t *mutex);
	pthread_mutex_unlock (pthread_mutex_t *mutex);

	Pthreads提供的与Spin Lock锁操作相关的API主要有：
	pthread_spin_lock (pthread_spinlock_t *lock);
	pthread_spin_trylock (pthread_spinlock_t *lock);
	pthread_spin_unlock (pthread_spinlock_t *lock);

	从实现原理上来讲，Mutex属于sleep-waiting类型的锁。例如在一个双核的机器上有两个线
程(线程A和线程B)，它们分别运行在Core0和 Core1上。假设线程A想要通过pthread_mutex_lock
操作去得到一个临界区的锁，而此时这个锁正被线程B所持有，那么线程A就会被阻塞 (blocking)，
Core0 会在此时进行上下文切换(Context Switch)将线程A置于等待队列中，此时Core0就可以运
行其他的任务(例如另一个线程C)而不必进行忙等待。而Spin lock则不然，它属于busy-waiting
类型的锁，如果线程A是使用pthread_spin_lock操作去请求锁，那么线程A就会一直在 Core0上进
行忙等待并不停的进行锁请求，直到得到这个锁为止。

	自旋锁相关API：
	1 int pthread_spin_destroy(pthread_spinlock_t *);
	2 int pthread_spin_init(pthread_spinlock_t *, int);
	3 int pthread_spin_lock(pthread_spinlock_t *);
	4 int pthread_spin_trylock(pthread_spinlock_t *);
	5 int pthread_spin_unlock(pthread_spinlock_t *);
*******************************************************************************/

#include "Thread.h"

/*******************************************************************************
*   函 数   名：PthreadSelf
*   功     能：获取当前调用线程的 thread identifier(标识号).
*             同进程一样，每个线程都有自己的ID，使用数据类型pthread_t来表示。同
*             进程ID的数据类型pid_t一样，pthread_t本质上也是一个无符号的整型。虽然
*             线程ID是一个整型，但是出于移植性的考虑，不建议像操作整型数据一样操作
*             pthread_t类型的数据，而是应该使用系统提供的函数对其进行操作。
*   输入参数：无
*   输出参数：无
*   返 回 值：成功返回线程ID
*             man上关于返回值的说明：这个函数总是成功，返回调用线程的ID
*   作     者：zhanxc
*   创建日期：018-7-24
*   修改历史：无
*******************************************************************************/
pthread_t PthreadSelf(VOID)
{
	return pthread_self();
}

/*******************************************************************************
*   函 数   名：PthreadEqual
*   功     能：判断两个线程的ID是否相等
*   输入参数：iTid1：线程ID
*             iTid2：线程ID
*   输出参数：无
*   返 回 值：如果相等则返回0
*             如不相等则返回非0值。
*   作     者：zhanxc
*   创建日期：018-7-24
*   修改历史：无
*******************************************************************************/
INT32 PthreadEqual(pthread_t iTid1, pthread_t iTid2)
{
	return pthread_equal(iTid1, iTid2);
}

/*******************************************************************************
*   函 数   名：PthreadCreate
*   功     能：创建线程（实际上就是确定调用线程函数的入口点），在线程创建以后，就
*             开始运行相关的线程函数。
*   输入参数：pTid：指向线程标识符的指针
*             pstAttr：用来设置线程属性，如果不想指定线程的属性，则将其设置为NULL。
*             fPthreadStartHandler：线程运行函数的起始地址，当该函数返回时该线程就
*                   停止运行了。
*             pvArg：运行函数的参数，pvArg是fPthreadStartHandler所指向的函数的参数，
*                   在线程开始执行时，该参数由内核负责传递给线程。
*   输出参数：无
*   返 回 值：创建成功返回0
*             创建失败则返回出错编号
*   说     明：返回成功时，由pTid指向的内存单元被设置为新创建线程的线程ID。pstAttr参数
*             用于指定各种不同的线程属性。新创建的线程从fPthreadStartHandler函数的地址
*             开始运行，该函数只有一个万能指针参数pvArg，如果需要向fPthreadStartHandler
*             函数传递的参数不止一个，那么需要把这些参数放到一个结构中，然后把这个结构
*             的地址作为pvArg的参数传入。
*   注     意：因为pthread并非Linux系统的默认库，而是POSIX线程库。在Linux中将其作为一个
*             库来使用，因此加上-lpthread（或-pthread）以显示链接该库。函数在执行错误时
*             的错误信息将作为返回值返回，并不修改全局变量errno，当然也无法使用perror()
*             打印错误信息。
*   作     者：zhanxc
*   创建日期：018-7-24
*   修改历史：无
*******************************************************************************/
INT32 PthreadCreate(pthread_t *pTid, const pthread_attr_t *pstAttr, 
                    fPthreadStart fPthreadStartHandler, VOID *pvArg)
{
	return pthread_create(pTid, pstAttr, fPthreadStartHandler, pvArg);
}

/*******************************************************************************
*   函 数   名：PthreadExit
*   功     能：线程退出
*             线程退出方式有以下3种：
*             1、线程体函数执行结束，使用pthread_create创建的线程执行一个函数，如果该
*                函数执行结束，则线程退出。这种方法类似于进程的main()函数返回。
*             2、线程被另一个线程所取消。这种方法类似于一个进程被另一个进程调用kill()
*                函数杀死。
*             3、线程自行退出。这种方法类似于一个进程调用了exit()函数。
*             第1种方法是最常见的方法，这是一种正常退出情况。第2、3种方法是线程异常
*             退出的情况。
*   输入参数：pvRVal：指向任意类型的指针，该指针指向的区域存储退出信息，该信息类似于
*               传递给新线程的参数，可以将多个信息组织成一个结构体。
*             一个线程的结束信息可以有两种，一种是线程体函数返回的指针所指向的区域，
*             另一种就是pthread_exit()函数所指向的区域。第一种方法可以取得线程体函数
*             的返回值，而第二种方法得到pthread_exit()函数所设置的退出信息。
*             当一个线程结束运行后，其结束信息的地址被保存在内核中，其他的线程可以
*             引用此线程的结束信息。Linux环境下使用pthread_join()函数访问指定线程的
*             结束信息。
*   输出参数：无
*   返 回 值：无
*   作     者：zhanxc
*   创建日期：018-7-24
*   修改历史：无
*******************************************************************************/
VOID PthreadExit(VOID *pvRVal)
{
    pthread_exit(pvRVal);

    return;
}

/*******************************************************************************
*   函 数   名：PthreadJoin
*   功     能：用来等待一个线程的结束，线程间同步的操作。使一个线程等待另一个线程结束。
*             pthread_join()函数，以阻塞的方式等待iTid指定的线程结束。当函数返回时，
*             被等待线程的资源被收回。如果线程已经结束，那么该函数会立即返回。并且iTid
*             指定的线程必须是joinable的。
*   输出参数：iTid：线程标识符，即线程ID，标识唯一线程。
*             pvRVal：用户定义的指针，用来存储被等待线程的返回值。
*   说     明：在Linux中，默认情况下是在一个线程被创建后，必须使用此函数对创建的线程
*             进行资源回收，但是可以设置线程属性来设置当一个线程结束时，直接回收此
*             线程所占有的系统资源。
*             代码中如果没有pthread_join()，主线程会很快结束从而使得整个进程结束，从而
*             使创建的线程没有机会开始执行就结束了。加入pthread_join()后，主线程会
*             一直等待直到等待的线程结束自己才结束，使创建的线程有机会执行。
*             另外需要说明的是，一个线程不能被多个线程等待，也就是说对一个线程只能调用
*             一次pthread_join()，否则只有一个能正确返回，其他的将返回ESRCH错误。
*
*             前面我们提到，可以通过pthread_join()函数来使主线程阻塞等待其他线程退出，
*             这样主线程可以清理其他线程的环境。但是还有一些线程，更喜欢自己来清理退出
*             的状态，他们也不愿意主线程调用pthread_join来等待他们。我们将这一类线程的
*             属性称为detached。如果我们在调用pthread_create()函数的时候将属性设置为
*             NULL，则表明我们希望所创建的线程采用默认的属性，也就是joinable。如果需要
*             将属性设置为detached
*             在线程设置为joinable后，可以调用pthread_detach()使之成为detached。但是
*             相反的操作则不可以。还有，如果线程已经调用pthread_join()后，则再调用
*             pthread_detach()则不会有任何效果。
*   返 回 值：0代表成功
*             失败则返回错误号
*   作     者：zhanxc
*   创建日期：018-7-24
*   修改历史：无
*******************************************************************************/
VOID PthreadJoin(pthread_t iTid, VOID **pvRVal)
{
    pthread_join(iTid, pvRVal);
    return;
}

/*******************************************************************************
*   函 数   名：PthreadCancel
*   功     能：取消线程
*             一个进程可以通过发送信号的方式使另一个进程结束运行，例如，使用调用
*             kill()函数发送SIGKILL信号。同进程一样，一个线程也可以被另一个线程
*             取消。
*             Linux环境下使用pthread_cancel()函数取消另一个线程。
*   输入参数：iTid：取消线程ID
*   输出参数：无
*   返 回 值：如果取消成功，则返回0
*             如果失败则返回错误编号
*   说     明：调用pthread_cancel()函数等效于要被取消的线程自己调用
*             pthread_exit(PTHREAD_CANCELED);
*   作     者：zhanxc
*   创建日期：018-7-24
*   修改历史：无
*******************************************************************************/
INT32 PthreadCancel(pthread_t iTid)
{
	return pthread_cancel(iTid);
}

#if 0
/*******************************************************************************
*   函 数   名：PthreadCleanupPush
*   功     能：添加一个清理程序记录
*             同进程一样，一个线程在退出时也可以调用用户设置好的函数，这些函数成为
*             线程清理程序，记录在栈中。
*   输入参数：fCleanUpProcessHandler：一个函数指针，指向清理程序。清理程序是一个没有
*                   返回值的函数，其参数是一个任意类型的指针。
*             pvArg：是一个任意类型的指针，该参数实际表示的是清理程序的参数。
*             注意：清理程序的执行顺序和设置顺序是正好相反的。
*   输出参数：无
*   返 回 值：无
*   说     明：pthread_cleanup_push()函数会在以下3种情况下执行：
*               1、调用pthread_exit()函数时
*               2、线程被其他线程取消时
*               3、使用非0参数调用pthread_cleanup_pop()函数时
*   作     者：zhanxc
*   创建日期：018-7-24
*   修改历史：无
*******************************************************************************/
VOID PthreadCleanupPush(fCleanUpHandler fCleanUpProcessHandler, VOID *pvArg)
{
    pthread_cleanup_push(fCleanUpProcessHandler, pvArg);
    
	return;
}

/*******************************************************************************
*   函 数   名：PthreadCleanupPop
*   功     能：调用清理程序
*   输入参数：iExecute：表示是否执行栈顶的清理程序。参数为0时，表示不执行清理程序，但是将
*               栈顶的清理程序记录出栈（删除清理程序记录）；参数非0时，表示执行栈顶清理
*               程序，执行之后该记录也会出栈。
*   输出参数：无
*   返 回 值：无
*   作     者：zhanxc
*   创建日期：018-7-24
*   修改历史：无
*******************************************************************************/
VOID PthreadCleanupPop(INT32 iExecute)
{
    pthread_cleanup_pop(iExecute);
    
	return;
}
#endif

/*******************************************************************************
*   函 数   名：PthreadMutexInit
*   功     能：互斥锁的初始化
*             使用互斥锁（互斥）可以使线程按顺序执行。通常，互斥锁通过确保一次只有一个
*             线程执行代码的临界段来同步多个线程。互斥锁还可以保护单线程代码。
*             要更改缺省的互斥锁属性，可以对属性对象进行声明和初始化。通常，互斥锁属性
*             会设置在应用程序开头的某个位置，以便可以快速查找和轻松修改。
*             函数成功执行后，互斥锁被初始化为未锁住态。
*   输入参数：pstMutex：Linux环境下使用pthread_mutex_t数据类型表示互斥量，用户不需要了解
*               该类型的实现细节，在使用互斥量之前要对其进行初始化。Linux环境下使用
*               pthread_mutex_init()函数对互斥量进行初始化。
*               pstMutex是互斥量的指针，互斥量在该函数内被初始化，并通过此参数返回
*               给调用者。
*             pstAttr：互斥量的属性，在此先将该参数设置为NULL，内核会使用默认属性对
*               互斥量进行初始化。
*   输出参数：无
*   返 回 值：初始化成功返回0
*             失败则返回错误号
*   说     明：pthread_mutex_init()函数是以动态方式创建互斥锁的，参数attr指定了新建互斥锁
*             的属性。如果参数attr为空，则使用默认的互斥锁属性，默认属性为快速互斥锁 。
*             互斥锁的属性在创建锁的时候指定，在LinuxThreads实现中仅有一个锁类型属性，
*             不同的锁类型在试图对一个已经被锁定的互斥锁加锁时表现不同。
*             pthread_mutexattr_init()函数成功完成之后会返回零，其他任何返回值都表示出现
*             了错误。
*
*             Linux还提供了一种不使用pthread_mutexattr_init()函数初始化互斥量的方法，
*             用户可以将互斥量设置为PTHREAD_MUTEX_INITIALIZER。此种方法有个局限性，那就
*             是如果互斥量是使用动态分配内存的方法得到的，那么下面的初始化方法是错误的:
*               pthread_mutex_t *mutex;
*               mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
*               mutex = PTHREAD_MUTEX_INITIALIZER;
*             以上的初始化是错误的，原因在于Linux将pthread_mutex_t类型定义为结构体，而
*             PTHREAD_MUTEX_INITIALIZER常量相当于已经设置好的结构体变量中每个成员变量的
*             值。显而易见，一个已经定义的结构体对象可以使用这种方法，而一个使用malloc()
*             函数分配的结构体对象则不能够使用这种方法。正确的写法应该是这样的：
*               pthread_mutex_init(mutex, NULL);    //属性使用系统默认
*             如果要使用PTHREAD_MUTEX_INITIALIZER则应该写成：
*               pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
*   作     者：zhanxc
*   创建日期：018-7-24
*   修改历史：无
*******************************************************************************/
INT32 PthreadMutexInit(pthread_mutex_t *pstMutex, const pthread_mutexattr_t *pstAttr)
{
	return pthread_mutex_init(pstMutex, pstAttr);
}

/*******************************************************************************
*   函 数   名：PthreadMutexDestroy
*   功     能：销毁互斥量
*   输入参数：pstMutex：表示需要销毁的互斥量
*   输出参数：无
*   返 回 值：成功销毁互斥量返回0
*             失败则返回错误号
*   作     者：zhanxc
*   创建日期：018-7-24
*   修改历史：无
*******************************************************************************/
INT32 PthreadMutexDestroy(pthread_mutex_t *pstMutex)
{
	return pthread_mutex_destroy(pstMutex);
}

/*******************************************************************************
                        线程同步之互斥量
*******************************************************************************/

/*******************************************************************************
*   函 数   名：PthreadMutexLock
*   功     能：得到一个互斥量的锁（对临界区加锁）
*             如果该互斥量的锁已经被某一个线程得到，那么该函数会导致线程阻塞，直到
*             互斥量的锁被释放。
*   输入参数：pstMutex：表示一个互斥量,调用线程希望得到该互斥量的锁
*   输出参数：无
*   返 回 值：成功返回0
*             失败则返回错误号
*   作     者：zhanxc
*   创建日期：018-7-24
*   修改历史：无
*******************************************************************************/
INT32 PthreadMutexLock(pthread_mutex_t *pstMutex)
{
	return pthread_mutex_lock(pstMutex);
}

/*******************************************************************************
*   函 数   名：PthreadMutexTryLock
*   功     能：得到一个互斥量的锁（对临界区加锁）
*             该函数在得不到指定互斥量的锁时并不会导致调用线程阻塞，而是立即返回
*             一个错误编号EBUSY，表示锁申请的锁处于繁忙状态。
*             该函数实现一个非阻塞的加锁函数版本，用于忙等待一个互斥量的锁。
*   输入参数：pstMutex：表示一个互斥量,调用线程希望得到该互斥量的锁
*   输出参数：无
*   返 回 值：成功返回0
*             失败则返回错误号
*   作     者：zhanxc
*   创建日期：018-7-24
*   修改历史：无
*******************************************************************************/
INT32 PthreadMutexTryLock(pthread_mutex_t *pstMutex)
{
	return pthread_mutex_trylock(pstMutex);
}

/*******************************************************************************
*   函 数   名：PthreadMutexUnLock
*   功     能：释放一个互斥量的锁
*   输入参数：pstMutex：表示需要释放锁的互斥量
*   输出参数：无
*   返 回 值：成功返回0
*             失败则返回错误号
*   作     者：zhanxc
*   创建日期：018-7-24
*   修改历史：无
*******************************************************************************/
INT32 PthreadMutexUnLock(pthread_mutex_t *pstMutex)
{
	return pthread_mutex_unlock(pstMutex);
}

/*******************************************************************************
                        线程同步之读写锁
    读写锁是另一种线程之间同步的操作。如果没有同步操作，会导致线程在执行时出现与
    时间有关的错误。
    读写锁与互斥量类似，但是并行性更高，其原因在于互斥锁每次只有一个线程可以得到
    锁进行操作，其余的线程都以为得不到锁而处于阻塞状态。创建多线程进行操作的本意
    是为了并发执行任务，但是由于互斥锁的缘故导致线程的操作变成了串行的，由此可知
    程序整个运行的效率会大打折扣。
    读写锁的好处是线程根据操作类型分为两类：读线程和写线程。读线程只对共享资源
    进行读操作，并不改变共享资源；而写操作对共享资源进行写操作，会改变共享资源。
    因此多个读线程可以共同占有一把读写锁，而对于写线程而言，任意时刻只能有一个
    写线程占有读写锁。
    在程序执行时，如果对共享资源做读操作的线程远远大于写线程的时候，使用读写锁
    可以大大提高线程的并发度，从而提高程序运行的效率。Linux环境下使用pthread_rwlock_t
    结构类型表示读写锁，在使用读写锁之前要对其进行初始化。

    对于读操作远远大于写操作的应用场合，读写锁比互斥量的性能要优越的多。
*******************************************************************************/

/*******************************************************************************
*   函 数   名：PthreadRwLockInit
*   功     能：对读写锁进行初始化
*   输入参数：pstRwLock：读写锁的指针,读写锁在该函数内被初始化，并通过该参数返回给调用者
*             pstRwLockAttr：读写锁的属性，通常将该参数设置为NULL，内核会使用默认属性
*               对读写锁进行初始化。
*   输出参数：无
*   返 回 值：成功返回0
*             失败则返回错误号
*   作     者：zhanxc
*   创建日期：018-7-24
*   修改历史：无
*******************************************************************************/
INT32 PthreadRwLockInit(pthread_rwlock_t *pstRwLock, const pthread_rwlockattr_t *pstRwLockAttr)
{
	return pthread_rwlock_init(pstRwLock, pstRwLockAttr);
}

/*******************************************************************************
*   函 数   名：PthreadRwLockDestroy
*   功     能：销毁一个读写锁
*   输入参数：pstRwLock：表示需要销毁的读写锁。
*   输出参数：无
*   返 回 值：成功返回0
*             失败则返回错误号
*   作     者：zhanxc
*   创建日期：018-7-24
*   修改历史：无
*******************************************************************************/
INT32 PthreadRwLockDestroy(pthread_rwlock_t *pstRwLock)
{
	return pthread_rwlock_destroy(pstRwLock);
}

/*******************************************************************************
*   函 数   名：PthreadRwLockRdLock
*   功     能：在读模式下得到读写锁
*             调用线程希望在读模式下得到该读写锁。如果该读写锁已经被某一个线程在读
*             模式下得到，则测试线程仍然会得到该锁；如果该读写锁已经被某一个线程在
*             写模式下得到，或者有一个线程在写模式下等待该锁，这时pthread_rwlock_rdlock()
*             函数会导致线程阻塞，知道读写锁被释放。
*   输入参数：pstRwLock：表示一个读写锁
*   输出参数：无
*   返 回 值：成功返回0
*             失败则返回错误号
*   作     者：zhanxc
*   创建日期：018-7-24
*   修改历史：无
*******************************************************************************/
INT32 PthreadRwLockRdLock(pthread_rwlock_t *pstRwLock)
{
	return pthread_rwlock_rdlock(pstRwLock);
}

/*******************************************************************************
*   函 数   名：PthreadRwLockTryRdLock
*   功     能：在读模式下得到读写锁
*             pthread_rwlock_tryrdlock()函数在得不到读写锁的锁时，并不会导致调用线程
*               阻塞，而是立即返回一个错误编号EBUSY，表示锁申请的锁处于繁忙状态。对应于
*               读模式，写模式下也有一组得到读写锁的函数
*   输入参数：pstRwLock：表示一个读写锁
*   输出参数：无
*   返 回 值：成功返回0
*             失败则返回错误号
*   作     者：zhanxc
*   创建日期：018-7-24
*   修改历史：无
*******************************************************************************/
INT32 PthreadRwLockTryRdLock(pthread_rwlock_t *pstRwLock)
{
	return pthread_rwlock_tryrdlock(pstRwLock);
}

/*******************************************************************************
*   函 数   名：PthreadRwLockWrLock
*   功     能：在写模式下得到读写锁
*             由于写模式下得到锁的条件与读模式不同，因此，pthread_rwlock_wrlock()
*             函数的阻塞情况也与pthread_rwlock_rdlock()函数不同；如果在测试读写锁
*             的时候有任意一个线程占有该锁，pthread_rwlock_wrlock()函数就会导致阻塞，
*             并且会导致其后所有申请该读写锁的线程阻塞等待。
*   输入参数：pstRwLock：表示一个读写锁
*   输出参数：无
*   返 回 值：成功返回0
*             失败则返回错误号
*   作     者：zhanxc
*   创建日期：018-7-24
*   修改历史：无
*******************************************************************************/
INT32 PthreadRwLockWrLock(pthread_rwlock_t *pstRwLock)
{
	return pthread_rwlock_wrlock(pstRwLock);
}

/*******************************************************************************
*   函 数   名：PthreadRwLockTryWrLock
*   功     能：在写模式下得到读写锁
*             pthread_rwlock_trywrlock()函数在读写锁不可用的时候不会导致线程阻塞，而
*               是返回一个错误编号EBUSY。
*   输入参数：pstRwLock：表示一个读写锁
*   输出参数：无
*   返 回 值：成功返回0
*             失败则返回错误号
*   作     者：zhanxc
*   创建日期：018-7-24
*   修改历史：无
*******************************************************************************/
INT32 PthreadRwLockTryWrLock(pthread_rwlock_t *pstRwLock)
{
	return pthread_rwlock_trywrlock(pstRwLock);
}

/*******************************************************************************
*   函 数   名：PthreadRwLockUnLock
*   功     能：释放读写锁
*             无论线程在那种模式（读、写模式）下占有锁，都使用pthread_rwlock_unlock()
*               函数释放读写锁。
*   输入参数：pstRwLock：表示需要释放的读写锁
*   输出参数：无
*   返 回 值：成功返回0
*             失败则返回错误号
*   作     者：zhanxc
*   创建日期：018-7-24
*   修改历史：无
*******************************************************************************/
INT32 PthreadRwLockUnLock(pthread_rwlock_t *pstRwLock)
{
	return pthread_rwlock_unlock(pstRwLock);
}

/*******************************************************************************
                    线程属性
    设置线程的属性属于线程的高级操作。由于属性会影响到内核的行为，所以在这里笔者
    并不建议读者修改这些属性。尤其是线程的内核堆栈的大小，该值的改变影响非常大，
    在尝试修改之前一定要慎重。

    在使用pthread_create()函数创建线程时，可以通过设置其第二个参数attr设置线程的
    属性，如果该参数被设置为NULL，系统会使用默认属性来创建线程。线程的属性被组织
    在一个结构体中，用户无需掌握其实现细节，可以使用；pthread_attr_init()函数初始
    化属性结构。

    线程的属性结构包括线程的4个基本属性：
    1、detachstate：线程以分离状态创建
    2、guardsize：线程栈末尾的警戒缓冲区大小
    3、stackaddr：线程栈的最低地址
    4、stacksize：线程栈的大小
    以上4个基本属性都由系统提供函数进行设置，通过系统函数获得其值，用户不要直接
    对属性结构赋值，以免影响到程序代码的可移植性。第一个属性和线程的分离状态有关，
    其余的3个属性都涉及线程的栈问题。
    线程的分离状态：
    由于线程和进程一样，在内核中拥有进程控制结构存储线程结束状态之类的信息，所以
    系统中同样会出现僵尸线程。不过当进程结束运行时，这种僵尸线程就会随着进程的
    消失而消失，因此影响不是很严重。如果想避免这种僵尸线程，则应将线程设置为结束
    状态分离，这时线程的结束状态将不能再被进程中的其他线程得到，同时，保存线程结束
    状态信息的存储区域也将变得不能引用。在创建线程的时候，可以指定线程的结束状态
    和线程分离。在创建线程时将属性设置为PTHREAD_CREATE_DETACHED，表示该线程是一个
    状态分离的线程。最后调用pthread_join()函数验证该线程确实已经分离。
    在创建一个线程时，可以通过设置创建线程属性的方法将一个线程设置为状态分离。但是
    如果需要将一个已经创建的线程设置为状态分离时，可以使用Linux操作系统的支持。该
    系统支持将一个已经运行的线程设置为状态分离，这项操作需要调用系统提供的接口函数。
    Linux环境下使用pthread_detach()函数使一个已经创建的线程进入分离状态。
    线程栈的属性：
    线程的栈是线程的所有数据，这部分内容是不共享的。在一个进程内有多个线程，这些
    线程都拥有自己的栈，但是都要挤在进程的虚拟地址空间内。线程之间的栈内存必须保证
    彼此不冲突，线程才能正确地运行。因此，虽然大多数情况下，由系统为用户设置和分配
    线程的栈内存，但有时还是需要用户亲自对线程栈进行设置。
    线程基本属性中和线程栈有关的属性有3个，分别是线程栈的大小、线程栈的最低地址和
    线程栈末尾警戒区的大小。
    系统默认为一个新建的线程提供PAGESIZE个字节的栈空间，但是很多时候需要更改此值。
    有时该栈会不够用，例如，线程体函数是一个多层递归函数，该函数的特点就是还需要
    多层嵌套栈来保存执行过程，因此，该函数可能会造成现有的栈尺寸缩小。另一种情况
    则刚好相反，用户进程空间的大小是固定的，如果进程内的线程太多，那么就会造成进程
    的虚拟地址空间不够使用，这时减小每一个线程的栈的大小是一个必须采取的措施。

    线程属性主要包括如下属性：
    1、作用域（scope）
    2、栈尺寸（stack size）
    3、栈地址（stack address）
    4、优先级（priority）
    5、分离的状态（detached state）
    6、调度策略和参数（scheduling policy and parameters）。
    默认的属性为非绑定、非分离、缺省1M的堆栈、与父进程同样级别的优先级。

    A、作用域属性描述特定线程将与哪些线程竞争资源。线程可以在两种竞争域内竞争资源：
        1、进程域（process scope）：与同一进程内的其他线程。
        2、系统域（system scope）：与系统中的所有线程。一个具有系统域的线程将与整个
           系统中所有具有系统域的线程按照优先级竞争处理器资源，进行调度。
        3、Solaris系统，实际上，从 Solaris 9 发行版开始，系统就不再区分这两个范围。
    B、轻进程（LWP：Light Weight Process）关于线程的绑定，牵涉到另外一个概念：
        轻进程（LWP：Light Weight Process）：轻进程可以理解为内核线程，它位于用户层
        和系统层之间。系统对线程资源的分配、对线程的控制是通过轻进程来实现的，一个轻
        进程可以控制一个或多个线程。
        1、非绑定状态
            默认状况下，启动多少轻进程、哪些轻进程来控制哪些线程是由系统来控制的，
            这种状况即称为非绑定的。
        2、绑定状态
            绑定状况下，则顾名思义，即某个线程固定的"绑"在一个轻进程之上。被绑定的线程
            具有较高的响应速度，这是因为CPU时间片的调度是面向轻进程的，绑定的线程可以
            保证在需要的时候它总有一个轻进程可用。通过设置被绑定的轻进程的优先级和调度级
            可以使得绑定的线程满足诸如实时反应之类的要求。
    C、线程的分离状态（detached state）
        1、线程的分离状态决定一个线程以什么样的方式来终止自己。
        2、非分离状态
            线程的默认属性是非分离状态，这种情况下，原有的线程等待创建的线程结束。
            只有当pthread_join()函数返回时，创建的线程才算终止，才能释放自己占用的
            系统资源。
        3、分离状态
            分离线程没有被其他的线程所等待，自己运行结束了，线程也就终止了，马上释放
            系统资源。应该根据自己的需要，选择适当的分离状态。
        4、线程分离状态的函数：pthread_attr_setdetachstate（pthread_attr_t *attr, int detachstate）。
            第二个参数可选为PTHREAD_CREATE_DETACHED（分离线程）和 PTHREAD _CREATE_JOINABLE（非分离线程）。
            这里要注意的一点是，如果设置一个线程为分离线程，而这个线程运行又非常快，它很
            可能在pthread_create函数返回之前就终止了，它终止以后就可能将线程号和系统资源
            移交给其他的线程使用，这样调用pthread_create的线程就得到了错误的线程号。要避
            免这种情况可以采取一定的同步措施，最简单的方法之一是可以在被创建的线程里调用
            pthread_cond_timewait函数，让这个线程等待一会儿，留出足够的时间让函数
            pthread_create返回。设置一段等待时间，是在多线程编程里常用的方法。但是注意
            不要使用诸如wait()之类的函数，它们是使整个进程睡眠，并不能解决线程同步的问题。
    D、线程的优先级（priority）
        1、新线程的优先级为默认为0。
        2、新线程不继承父线程调度优先级(PTHREAD_EXPLICIT_SCHED)
        3、仅当调度策略为实时（即SCHED_RR或SCHED_FIFO）时才有效，并可以在运行时通过
            pthread_setschedparam()函数来改变，缺省为0。
    E、线程的栈地址（stack address）
        1、POSIX.1定义了两个常量_POSIX_THREAD_ATTR_STACKADDR 和
            _POSIX_THREAD_ATTR_STACKSIZE检测系统是否支持栈属性。
        2、也可以给sysconf函数传递_SC_THREAD_ATTR_STACKADDR或 
            _SC_THREAD_ATTR_STACKSIZE来进行检测。
        3、当进程栈地址空间不够用时，指定新建线程使用由malloc分配的空间作为自己的栈空间。
            通过pthread_attr_setstackaddr和pthread_attr_getstackaddr两个函数分别设置和
            获取线程的栈地址。传给pthread_attr_setstackaddr函数的地址是缓冲区的
            低地址（不一定是栈的开始地址，栈可能从高地址往低地址增长）。
    F、线程的栈大小（stack size）
        1、当系统中有很多线程时，可能需要减小每个线程栈的默认大小，防止进程的地址空间不够用
        2、当线程调用的函数会分配很大的局部变量或者函数调用层次很深时，可能需要增大线程栈的默认大小。
        3、函数pthread_attr_getstacksize和 pthread_attr_setstacksize提供设置。
    G、线程的栈保护区大小（stack guard size）
        1、在线程栈顶留出一段空间，防止栈溢出。
        2、当栈指针进入这段保护区时，系统会发出错误，通常是发送信号给线程。
        3、该属性默认值是PAGESIZE大小，该属性被设置时，系统会自动将该属性大小补齐为页大小的整数倍。
        4、当改变栈地址属性时，栈保护区大小通常清零。
    H、线程的调度策略（schedpolicy）
        POSIX标准指定了三种调度策略：先入先出策略 (SCHED_FIFO)、循环策略 (SCHED_RR) 
            和自定义策略 (SCHED_OTHER)。SCHED_FIFO 是基于队列的调度程序，对于每个优先级
            都会使用不同的队列。SCHED_RR 与 FIFO 相似，不同的是前者的每个线程都有一个执行
            时间配额。SCHED_FIFO 和 SCHED_RR 是对 POSIX Realtime 的扩展。SCHED_OTHER 
            是缺省的调度策略。
        1、新线程默认使用 SCHED_OTHER 调度策略。线程一旦开始运行，直到被抢占或者直到线程
            阻塞或停止为止。
        2、SCHED_FIFO
            如果调用进程具有有效的用户 ID 0，则争用范围为系统 (PTHREAD_SCOPE_SYSTEM) 
            的先入先出线程属于实时 (RT) 调度类。如果这些线程未被优先级更高的线程抢占，
            则会继续处理该线程，直到该线程放弃或阻塞为止。对于具有进程争用范围 
            (PTHREAD_SCOPE_PROCESS)) 的线程或其调用进程没有有效用户 ID 0 的线程，请
            使用 SCHED_FIFO，SCHED_FIFO 基于 TS 调度类。
        3、SCHED_RR
            如果调用进程具有有效的用户 ID 0，则争用范围为系统 (PTHREAD_SCOPE_SYSTEM)) 的
            循环线程属于实时 (RT) 调度类。如果这些线程未被优先级更高的线程抢占，并且这些
            线程没有放弃或阻塞，则在系统确定的时间段内将一直执行这些线程。对于具有进程争
            用范围 (PTHREAD_SCOPE_PROCESS) 的线程，请使用 SCHED_RR(基于 TS 调度类)。此外，
            这些线程的调用进程没有有效的用户 ID 0。
    I、线程并行级别（concurrency）
        应用程序使用 pthread_setconcurrency() 通知系统其所需的并发级别。
*******************************************************************************/

/*******************************************************************************
*   函 数   名：PthreadAttrInit
*   功     能：初始化线程属性
*   输入参数：pstAttr：指向属性结构的地址，该函数为属性结构分配内存空间，通过这个参数
*               返回其首地址。
*   输出参数：无
*   返 回 值：成功返回0
*             失败则返回错误号
*   作     者：zhanxc
*   创建日期：018-7-24
*   修改历史：无
*******************************************************************************/
INT32 PthreadAttrInit(pthread_attr_t *pstAttr)
{
	return pthread_attr_init(pstAttr);
}

/*******************************************************************************
*   函 数   名：PthreadAttrDestroy
*   功     能：销毁一个不用的属性结构
*   输入参数：pstAttr：表示需要销毁的属性结构的地址，该函数会将pthread_attr_init()
*               函数分配的内存空间释放，因此，这两个函数一定要配套出现，否则会造成
*               内存泄漏。
*   输出参数：无
*   返 回 值：成功返回0
*             失败则返回错误号
*   作     者：zhanxc
*   创建日期：018-7-24
*   修改历史：无
*******************************************************************************/
INT32 PthreadAttrDestroy(pthread_attr_t *pstAttr)
{
	return pthread_attr_destroy(pstAttr);
}

/*******************************************************************************
*   函 数   名：PthreadDetach
*   功     能：使一个已经创建的线程进入分离状态
*   输入参数：pstTid：表示需要分离结束状态的线程的线程ID。
*   输出参数：无
*   返 回 值：如果成功将线程设置为分离状态则返回0
*             失败则返回错误号
*   说     明：分离状态的线程由于会导致pthread_join()函数出错，所以主线程不能依靠
*             该函数等待线程执行完毕，而必须使用sleep()函数休眠等待线程执行完毕。
*             主线程结束了进程，那么线程直接就退出了。
*   作     者：zhanxc
*   创建日期：018-7-24
*   修改历史：无
*******************************************************************************/
INT32 PthreadDetach(pthread_t iTid)
{
	return pthread_detach(iTid);
}

/*******************************************************************************
*   函 数   名：PthreadAttrSetdetachstate
*   功     能：设置线程分离属性
*   输入参数：pstAttr：线程属性
*             iDetachState：线程分离状态
*   输出参数：无
*   返 回 值：如果成功返回0
*             失败则返回错误号
*   作     者：zhanxc
*   创建日期：018-7-24
*   修改历史：无
*******************************************************************************/
INT32 PthreadAttrSetdetachstate(pthread_attr_t *pstAttr, INT32 iDetachState)
{
	return pthread_attr_setdetachstate(pstAttr, iDetachState);
}

/*******************************************************************************
*   函 数   名：PthreadAttrGetdetachstate
*   功     能：得到线程分离状态属性
*   输入参数：pstAttr：线程属性
*   输出参数：piDetachState：线程分离状态
*   返 回 值：如果成功返回0
*             失败则返回错误号
*   作     者：zhanxc
*   创建日期：018-7-24
*   修改历史：无
*******************************************************************************/
INT32 PthreadAttrGetdetachstate(pthread_attr_t *pstAttr, INT32 *piDetachState)
{
	return pthread_attr_getdetachstate(pstAttr, piDetachState);
}

/*******************************************************************************
*   函 数   名：PthreadAttrSetstacksize
*   功     能：设置线程栈大小
*   输入参数：pstAttr：线程属性
*             iStackSize：线程栈大小
*   输出参数：无
*   返 回 值：如果成功返回0
*             失败则返回错误号
*   作     者：zhanxc
*   创建日期：018-7-24
*   修改历史：无
*******************************************************************************/
INT32 PthreadAttrSetstacksize(pthread_attr_t *pstAttr, size_t iStackSize)
{
	return pthread_attr_setstacksize(pstAttr, iStackSize);
}

/*******************************************************************************
*   函 数   名：PthreadAttrGetstacksize
*   功     能：得到线程栈大小的属性值
*   输入参数：pstAttr：线程属性
*   输出参数：piStackSize：线程栈大小
*   返 回 值：如果成功返回0
*             失败则返回错误号
*   作     者：zhanxc
*   创建日期：018-7-24
*   修改历史：无
*******************************************************************************/
INT32 PthreadAttrGetstacksize(pthread_attr_t *pstAttr, size_t *piStackSize)
{
	return pthread_attr_getstacksize(pstAttr, piStackSize);
}

