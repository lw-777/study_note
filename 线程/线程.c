
线程设计
{
	线程的基本概念
	{
		进程：进程是资源管理的最小单位；
		线程：线程是程序执行的最小单位，通常把线程叫做轻量的进程；

		演化出线程的目的：更好的支持多处理器，并且减少进程上下文切换的开销

	}

	进程和线程的关系
	{
		线程时属于进程的，线程运行在进程空间内，同一进程产生的所有线程共享同一用户内存空间；
			当进程退出时，改进程产生的多有线程都会被强制退出并清除；

		默认情况下，一个进程在运行时至少有一个并且只有一个线程(主控线程)；
		开发人员可以通过主控线程用代码创建多个线程(子线程)；

	}

	线程分类
	{
		按照线程的调度者分类
		{
			1、用户级线程：(程序中会涉及)
				主要解决的是上下文切换的问题，其调度过程有用户决定；
				每一个用户级线程都会和某一个内核级线程进行绑定；
			2、内核级线程：
				有内核调度机制实现；
				
		}

	}

	Linux线程实现
	{
		pthread线程库
	}

	线程标识
	{
		线程ID：
	}

	线程的创建
	{
		int pthread_create(pthread_t *thread, const pthread_attr_t *attr,void *(*start_routine) (void *), void *arg);

		返回：成功返回0，否则返回错误编号

		不能保证子线程和调用线程的执行顺序

	}

	线程的局部变量
	{
		线程的局部变量是不会共享的

	}

	进程的全局变量
	{
		进程的全局变量是所有线程共享的，包括数据段的全局变量和堆区手动开辟的空间
	}

	线程终止
	{
		1、主动终止：
			void pthread_exit(void *retval);
			退出后，线程所占用的资源并不会随着线程的结束而释放，所以需要pthread_join()函数来等待线程结束，类似于wait系统调用
		2、被动终止：
			int pthread_cancel(pthread_t thread);

	}

	线程清理和控制函数
	{
		void pthread_cleanup_push(void (*routine)(void *),void *arg);
			routine：自定义的清理函数，类型如上
		void pthread_cleanup_pop(int execute);

		这一组函数是成对出现
		等同于：
			while(execute)
			{
				//执行线程处理函数
			}
		返回：成功返回0，否则返回错误编号
	}

	进程和线程的比较
	{
		---------------------------------------------------------
		|			进程			|			线程			|
		---------------------------------------------------------
		|			fork()			|	 pthread_create() 		|
		---------------------------------------------------------
		|	return/exit()/_exit()	|	return/pthread_exit()	|
		---------------------------------------------------------
		|			wait()			|		pthread_join()		|
		---------------------------------------------------------
		|			atexit()		|	pthread_cleanup_push()/	|
		|							|	pthread_cleanup_pop()	|
		---------------------------------------------------------

	}

	线程的状态转换
	{
														pthread_join()
		新的线程	------>		就绪态	------>	运行态 	------> 	阻塞态
				  pthread_create()		获取CPU		    sleep()
	}

	线程属性的初始化和销毁
	{
		int pthread_attr_init(pthread_attr_t *attr);

		int pthread_attr_destroy(pthread_attr_t *attr);

		attr：结构体指针

		返回：成功返回0，否则返回错误编号
	}

	设置和获得分离属性
	{
		int pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate);				//设置分离属性

		int pthread_attr_getdetachstate(const pthread_attr_t *attr, int *detachstate);		//获得分离属性
			
			attr：结构体指针
			
			detachstate 取值：
				PTHREAD_CREATE_JOINABLE(默认值)		正常启动线程
				PTHREAD_CREATE_DETACHED				以分离状态启动线程

		返回：成功返回0，否则返回错误编号

		！以默认方式启动的线程，在线程运行结束后不会自动释放占用的系统资源，只有在主线程中调用pthread_join()后才会释放
		！以分离状态启动的线程，在线程结束后会自动释放所占用的系统资源

	}

	线程的同步和互斥
	{
		互斥：线程执行的相互排斥
		{
			互斥锁
			{
				类型：pthread_mutex_t
				
				int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutex_attr_t *mutexattr);
					初始化互斥锁，创建一把互斥锁
					
				int pthread_mutex_destroy(pthread_mutex_t *mutex);
					销毁互斥锁
					
				返回：成功返回0，否则返回错误编号
				
				mutex:互斥锁
				mutexattr:互斥锁创建方式
					PTHREAD_MUTEX_INITIALIZER ---> 创建快速互斥锁
					PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP ---> 创建递归互斥锁
					PTHREAD_ERRORCHECK_MUTEX_INITIALIZER_NP ---> 创建检错互斥锁
					
				int pthread_mutex_lock(pthread_mutex_t *mutex);
					上锁，拿不到锁阻塞
				
				int pthread_mutex_trylock(pthread_mutex_t *mutex);
					上锁，拿不到锁返回出错信息
					
				int pthread_mutex_unlock(pthread_mutex_t *mutex);
					释放锁
					
				返回：成功返回0，否则返回错误编号

				临界区：对共享资源进行解锁到释放锁的区域，就叫临界区
					必须成功获取到互斥锁，并且成功上锁以后，才能够进入到临界区，否则就不能进入
				
				互斥锁属性创建和销毁
				{
					int pthread_mutexattr_init(pthread_mutexattr_t *attr);
					
					int pthread_mutexattr_destroy(pthread_mutexattr_t *attr);
					
					返回：成功返回0，否则返回错误编号
					
					互斥锁进程共享属性操作
					{
						int pthread_mutexattr_getpshared(const pthread_mutexattr_t *attr, int * pshared);
						
						int pthread_mutexattr_setpshared(pthread_mutexattr_t *attr, int pshared);
						
						返回：成功返回0，否则返回错误编号
						
						pshared: 进程共享属性
							PTHREAD_PROCESS_PRIVATE (默认值) ---> 锁只能用于一个进程内部的两个线程进行互斥
							PTHREAD_PROCESS_SHARED			 ---> 锁可以用于两个不同进程中的线程进行互斥
						
					}
					
					互斥锁类型操作
					{
						int pthread_mutexattr_gettype(const pthread_mutexattr_t *attr, int *type);
						
						int pthread_mutexattr_settype(pthread_mutexattr_t *attr, int type);
						
						返回：成功返回0，否则返回错误编号
						
						type：互斥锁类型
							1、标准互斥锁
								PTHREAD_MUTEX_NORMAL	--->	第一次上锁成功，第二次上锁会阻塞
							2、递归互斥锁
								PTHREAD_MUTEX_RECURSIVE	--->	第一次上锁成功，第二次以后上锁还是成功，内部计数
							3、检错互斥锁
								PTHREAD_MUTEX_ERRORCHECK--->	第一次上锁成功，第二次上锁回出错							
							4、默认互斥锁
								PTHREAD_MUTEX_DEFAULT	--->	同标准互斥锁
					}
				}
			}
		
			读写锁
			{
				线程使用互斥锁缺乏读并发性；
				当读操作较多，写操作较少时，可使用读写锁提高线程读并发性；
				读写锁数据类型：
					pthread_rwlock_t
				
				读写锁创建和销毁
				{
					int pthread_rwlock_init(pthread_rwlock_t *rwlock, const pthread_rwlockattr_t *attr);
					
					int pthread_rwlock_destroy(pthread_rwlock_t *rwlock);
					
					返回：成功返回0，否则返回错误编号
					
					rwlock: 读写锁
					attr：读写锁属性(使用默认即可)
				}
			
				读写锁加锁和解锁
				{
					int pthread_rwlock_rdlock(pthread_rwlock_t *rwlock);	//加读锁
								
					int pthread_rwlock_wrlock(pthread_rwlock_t *rwlock);	//加写锁

					int pthread_rwlock_unlock(pthread_rwlock_t *rwlock);	//释放锁
					
					返回：成功返回0，否则返回错误编号
				}
			
				读写锁的特性
				{
					1、读和读：不排斥
					
					2、读和写/写和读：排斥
					
					3、写和写：排斥
				}
			}
		}

		同步：包含线程的相互排斥和线程执行的先后问题
		{
			条件变量
			{
				内部是一个等待队列，放置等待的线程，线程在条件变量上等待和通知，
					互斥锁用来保护等待队列(对等待队列上锁)，条件变量通常和互斥锁一起使用
				
				条件变量允许等待特定条件的发生，当条件不满足的时候，线程通常线进入阻塞状态，等待条件发生变化
					一旦其他的某个线程改变了条件，可唤醒一个或多个阻塞的线程
					
				具体的判断条件需要开发人员给出
				
				数据类型：
					pthread_cond_t
					
				条件变量创建和销毁
				{
					int pthread_cond_init(pthread_cond_t *cond, pthread_condattr_t *attr);
					
					int pthread_cond_destroy(pthread_cond_t *cond);
					
					返回：成功返回0，否则返回错误编号
				}
			
				条件变量等待操作
				{
					int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);
						一直阻塞等待
						//----------------------------------------------------------------
						！！！！！！
						wait操作为什么要放在unlock之前，而且wait操作之前已经上锁了
						{
							pthread_cond_wait(&cond, &mutex)函数内部流程
							{
								1、释放锁
									pthread_mutex_unlock(&mutex);
								
								2、上锁
									pthread_mutex_lock(&mutex);		
								
								3、将线程自己插入到条件变量的等待队列中
								
								4、释放锁
									pthread_mutex_unlock(&mutex);
									
								5、当前等待的线程阻塞 ---> 等待其他线程通知唤醒
								
								6、在唤醒后，上锁
									pthread_mutex_lock(&mutex);
								
								7、从等待队列中删除线程自己
								
							}
						}
						//----------------------------------------------------------------
				
					int pthread_cond_timewait(pthread_cond_t *cond, pthread_mutex_t *mutex, const struct timespec *tiemout);
						超时返回
					
					返回：成功返回0，否则返回错误编号
					
					struct timespec
					{
						time_t	tv_sec;		//秒
						long 	tv_nsec;	//纳秒
					}
					
					互斥锁mutex是对条件变量的保护
					
					线程由于调用wait函数阻塞，否则释放互斥锁
				
				}
			
				条件变量通知操作
				{
					int pthread_cond_signal(pthread_cond_t *cond);
						通知单个线程
					
					int pthread_cond_broadcast(pthread_cond_t *cond);
						通知所有线程
						
					返回：成功返回0，否则返回错误编号
				}
			
			}
		
			线程信号量
			{
				本质：一个非负整数的计数器
				
				数据类型：
					sem_t
					
				信号量的创建和销毁
				{
					int sem_init(sem_t *sem, int pshared, unsigned value);
					
					int sem_destroy(sem_t *sem);
					
					返回：成功返回0，否则返回错误编号
					
					pshared: 是否在进程间共享的标志，0为不共享，1为共享
				}
				
				信号量的加和减操作
				{
					int sem_post(sem_t *sem);
						功能：增加信号量 ---> +1
					
					int sem_wait(sem_t *sem);
						功能：减少信号量 ---> -1
					
					int sem_trywait(sem_t *sem);
						功能：sem_wait() 的非阻塞版本
						
					返回：成功返回0，否则返回错误编号
					
				}
				
				PV操作(原语)
				{
					加减的步长可以自己设定！
					P操作：加
					V操作：减
				}
			}
		} 
		死锁
		{
			产生原因
			{
				两个线程试图同时占用两个资源，并按不同的次序锁定相应的共享资源
			}
			
			解决方法
			{
				1、按相同的次序锁定相应的共享资源
				
				2、使用函数 pthread_mutex_trylock(), 他是函数pthread_mutex_lock()的非阻塞函数
				
			}
		}
	}
	
	线程和信号
	{
		1、进程中每个线程都有自己的信号屏蔽字和信号未决字
		
		2、信号的处理方式是进程中所有线程共享的
		
		3、进程中的信号是传递给单个线程的
		
		4、定时器是进程资源，进程中所有的线程共享相同的定时器
			子线程调用alarm()函数产生的alarm信号发送给主控线程
		
			int pthread_sigmask(int how, const sigset_t *set, sigset_t *oset)
				功能：线程的信号屏蔽
			返回：成功返回0，否则返回错误编号
		
	}
}  

最后练习
{
	1、利用线程信号量使三个线程交替循环输出10次
	2、读者和写者问题(一个写者和多个读者)，利用条件变量完成
	3、读者和写者问题(一个写者和一个读者)，利用线程信号量完成
	4、编写一个需要进行保护的链表操作，利用互斥锁或线程信号量完成
	5、哲学家就餐问题
		//四个哲学家四根筷子(线程互斥)经典多线程问题
		
	6、生产者和消费者问题
}
























