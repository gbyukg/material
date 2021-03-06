/* Copyright (C)
 * 2016 - Lock
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 */

/**
 * @file pthread.h
 * @brief POSIX 线程
 *
 * 一个进程中可以包含多个线程, 每个线程都会独立执行相同程序,
 * 且共享同一份全局内存区域, 其中包括初始化数据段, 未初始化数据段,
 * 以及堆内存段.
 *
 * 相对于多进程来说, 线程有更多的优势:
 *   - 速度更快. fork() 即便使用了写时复制技术, 仍然需要复制诸如内存页表和文件描述符表之类的多重进程属性,
 *   这意味着 fork() 调用在时间上的开销依然不菲.
 *   线程是基于 closen() 函数实现的, fork() 复制的诸多属性, 在线程中本来就是共享的.
 *   特别是, 既无需采用写时复制来复制内存页, 也无需复制制表页.
 *   这就是线程的创建要快于进程的创建 10 至 100 倍. 因为这一点,
 *   可以大量创建线程而无需太过于担心 CPU 和内存不足的问题.
 *   - 线程之间能够方便, 快速的共享信息. 只需将数据复制到共享(全局或堆)变量中即可.
 *   不过应当避免多个线程之间试图同时修改同一变量的情况发生.
 *
 * Pthreads 系列的所有函数执行成功后后返回 0, 但失败后却返回一个正数.
 *
 * 共享属性 | 非共享属性
 * -------- | ---------
 * 进程ID(process ID) 和 父进程ID | 线程 ID
 * 进程组 ID 与 会话 ID | 信号掩码 (signal mask)
 * 控制终端 | 线程特有数据
 * 进程凭证 (process credential) (用户 ID 和 组 ID) | 信号备选栈 (sigaltstack())
 * 打开的文件描述符 | errno 变量
 * 由 fcntl() 创建的记录所 (record lock) | 浮点型 (floating-point) 环境
 * 信号处置 | 实时调度策略 和 优先级
 * 文件系统相关的信息: 文件权限掩码 (umask), 当前工作目录和根目录 | CPU 亲和力
 * 间隔定时器(setitimer()) 和 POSIX 定时器(timer_create()) | 能力
 * 系统 V 信号量撤销值 | 栈, 本地变量和函数的调用链接 (linkage) 信息
 * 资源限制 | <BR>
 * CPU 时间消耗 (由 times() 返回) | <BR>
 * 资源消耗 (由 getrusage() 返回) | <BR>
 * nice 值 (由 setpriority() 和 nice() 设置) | <BR>
 *
 * ### 互斥量(mutex: mutual exclusion)
 * 线程的主要优势在于能够共享全局变量. 不过, 必须要保证全局变量在同一时刻只能由一个线程修改,
 * 或者其他线程不会读取另一个线程正在修改的全局变量.<BR>
 * 临界区(critical section): 是指访问某一共享资源的代码片段, 并且这段代码的执行应为原子(atomic)性的操作,
 * 亦即, 同时访问同一个共享资源的其他线程不应该中断该片段的执行.
 *
 * 互斥量有两种状态: 已锁定(locked) 和 未锁定(unlocked). 任何时刻, 至多只有一个线程允许锁定一个互斥量.
 * 当试图对已经锁定的某一互斥量再次加锁, 将可能阻塞线程或者报错失败, 具体取决于加锁时使用的方法.<BR>
 * 一旦互斥量被锁定, 锁定互斥量的线程即成为该互斥量的所有者. 只有所有者才能给互斥量解锁.
 *
 * 互斥量有两种类型:
 *   - 静态互斥量: 经由静态分配且携带默认属性.在使用之前, 必须通过 @ref PTHREAD_MUTEX_INITIALIZER 进行初始化之后才可以使用.
 *   - 动态互斥量: 通过 malloc() 分配.
 *
 * 互斥量初始化操作只能应用于互斥量的真身, 不能对他们的 副本(copy) 进行初始化操作.
 *
 * @author Lock
 * @version V1.0
 * @date 2016-01-30
 */


/**
 * @brief 初始化静态分配的互斥量
 *
 * @code{.c}
 * pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
 * @endcode
 */
#define PTHREAD_MUTEX_INITIALIZER {0, 0, -1, -1, -1}

/**
 * @brief 创建线程
 *
 * <pthread.h>
 *
 * 创建新的线程之后, 新线程会从 @p start 所指定的函数开始执行.
 *
 * @param thread 指向 pthread_t 类型的缓冲区, 在 pthread_create() 返回前,
 * 会在此保存一个该线程的唯一标识. 后续的 Pthreads 函数将使用该表示来引用此线程.<BR>
 * SUSv3 规定, 在新线程开始执行之前, 实现无需对 @p thread 参数所指向的缓冲区进行初始化,
 * 即新线程可能会在 pthread_create() 函数返回之前就已经开始执行.
 * 如新线程需要获取自身的线程 DI, 则只能通过调用 pthread_self() 方法.
 * @param attr 指向 `pthread_attr_t` 对象的指针, 该对象指定了新线程的各种属性.
 * 如果该属性设为 NULL, 那么创建新线程时将使用各个默认属性.
 * @param start 执行一个返回类型为 `void *` 的函数指针, 新创建的线程会从该函数开始执行,
 * 当函数执行完毕, 该线程也将结束.
 * @param arg 为执行 @p start 函数时传递的参数. 一般情况下,
 * 该参数应当指向一个全局或堆变量, 也可以将其设置为 NULL.
 * 将参数类型设置为 `void`, 意味着可以将指向任意类型对象的指针传递给 @p start() 函数.
 * 如果要传递多个参数, 可以将 @p arg 指向一个结构, 结构中保存了要传递的多个参数.
 *
 * @return 返回函数的执行状态
 * @retval 0 函数执行成功
 */
int
pthread_create(pthread_t *thread, const pthread_attr_t *attr,
        void *(*start)(void *), void *arg);

/**
 * @brief 终止线程
 *
 * 终止线程的方法:
 *   - 线程 start 函数执行 return 语句并返回指定值
 *   - 线程调用 pthread_exit()
 *   - 调用 pthread_cancel() 取消线程
 *   - 任意线程调用了 exit(), 或者主线程(在 main() 函数中)执行了 return 语句,
 *   都会导致 <B>进程中的所有线程<B> 立即终止.
 *
 * 调用 pthread_exit() 相当于在 start 函数中执行 return, 不同之处是,
 * pthread_exit() 可以在线程 start 函数所调用的任意函数中调用 pthread_exit() 来结束该线程.
 *
 * 如果主线程中调用了 pthread_exit(), 而非执行了 exit() 或 return 语句,
 * 那么其他线程将会继续执行.
 *
 * @param retval 指定了线程的返回值. 该参数所指向的内容不应该被分配与线程栈中,
 * 因为当线程终止时, 将无法确定线程栈的内容是否有效. 出于同样道理,
 * 也不应在线程栈中分配线程 start 函数的返回值.
 */
void
pthread_exit(void *retval);

/**
 * @brief 获取线程 DI
 *
 * 进程内部的每个线程都有一个唯一标识符: 线程 DI. 线程 ID 会返回给 pthread_creat()
 * 的调用者, 一个线程可以通过 pthread_self() 来获取自己的线程 ID.
 *
 * 在所有的进程中, 线程ID也许并不是唯一的.
 *
 * @return
 */
pthread_t
pthread_self(void);

/**
 * @brief 用于检测两个线程ID是否相同
 *
 * 由于SUSv3并未对 pthread_t 类型的实现做任何定义, 每个操作系统的实现方式有可能不同,
 * 可能是一个整数, 或是一个指针, 或是一个结构体, 因此不能简单的使用 `==` 来判断两个线程ID 是否相同.
 *
 * @param t1 线程ID 1
 * @param t2 线程ID 2
 *
 * @return 返回比较结果
 * @retval 非0 要比较的线程ID相同
 * @retval 0 不同
 */
int
pthread_equal(pthread_t t1, pthread_t t2);

/**
 * @brief 连接已终止的线程
 *
 * 若线程并未分离(pthread_detach()), 则必须使用 pthread_join() 来进行连接.
 * 如果未能连接, 那么线程终止时将会产生 <B>僵尸线程<B>, 类似 僵尸进程 的概念.
 * 除了浪费资源以外, 僵尸线程若积累过多, 应用将再也无法创建新的线程.<BR>
 * pthread_join() 类似于 wait() 的功能, 但是又存在一些不同点:
 *   - 线程之间的关系是对等的. 进程中的任意线程均可以调用 pthread_join()
 *   与该进程的任何其他线程连接起来. 这与进程不同, 如果父进程创建了子进程,
 *   那么父进程是唯一能够对子进程调用 wait() 的进程.
 *   - 无法连接任意线程, 必须要指定一个线程, 也没有非阻模式的 pthread_join().
 *
 * 等待由 @p thread 标识的线程终止. 如果线程已经终止, 则 pthread_join() 会马上返回,
 * 否则会阻塞系统指定.
 *
 * 如果 pthread_join() 函数传入一个之前已经连接过的线程 ID, 将会导致无法预知的行为.
 *
 * @param thread 指定要等待终止的线程.
 * @param retval 若为非空指针时, 保存@p thread 线程返回时值的拷贝,
 * 该返回值即线程调用 `return` 或 `pthread_exit()` 时所指定的值.
 *
 * @return 返回函数执行状态
 * @retval 0 函数执行成功
 * @retval 大于0 函数执行失败
 */
int
pthread_join(pthread_t thread, void **retval);

/**
 * @brief 分离线程
 *
 * 有时并不关心线程退出的状态, 只是希望线程在终止时能够自动清理并移除之.
 * 这种情况下可调用 pthread_detach() 函数, 当执行了该函数后, 指定的线程在终止时将自动进行清理操作.
 * 该函数并不会阻塞系统的执行.
 *
 * 对执行了 pthread_detach() 函数的线程, 不能够在执行 pthread_join() 函数.
 *
 * @code{.c}
 * // 自动清理自身线程, 在结束后.
 * pthread_detach(pthread_self());
 * @endcode
 *
 * @param thread
 *
 * @return 函数执行状态
 * @retval 0 函数执行成功
 * @retval 大于0 函数执行失败
 */
int
pthread_detach(pthread_t thread);

/**
 * @brief 锁定互斥量
 *
 * 互斥量在通过 @r PTHREAD_MUTEX_INITIALIZER 或 pthread_mutex_init() 初始化之后处于未锁定状态,
 * 该函数用于锁定互斥量.
 *
 * 当锁定一个已经由其他线程锁定了的互斥量时, 该调用将会阻塞系统调用,
 * 直到这个互斥量被解锁为止.
 *
 * 如果线程尝试锁定一个已经被当前线程锁定的互斥量时, 可能发生的情况有:
 *   - 线程陷入死循环(deadlock), Linux 默认行为
 *   - 调用失败, 并设置 errno 为 `EDEADLK`
 *
 * @param mutex 要锁定的互斥量变量.
 *
 * @return 返回函数执行状态
 * @retval 0 函数执行成功.
 *
 * @see pthread_mutex_unlock()
 */
int
pthread_mutex_lock(pthread_mutex_t *mutex);

/**
 * @brief 解锁由自身线程锁定的互斥量
 *
 * 当要锁定的互斥量当前处于未锁定状态, 该调用将锁定互斥量, 并马上返回.
 *
 * 如果同时有多个线程正在等待由 pthread_mutex_unlock() 解锁的互斥量时,
 * 在互斥量被解锁之后, 无法判断哪个线程会先获取到这个互斥量.
 *
 * @param mutex
 *
 * @return 返回函数执行状态
 * @retval 0 函数执行成功.
 *
 * @see pthread_mutex_lock()
 */
int
pthread_mutex_unlock(pthread_mutex_t *mutex);

/**
 * @brief 动态初始化互斥量
 *
 * 必须使用函数 pthread_mutex_init() 函数初始化一个互斥量的情况为:
 *   - 动态分配与堆中的互斥量.
 *   - 互斥量是在栈中分配的自动变量.
 *   - 初始化经由静态分配, 且不使用默认属性的互斥量.
 *
 * SUSv3 规定, 初始化一个已经被初始化过的互斥量, 将会导致未定义行为,
 * 应当避免重复初始化互斥量.
 *
 * @param mutex 要初始化的目标互斥量.
 * @param attr pthread_mutexattr_t 类型对象, 该对象在函数调用之前经过了初始化处理, 用于定义互斥量的属性.
 * 若为 NULL, 则使用默认属性.
 *
 * @return 返回函数执行状态
 * @retval 0 函数执行成功.
 */
int
pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr);

/**
 * @brief 销毁动态互斥量
 *
 * 当一个动态互斥量不再使用时, 应当释放互斥量所占用的资源.
 *
 * 静态互斥量则无需释放.
 *
 * @param mutex 要释放的动态互斥量
 *
 * @return 返回函数执行状态
 * @retval 0 函数执行成功.
 */
int
pthread_mutex_destory(pthread_mutex_t *mutex);
