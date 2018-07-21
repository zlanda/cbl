/*******************************************************************************
*   文  件 名：Sem.c
*   功     能：封装信号量功能
*   作     者：zhanxc
*   E-Mail   : zhanxc_chpu@sina.com
*   创建日期：018-7-17
*   修改历史：无
*******************************************************************************/

#include "Sem.h"

/*******************************************************************************
*   函 数   名：CreateSemaphore
*   功     能：创建一个新的信号量或获取一个已经存在的信号量的键值
*   输入参数：iKey:为整数型，用户可以自己设定。有两种情况
*                  IPC_PRIVATE：使用IPC_PRIVATE创建的IPC对象, key值属性为0，和IPC对象
*                       的编号就没有了对应关系。这样毫无关系的进程，就不能通过key值来
*                       得到IPC对象的编号（因为这种方式创建的IPC对象的key值都是0）。因
*                       此，这种方式产生的IPC对象，和无名管道类似，不能用于毫无关系的进
*                       程间通信。但也不是一点用处都没有，仍然可以用于有亲缘关系的进程
*                       间通信。
*                  键值不是IPC_PRIVATE，我们可以指定键值，例如1234；也可以一个ftok()函数来
*                       取得一个唯一的键值
*             iSemNums:表示初始化信号量的个个数。比如我们要创建一个信号量，则该值为1，
*                   创建2个就是2。
*             iSemFlag:信号量的创建方式或权限。有IPC_CREATE、IPC_EXCL。
*                   IPC_CREAT:如果信号量不存在，则创建一个信号量，否则获取。
*                   IPC_EXCL:只有信号量不存在的时候，新的信号量才建立，否则就产生错误。
*   输出参数：无
*   返 回 值：成功返回信号量的标识码ID
*             失败返回-1
*   附加说明: 上述semflg参数为模式标志参数，使用时需要与IPC对象存取权限（如0600）进行|运算
*             来确定信号量集的存取权限
*   错 误 码: EACCESS：没有权限
*             EEXIST：信号量集已经存在，无法创建
*             EIDRM：信号量集已经删除
*             ENOENT：信号量集不存在，同时semflg没有设置IPC_CREAT标志
*             ENOMEM：没有足够的内存创建新的信号量集
*             ENOSPC：超出限制
*
*       如果用semget创建了一个新的信号量集对象时，则semid_ds结构成员变量的值设置如下：
*           sem_otime设置为0。
*           sem_ctime设置为当前时间。
*           msg_qbytes设成系统的限制值。
*           sem_nsems设置为nsems参数的数值。
*           semflg的读写权限写入sem_perm.mode中。
*           sem_perm结构的uid和cuid成员被设置成当前进程的有效用户ID，gid和cuid成员被设置成当前进程的有效组ID。
*
*   作     者：zhanxc
*   创建日期：018-7-20
*   修改历史：无
*******************************************************************************/
INT32 CreateSemaphore(key_t iKey, INT32 iSemNums, INT32 iSemFlag)
{
    return semget(iKey , iSemNums, iSemFlag);
}

/*******************************************************************************
*   函 数   名：OperationSemaphore
*   功     能：操作一个信号量集
*   输入参数：iSemId:是一个通过semget()函数返回的一个信号量标识符
*             astSemOpArray:是一个struct sembuf结构类型的数组指针，结构sembuf用
*                           来说明所要执行的操作。其定义如下：
*               struct sembuf
*               {
*                   unsigned short sem_num;     //是相对应的信号量集中的某一个资源
*                                               //所以其值是一个从0到相应的信号量集
*                                               //的资源总数(ipc_perm.sem_nsems)之间的整数
*                   short sem_op;               //指明所要执行的操作，sem_op的值是一个整数
*                                               
*                                               //>0:释放相应的资源数，将sem_op的值加到信号量的值上
*                                               //0:进程阻塞直到信号量的相应值为0，当信号量已经为0，函数立即返回。
*                                               //如果信号量的值不为0，则依据sem_flg的IPC_NOWAIT位决定函数动作。
*                                               //如果sem_flg指定IPC_NOWAIT,则semop()函数出错返回EAGAIN。
*                                               //如sem_flg没有指定IPC_NOWAIT，则将该信号量的semncnt值加1，然后
*                                               //进程挂起直到下述情况发生。信号量值为0，将信号量semzcnt的值减1，
*                                               //函数semop()成功返回；此信号量被删除（只有超级用户或创建用户进程拥有此权限），
*                                               //函数semop()出错返回EIDRM；进程捕捉到信号，并从信号处理函数返回，在此情况
*                                               //将此信号量的semncnt值减1，函数semop()出错返回EINTR
*
*                                               //<0:请求sem_op的绝对值的资源。如果相应的资源数可以满足请求，则将该
*                                               //信号量的值减去sem_op的绝对值，函数成功返回。当相应的资源数不能满足
*                                               //请求时，这个操作与sem_flg有关。如sem_flg指定IPC_NOWAIT，则semop()函数
*                                               //出错返回EAGAIN。如sem_flg没有指定IPC_NOWAIT，则将该信号量的semncnt值加1
*                                               //然后进程挂起直到下述情况发生：
*                                               //当相应资源数可以满足请求，该信号的值减去sem_op的绝对值，成功返回；
*                                               //此信号量被删除（只有超级用户或创建用户进程拥有此权限），函数semop()
*                                               //出错返回EIDRM；进程捕捉到信号，并从信号处理函数返回，在此情况将此信号量的
*                                               //的semncnt值减1，函数semop()出错返回EINTR
*                   short sem_flg;              //说明函数semop()的行为。
*               }
*             iNOps:标明了参数astSemOpArray所指向数组中的元素个数
*   输出参数：无
*   返 回 值：成功返回信号量的标识码ID
*             失败返回-1,错误原因存于errno中
*   错 误 码: E2BIG：一次对信号量个数的操作超过了系统限制
*             EACCESS：权限不够
*             EAGAIN：使用了IPC_NOWAIT，但操作不能继续进行
*             EFAULT：sops指向的地址无效
*             EIDRM：信号量集已经删除
*             EINTR：当睡眠时接收到其他信号
*             EINVAL：信号量集不存在,或者semid无效
*             ENOMEM：使用了SEM_UNDO，但无足够的内存创建所需的数据结构
*             ERANGE：信号量值超出范围
*
*           sops为指向sembuf数组，定义所要进行的操作序列。下面是信号量操作举例。
*           struct sembuf sem_get={0,-1,IPC_NOWAIT}; //将信号量对象中序号为0的信号量减1
*           struct sembuf sem_get={0,1,IPC_NOWAIT};  //将信号量对象中序号为0的信号量加1
*           struct sembuf sem_get={0,0,0};           //进程被阻塞，直到对应的信号量值为0
*           flag一般为0，若flag包含IPC_NOWAIT，则该操作为非阻塞操作。若flag包含SEM_UNDO，则
*               当进程退出的时候会还原该进程的信号量操作，这个标志在某些情况下是很有用的，比
*               如某进程做了P操作得到资源，但还没来得及做V操作时就异常退出了，此时，其他进程
*               就只能都阻塞在P操作上，于是造成了死锁。若采取SEM_UNDO标志，就可以避免因为进程
*               异常退出而造成的死锁。
*
*   作     者：zhanxc
*   创建日期：018-7-20
*   修改历史：无
*******************************************************************************/
INT32 OperationSemaphore(INT32 iSemId, struct sembuf astSemOpArray[], size_t iNOps)
{
    return semop(iSemId , astSemOpArray, iNOps);
}

/*******************************************************************************
*   函 数   名：semctl
*   函数原型：int semctl(int sem_id, int semnum, int cmd [, union semun arg])
*   功     能：完成对共享内存的控制
*   输入参数：sem_id:信号量标识
*             semnum:指定sem_id的信号集中的某一个信号灯，其类似于在信号量集资
*                    源数组中的下标，用来对指定资源进行操作。
*             cmd:定义函数所要进行的操作，其取值及表达的意义如表：
*               GETVAL:返回成员semnum的semval值
*               SETVAL:使用arg.val对该信号量的semnum.sempid赋值(需要参数arg)
*               GETPID:返回成员semnum的sempid值
*               GETNCNT:返回成员semnum的semncnt值
*               GETZCNT:返回成员semnum的semzcnt值
*               GETALL:将该信号量集的值赋值到arg.array(需要参数arg)
*               SETALL:使用arg、array数组中的值对信号量集赋值(需要参数arg)
*               IPC_RMID:删除信号量集。此操作只能由具有超级用户的进程或信号量集
*                        拥有者的进程执行，这个操作会影响到正在使用该信号量集的进程
*               IPC_SET:设置此信号量集的sem_perm.uid、sem_perm.gid以及sem_perm.mode的值。
*                        此错误只能由具有超级用户的进程或信号量集拥有者的进程执行
*               SPC_STAT:（需要参数arg）
*             arg:为可选参数，根据参数cmd的相关操作来选择使用，其定义如下：
*               union semun {
*                   int val;                    //SETVAL用的值
*                   struct semid_ds *buf;       //IPC_STAT、IPC_SET用的semid_ds结构
*                   unsigned short *array;      //SETALL、GETALL用的数组值
*                   struct seminfo *buf;        //为控制IPC_INFO提供的缓存
*               } arg;
*   输出参数：无
*   返 回 值：成功:返回值>0(当semctl的操作为GET操作时返回相应的值，其余返回0)
*             出错:-1，并设置错误变量errno。
*             EPERM:参数iCmd为IPC_SET或IPC_RMID，却无足够的权限执行
*   附加说明: semid_ds结构见上文信号量集内核结构定义
*   错 误 码: EACCESS：权限不够
*             EFAULT：arg指向的地址无效
*             EIDRM：信号量集已经删除
*             EINVAL：信号量集不存在，或者semid无效
*             EPERM：进程有效用户没有cmd的权限
*             ERANGE：信号量值超出范围
*
*   命令解释:
*       IPC_STAT: 从信号量集上检索semid_ds结构，并存到semun联合体参数的成员buf的地址中
*       IPC_SET: 设置一个信号量集合的semid_ds结构中ipc_perm域的值，并从semun的buf中取出值
*       IPC_RMID: 从内核中删除信号量集合
*       GETALL: 从信号量集合中获得所有信号量的值，并把其整数值存到semun联合体成员的一个指针数组中
*       GETNCNT: 返回当前等待资源的进程个数
*       GETPID: 返回最后一个执行系统调用semop()进程的PID
*       GETVAL: 返回信号量集合内单个信号量的值
*       GETZCNT: 返回当前等待100%资源利用的进程个数
*       SETALL: 与GETALL正好相反
*       SETVAL: 用联合体中val成员的值设置信号量集合中单个信号量的值
*
*   作     者：zhanxc
*   创建日期：018-7-20
*   修改历史：无
*******************************************************************************/

/*******************************************************************************
*   函 数   名：InitSemaphoreValue
*   功     能：初始化信号的值
*   输入参数：iSemId: 信号量标识
*             iSemVal: 初始化信号量的值
*   输出参数：无
*   返 回 值：成功返回信号量的标识码ID
*             失败返回-1,错误原因存于errno中
*   作     者：zhanxc
*   创建日期：018-7-20
*   修改历史：无
*******************************************************************************/
INT32 InitSemaphoreValue(INT32 iSemId, INT32 iValue)
{
    return ControlSemaphore(iSemId, 0, SETVAL, iValue);
}

/*******************************************************************************
*   函 数   名：SetSemaphoreValue
*   功     能：设置信号量的值,+iSemVal/-iSemVal
*   输入参数：iSemId: 信号量标识
*             iSemVal: 信号量的值
*   输出参数：无
*   返 回 值：成功返回信号量的标识码ID
*             失败返回-1,错误原因存于errno中
*   作     者：zhanxc
*   创建日期：018-7-20
*   修改历史：无
*******************************************************************************/
INT32 SetSemaphoreValue(INT32 iSemId, INT32 iSemVal)
{
    struct sembuf stSemBuffer;

    stSemBuffer.sem_num = 0;            //first sem(index=0)
    stSemBuffer.sem_flg = SEM_UNDO;
    stSemBuffer.sem_op = iSemVal;

    return OperationSemaphore(iSemId, &stSemBuffer, 1);
}

/*******************************************************************************
*   函 数   名：GetSemaphoreValue
*   功     能：获取信号量的值
*   输入参数：iSemId: 信号量标识
*   输出参数：无
*   返 回 值：信号量的值
*   作     者：zhanxc
*   创建日期：018-7-20
*   修改历史：无
*******************************************************************************/
INT32 GetSemaphoreValue(INT32 iSemId)
{
    return ControlSemaphore(iSemId, 0, GETVAL);
}

/*******************************************************************************
*   函 数   名：DelSemaphore
*   功     能：删除信号量
*   输入参数：iSemId: 信号量标识
*   输出参数：无
*   返 回 值：失败返回-1
*   作     者：zhanxc
*   创建日期：018-7-20
*   修改历史：无
*******************************************************************************/
INT32 DelSemaphore(INT32 iSemId)
{
    return ControlSemaphore(iSemId, 0, IPC_RMID);
}

