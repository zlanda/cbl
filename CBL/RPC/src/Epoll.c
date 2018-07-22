/*******************************************************************************
*   文  件 名：Epoll.c
*   功     能：封装Epoll功能
*   作     者：zhanxc
*   E-Mail   : zhanxc_chpu@sina.com
*   创建日期：018-7-21
*   修改历史：无
*******************************************************************************/

#include "Epoll.h"

/*******************************************************************************
Epoll事件有两种模型：
Edge Triggered(ET)	//高速工作方式，错误率比较大，只支持no_block socket(非阻塞socket)
Level Triggered(LT) //缺省工作方式，即默认的工作方式，支持block socket和no_block socket，
					//错误率比较小。
例如有这样一个例子：（LT方式，即默认方式下，内核会继续通知，可以读数据，ET方式，内核
					  不会再通知，可以读数据）
1、我们已经把一个用来从管道中读取数据的文件句柄（RFD）添加到Epoll描述符
2、这个时候从管道的另一端被写入了2KB的数据
3、调用epoll_wait(2)，并且它会返回RFD，说明它已经准备好读取操作
4、然后我们读取了1KB的数据
5、调用epoll_wait(2)......

Edge Triggered工作模式：
	如果我们在第1步将RFD添加到epoll描述符的时候使用了EPOLLET标志，那么
	在第5步调用epoll_wait(2)之后将有可能会挂起，因为剩余的数据还存在于
	文件的输入缓冲区内，而且数据发出端还在等待一个针对已经发出数据的反馈
	信息。只有在监视的文件句柄上发生了某个事件的时候ET工作模式才会汇报
	事件。因此在第5步的时候，调用者可能会放弃等待仍存在于文件输入缓冲区
	内的剩余数据。在上面的例子中，会有一个事件产生在RFD句柄上，因为在第2步
	执行了一个写操作，然后，事件将会在第3步被销毁。因为第4步的读取操作没有
	读空文件输入缓冲区内的数据，因此我们在第5步调用epoll_wait(2)完成后，是
	否挂起是不确定的。epoll工作在ET模式的时候，必须使用非阻塞套接口，以避免
	由于一个文件句柄的阻塞读/阻塞写操作把处理多个文件描述符的任务饿死。最好
	以下面的方式调用ET模式的epoll接口，在后面会介绍避免可能的缺陷。(LT方式
	可以解决这种缺陷)
  		i   基于非阻塞文件句柄
  		ii  只有当read(2)或者write(2)返回EAGAIN时(认为读完)才需要挂起，
  			等待。但这并不是说每次read()时都需要循环读，直到读到产生一个
  			EAGAIN才认为此次事件处理完成，当read()返回的读到的数据长度小于
  			请求的数据长度时(即小于sizeof(buf))，就可以确定此时缓冲中已没有
  			数据了，也就可以认为此事读事件已处理完成。

Level Triggered工作模式     (默认的工作方式)
     相反的，以LT方式调用epoll接口的时候，它就相当于一个速度比较快的poll(2)，
     并且无论后面的数据是否被使用，因此他们具有同样的职能。因为即使使用ET模
     式的epoll，在收到多个chunk的数据的时候仍然会产生多个事件。调用者可以设
     定EPOLLONESHOT标志，在epoll_wait(2)收到事件后epoll会与事件关联的文件句
     柄从epoll描述符中禁止掉。因此当EPOLLONESHOT设定后，使用带有EPOLL_CTL_MOD
     标志的epoll_ctl(2)处理文件句柄就成为调用者必须作的事情。

然后详细解释ET, LT:
	//没有对就绪的fd进行IO操作，内核会不断的通知。
	LT(leveltriggered)是缺省的工作方式，并且同时支持block和no-blocksocket。
	在这种做法中，内核告诉你一个文件描述符是否就绪了，然后你可以对这个就绪
	的fd进行IO操作。如果你不作任何操作，内核还是会继续通知你的，所以，这种
	模式编程出错误可能性要小一点。传统的select/poll都是这种模型的代表。
	//没有对就绪的fd进行IO操作，内核不会再进行通知。
	ET(edge-triggered)是高速工作方式，只支持no-blocksocket。在这种模式下，
	当描述符从未就绪变为就绪时，内核通过epoll告诉你。然后它会假设你知道文件
	描述符已经就绪，并且不会再为那个文件描述符发送更多的就绪通知，直到你做了
	某些操作导致那个文件描述符不再为就绪状态了(比如，你在发送，接收或者接收
	请求，或者发送接收的数据少于一定量时导致了一个EWOULDBLOCK错误）。但是请
	注意，如果一直不对这个fd作IO操作(从而导致它再次变成未就绪)，内核不会发
	送更多的通知(only once),不过在TCP协议中，ET模式的加速效用仍需要更多的
	benchmark确认（这句话不理解）。

	另外，当使用epoll的ET模型(epoll的非默认工作方式)来工作时，当产生了一个
	EPOLLIN事件后，读数据的时候需要考虑的是当recv()返回的大小如果等于要求的
	大小，即sizeof(buf)，那么很有可能是缓冲区还有数据未读完，也意味着该次
	事件还没有处理完，所以还需要再次读取：

*******************************************************************************/

/*******************************************************************************
*   函 数   名：CreateEpoll
*   功     能：创建Epoll句柄
*   输入参数：uiMaxEventsNum:用来告诉内核这个监听的数目一共有多大。
*   输出参数：无
*   返 回 值：成功返回Epoll ID
*			  失败返回-1
*	注     意: 当创建好Epoll句柄后，它就只会占用一个fd值，在Linux下
*			  如果查看/proc/进程id/fd/，是能够看到这个fd的，所以在
*			  使用完Epoll后，必须调用close()关闭，否则可能导致fd被
*			  耗尽。
*			  该函数生成一个Epoll专用的文件描述符。它其实是在内核申请
*			  空间，用来存放你想关注的socket fd上是否发生以及发生了什么
*			  事件。uiMaxEventsNum就是你在这个Epoll fd上能关注的最大socket fd
*			  数。随你定好了。只要你有空间。
*   作     者：zhanxc
*   创建日期：018-7-21
*   修改历史：无
*******************************************************************************/
INT32 CreateEpoll(UINT32 uiMaxEventsNum)
{
	INT32 iEpollFd = -1;
	
	iEpollFd = epoll_create(uiMaxEventsNum);  
	if (iEpollFd < 0)  
	{  
		return CBL_EPOLL_ERR;
	}

	return iEpollFd;
}

/*******************************************************************************
*   函 数   名：CloseEpoll
*   功     能：关闭Epoll
*   输入参数：iEpollFd:Epoll事件描述符
*   输出参数：无
*   返 回 值：无
*   作     者：zhanxc
*   创建日期：018-7-21
*   修改历史：无
*******************************************************************************/
VOID CloseEpoll(INT32 iEpollFd)
{
	if (iEpollFd < 0)
	{
		return;
	}
	
	close(iEpollFd);
	
	return;
}

/*******************************************************************************
*   函 数   名：ControlEpoll
*   功     能：将被监听的描述符添加到Epoll句柄或从Epoll句柄中删除或者对监听事件进行修改
*			  该函数用于控制某个Epoll文件描述符上的事件，可以注册事件，修改事件，删除事件
*   输入参数：iEpollFd:由epoll_create生成的epoll专用的文件描述符
*			  iOp:要进行的操作，例如注册事件，可能的取值EPOLL_CTL_ADD注册、
*				  EPOLL_CTL_MOD修改、EPOLL_CTL_DEL删除
*			  iFd:关联的文件描述符，是需要监听的fd
*			  pstEvent:指向epoll_event的指针，是告诉内核需要监听什么事件。
*			  epoll_event结构定义如下：
*			  typedef union epoll_data {  
*			  		void *ptr;
*			  		int fd;  
*			  		__uint32_t u32;  
*			  		__uint64_t u64;  
*			  } epoll_data_t;  
*
*			  struct epoll_event {  
*			  		__uint32_t events; // Epoll events   
*			  		epoll_data_t data; // User data variable  
*			  }; 
*
*			  events可以是以下几个宏的集合:
*			  EPOLLIN：触发该事件，表示对应的文件描述符上有可读数据。
*					(包括对端SOCKET正常关闭)；
*			  EPOLLOUT：触发该事件，表示对应的文件描述符上可以写数据；
*  			  EPOLLPRI：表示对应的文件描述符有紧急的数据可读（这里应该表示有带外数据到来）；
*			  EPOLLERR：表示对应的文件描述符发生错误；
*			  EPOLLHUP：表示对应的文件描述符被挂断；
*			  EPOLLET：将EPOLL设为边缘触发(Edge Triggered)模式，这是相对于水平触发(Level Triggered)来说的。
*			  EPOLLONESHOT：只监听一次事件，当监听完这次事件之后，如果还需要继续监听这个socket的话，
*					需要再次把这个socket加入到EPOLL队列里。
*   输出参数：无
*   返 回 值：成功返回0
*			  失败返回-1
*	注     意：epoll的事件注册函数，它不同与select()是在监听事件时告诉内核要监听什么类型的
*			  事件，而是在这里先注册要监听的事件类型
*			  例如：
*			  	struct epoll_event ev;
*			  	//设置与要处理的事件相关的文件描述符
*			  	ev.data.fd=listenfd;
*			  	//设置要处理的事件类型
*			  	ev.events=EPOLLIN|EPOLLET;
*			  	//注册epoll事件
*			  	epoll_ctl(epfd,EPOLL_CTL_ADD,listenfd,&ev);
*   作     者：zhanxc
*   创建日期：018-7-22
*   修改历史：无
*******************************************************************************/
INT32 ControlEpoll(INT32 iEpollFd, INT32 iOp, INT32 iFd, struct epoll_event *pstEvent)
{
	return epoll_ctl(iEpollFd, iOp, iFd, pstEvent);
}

/*******************************************************************************
*   函 数   名：WaitEpoll
*   功     能：等待事件触发，当超过iTimeout还没有事件触发时，就超时
*			  等待事件的产生，类似于select()调用。
*			  该函数用于轮询I/O事件的发生。
*			  等待注册在iEpollFd上的socket fd的事件的发生，如果发生则将发生的socket fd和事件
*			  类型放入到pstEvents数组中。并且将注册在iEpollFd上的socket fd的事件类型给清空，
*			  所以如果下一个循环你还要关注这个socket fd的话，则需要用epoll_ctl(iEpollFd, EPOLL_CTL_MOD, iListenFd, &event)
*			  来重新设置socket fd的事件类型。这时不用EPOLL_CTL_ADD，因为socket fd并未清空，只是
*			  事件类型清空。这一步非常重要。
*   输入参数：iEpollFd:Epoll事件描述符，由epoll_create生成的epoll专用的文件描述符
*			  pstEvents:用来从内核得到事件的集合，用于回传处理事件的数组。
*			  iMaxEvents:告之内核这个events有多大（数组成员的个数），这个
*				iMaxEvents的值不能大于创建epoll_create()时的uiMaxEventsNum。
*				每次能处理的事件数。
*			  iTimeout:超时时间(毫秒，0会立即返回，-1将不确定，也有说法说是永久阻塞)
*				等待I/O事件发生的超时值；-1相当于阻塞，0相当于非阻塞。一般用-1即可
*   输出参数：无
*   返 回 值：该函数返回需要处理的事件数目，如返回0表示已超时。
*			  返回的事件集合在events数组中，数组中实际存放的成员个数是函数的返回值。
*			  返回0表示已经超时。
*			  返回发生事件数。
*   作     者：zhanxc
*   创建日期：018-7-21
*   修改历史：无
*******************************************************************************/
INT32 WaitEpoll(INT32 iEpollFd, struct epoll_event *pstEvents, INT32 iMaxEvents, INT32 iTimeout)
{	
	return epoll_wait(iEpollFd, pstEvents, iMaxEvents, iTimeout);
}

/*******************************************************************************
*   函 数   名：SetFdNonBlocking
*   功     能：设置文件描述符为非阻塞模式
*   输入参数：iSockFd:Socket文件描述符
*   输出参数：无
*   返 回 值：无
*   作     者：zhanxc
*   创建日期：018-7-22
*   修改历史：无
*******************************************************************************/
INT32 SetFdNonBlocking(INT32 iSockFd)
{
    if (CBL_FCTL_ERR == fcntl(iSockFd, F_SETFL, fcntl(iSockFd, F_GETFD, 0) | O_NONBLOCK)) 
    {
        return CBL_FCTL_ERR;
    }
    
    return CBL_FCTL_SUCC;
}

