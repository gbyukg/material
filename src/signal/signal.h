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
 * @file signal.h
 * @brief 信号相关处理函数
 * @author Lock
 * @version V1.0
 * @date 2016-01-14
 */

#include <signal.h>

/**
 * @brief 信号集数据结构
 *
 * 用于保存一组信号信息
 *
 */
typedef struct {
    unsigned long sig[_NSIG_WORDS]; //!< 信号信息
} sigset_t;

/**
 * @brief siginfo
 */
siginfo_t {
    int      si_signo;     //!< Signal number
    int      si_errno;     //!< An errno value
    int      si_code;      //!< Signal code
    int      si_trapno;    //!< Trap number that caused
                           //!<  hardware-generated signal
                           //!<  (unused on most architectures)
    pid_t    si_pid;       //!< Sending process ID
    uid_t    si_uid;       //!< Real user ID of sending process
    int      si_status;    //!< Exit value or signal
    clock_t  si_utime;     //!< User time consumed
    clock_t  si_stime;     //!< System time consumed
    sigval_t si_value;     //!< Signal value
    int      si_int;       //!< POSIX.1b signal
    void    *si_ptr;       //!< POSIX.1b signal
    int      si_overrun;   //!< Timer overrun count;
                           //!<  POSIX.1b timers
    int      si_timerid;   //!< Timer ID; POSIX.1b timers
    void    *si_addr;      //!< Memory location which caused fault
    long     si_band;      //!< Band event (was int in
                           //!<  glibc 2.3.2 and earlier)
    int      si_fd;        //!< File descriptor
    short    si_addr_lsb;  //!< Least significant bit of address
                           //!<  (since Linux 2.6.32)
    void    *si_call_addr; //!< Address of system call instruction
                           //!<  (since Linux 3.5)
    int      si_syscall;   //!< Number of attempted system call
                           //!<  (since Linux 3.5)
    unsigned int si_arch;  //!< Architecture of attempted system call
                           //!<  (since Linux 3.5)
};

/**
 * @brief 用于 sigaction() 函数
 */
struct sigaction {
    union {
        void (*sa_handler)(int); //!< @ref sa_flags 没有设置 `SA_SIGINFO` 标志位时使用的信号处理函数.
        void (*sa_sigaction)(int, siginfo_t *, void *); //!< @ref sa_flags 中设置了 `SA_SIGINFO` 标志位时使用的信号处理函数.
    } __sigaction_handler;      //!< 处理函数地址, 或是 `SIG_IGN` 和 `SIG_DFL` 之一.
    sigset_t sa_mask;           //!< 执行处理函数时需要被暂时阻塞的信号.
                                //!< 当调用信号处理程序时, 会在调用信号处理器之前,
                                //!< 将该组信号中当前未处于进程掩码之列的任何信号自动添加到进程掩码中.
                                //!< 这些信号将保留在信号掩码中,直至信号处理器返回返回.
                                //!< 届时将自动从信号掩码中删除这些信号.<BR>
                                //!< 此外, 对引发处理程序调用的信号也将自动添加到信号掩码中.
                                //!< 这意味着, 当正在执行处理程序时, 如果同一信号第二次抵达,
                                //!< 信号处理程序将不会递归中断自己. 由于不会对遭阻塞的信号进行排队处理,
                                //!< 如果在处理器程序执行过程中重复产生这些信号的任何信号, 稍后对信号的传递是一次性的.
    int sa_flags;               //!< 位掩码, 指定用于控制信号处理过程中的各种选项, 可选择有:
                                //!<    - `SA_NOCLDSTOP`: 若 sig 为 `SIGCHLD` 信号, 则当因接受因信号而停止或回复某一子进程时, 将不会产生此信号.
                                //!<    - `SA_NOCLDWAIT`: 若 sig 为 `SIGCHLD` 信号, 当子进程终止时, 不会将其转化为僵尸进程. 该标志位的设置等同于对 SIGCHLD 的处置置为 `SIG_IGN`.<BR>
                                //!<    同样, 如果对忽略 `SIGCHLD` 信号, 即信号处理函数设置为 `SIG_IGN`, 子进程结束后系统会直接将其删除,
                                //!<    而不会成为僵尸进程. 这时会将子进程的状态弃之不问, 故而后续的 wait() 系列函数不会返回子进程的任何信息.
                                //!<    - `SA_NODEFER`: 捕获该信号时, 不会将该信号自动添加到进程掩码中去.
                                //!<    - `SA_ONSTACK`: 针对此信号带哦用处理器函数时, 使用了由 sigaltstack() 安装的备选栈.
                                //!<    因为调用 execle() 系列函数会清空原始程序的数据段, 堆栈信息, 程序注册的备选栈同样会被清除, 所以在新的程序中, 该标志位会被清除.
                                //!<    - `SA_RESETHAND`: 当捕获该信号时, 会在调用处理器函数之前就爱那个信号处置重置为默认值(即 `SIG_DFL`).
                                //!<    - `SA_RESTART`: 自动重启由信号处理器程序中断的系统调用.
                                //!<    当发起一个阻塞系统调用后阻塞了系统执行(如 read()) 时, 该进程接受到了一个已经为其创建了信号处理函数的信号时,
                                //!<    当信号处理函数执行完成后, 系统默认的行为是系统阻塞函数 (read()) 调用失败, 并设置 `errno` 为 `EINTR`.
                                //!<    在为信号设置信号处理函数时, 使用该标志位可阻止这种情况, 即系统函数不会调用失败, 而是继续阻塞系统调用.
                                //!<    - `SA_SIGINFO`: 调用信号处理器程序时携带了额外参数, 其中提供了关于信号的深入信息.
    void (*sa_restorer)(void);  //!< 未使用

#define sa_handler __sigaction_handler.sa_handler       //!< sigaction.sa_handler
#define sa_sigaction __sigaction_handler.sa_sigaction   //!< sigaction.sa_sigaction
};

/**
 * @brief 用于设置信号备选栈信息
 *
 * @see sigaltstack()
 */
typedef struct {
    void   *ss_sp;      //!< 信号处理函数的起始地址, 在实际使用信号栈时, 内核对将 ss_sp 值自动对齐与硬件架构相适宜的地址边界
    int    ss_flags;    //!< 标志位, 可选值有:
                        //!<   - `SS_ONSTACK`: 当在获取备选栈信息时, 在返回的信息中该标志位如果已经被设置,
                        //!<   则说明进程正在备选信号栈上执行. 当进程已经在备选栈运行时,
                        //!<   试图调用 sigaltstack() 来创建一个新的备选信号栈将会阐释一个错误(EPERM)
                        //!<   - `SS_DISABLE`: 在 old_sigstack 中返回, 比奥斯当前不存在已创建的备选信号栈.
                        //!<   如果在 sigstack 中指定, 则会禁用当前已创建的备选信号栈.
    size_t ss_size;     //!< 备选栈大小
} stack_t;

/**
 * @brief 改变信号处置
 *
 * 该函数较 sigaction() 简单, 但是对于跨平台程序, 觉不可使用该函数.
 *
 * @param sig 要处置的信号
 * @param handler 信号抵达时, 要调用的信号处置函数, 该函数无返回值,
 * 并接收一个整形参数, 用于保存接收到的信号. 除了可以指定函数地址外, 还可以指定:
 *   - `SIG_DFL`: 将信号处置重置为系统默认行为
 *   - `SIG_IGN`: 忽略信号
 *
 * @code{.c}
 * // handler 参数原型
 * void
 * handler(int sig)
 * {
 *      // Code for the handler
 * }
 * @endcode
 *
 * @return 函数执行成功, 则返回 @p sig 信号之前的处理函数地址,
 * 也可能是 `SIG_DFL` 或者 `SIG_IGN`.
 * @retval SIG_ERR 函数执行失败.
 */
void
( *singal(int sig, void (*handler)(int)) )(int);

/**
 * @brief 手动发送信号到某个或某些进程
 *
 * 通过 kill() 函数, 一个进程可以向另一个进程 @p pid 发送一个指定的信号 @p sig.
 * 之所以叫 kill, 是因为早期 UNIX 实现中大多数信号的默认行为是终止进程.
 *
 * kill() 会将信号传递给某个进程下的任意一个线程中.
 *
 * 一个进程要发送信号到另一个进程, 需要适当权限才可以, 其规则:
 *   - 特权级进程可以向任何进程发送信号.
 *   - 以 root 用户和组运行的 `init` 进程(进程号1), 是一种特例,
 *   仅能接受已安装了处理器函数的信号. 这可以防止系统管理员意外杀死 `init` 进程.
 *   - `SIGCONT` 信号需要特殊处理. 无论对用户 ID 的检查结果如何,
 *   非特权进程可以向同一会话中的任何其他进程发送这一信号. 利用这一规则,
 *   运行作业控制的 shell 可以重启已停止的作业, 即使作业进程已经修改了他们的用户 ID.
 *
 * @param pid 用于标示一个或多个目标进程, 可以有以下几种解释:
 *   - `pid 大于 0`: pid 为进程号
 *   - `pid 等于 0`: 信号将发送给与调用进程所在的进程组中的所有进程,
 *   包括调用进程自身.
 *   - `pid 小于 -1`: pid 的绝对值最为进程组ID, 信号将发送给该进程组中的所有进程.
 *   - `pid 等于 -1`: 发送范围为: 调用进程有权将信号发往的每个目标进程,
 *   但是不包括 init(1) 进程和自身进程. 如果调用进程为特权级进程, 那么将发给系统中的所有进程.
 *   这种信号发送方式有时也称为 <B>广播信号</B>
 * @param sig 要发送的信号
 *
 * @return 返回函数执行状态
 * @retval 0 函数执行成功
 * @retval -1 函数执行失败. errno 的值:
 *   - `EPERM`: 进程无权向指定的进程发送信号.
 *   若 @p pid 指定了一组进程, 那么只要可以向其中一个进程发送信号, 函数则执行成功.
 *   - `ESRCH`: 指定的进程不存在
 *
 * @see raise()
 * @see killpg()
 */
int
kill(pid_t pid, int sig);

/**
 * @brief 向进程自身发送信号
 *
 * 当进程调用 raise() 向自身发送信号时, 信号将立即传递, 即, 在 raise() 返回调用者之前.
 *
 * @code{.c}
 * // 对于单线程来说, 相当于
 * kill(getpid(), sig);
 *
 * // 对于支持线程的系统, 会将 raise() 实现为:
 * pthread_kill(pthread_self(), sig);
 * @endcode
 *
 * @param sig 要发送的信号
 *
 * @return 返回函数执行状态
 * @retval 0 函数执行成功
 * @retval 非0 函数执行失败, 调用该函数唯一可能发生的错误为 `EINVAL`,
 * 即 @p sig 无效.
 *
 * @see kill()
 * @see killpg()
 */
int
raise(int sig);

/**
 * @brief 向指定进程组中的所有成员发送信号
 *
 * @code{.c}
 * // 等同于
 * kill(-pgrp, sig);
 * @endcode
 *
 * @param pgrp 指定的进程组
 * @param sig 要发送的信号
 *
 * @return 返回函数执行状态
 * @retval 0 函数执行成功
 * @retval -1 函数执行失败
 *
 * @see kill()
 * @see raise()
 */
int
killpg(pid_t pgrp, int sig);

/**
 * @brief 显示信号描述
 *
 * 每个信号都有一串与之相关的可打印说明. 这些描述符位于数组 `sys_siglist` 中.
 * 例如, 可以用 `sys_siglist[SIGPIPE]` 来获取对 `SIGPIPE` 信号的描述.
 * @code{.c}
 * extern const char *const sys_siglist[];
 * @endcode
 *
 * 与之使用 `sys_siglist` 数组相比, 该函数对 @p sig 参数进行边界检查,
 * 然后返回一枚指针, 指向针对该信号的可打印描述符字符串, 或者是当信号无效时,
 * 指向错误字符串.
 *
 * strsignal() 对本地设置敏感, 所以显示信号描述时会使用本地语言.
 *
 * @param sig 指定要显示的信号
 *
 * @return 返回描述字符串的指针
 */
char *
strsignal(int sig);

/**
 * @brief 初始化一个未包含任何成员的信号集
 *
 * @param set 要初始化的信号集
 *
 * @return 返回函数执行状态
 * @retval 0 函数执行成功
 * @retval -1 函数执行失败
 *
 * @see sigfillset()
 * @see sigaddset()
 * @see sigdelset()
 * @see sigismember()
 *
 * @note 在使用信号集之前, 必须要使用 sigempty() 或 sigfillset() 函数对信号集进行初始化操作.
 */
int
sigemptyset(sigset_t *set);

/**
 * @brief 初始化一个包含了所有信号的信号集
 *
 * @param set 要初始化的信号集
 *
 * @return 返回函数执行状态
 * @retval 0 函数执行成功
 * @retval -1 函数执行失败
 *
 * @see sigfillset()
 * @see sigaddset()
 * @see sigdelset()
 * @see sigismember()
 *
 * @note 在使用信号集之前, 必须要使用 sigempty() 或 sigfillset() 函数对信号集进行初始化操作.
 */
int
sigfillset(sigset_t *set);

/**
 * @brief 向一个通过 sigemptyset() 或 sigfillset() 初始化过的信号集中增加信号.
 *
 * @param set 信号集
 * @param sig 要增加的信号
 *
 * @return 返回函数执行状态
 * @retval 0 函数执行成功
 * @retval -1 函数执行失败
 *
 * @see sigemptyset()
 * @see sigfillset()
 * @see sigdelset()
 * @see sigismember()
 */
int
sigaddset(sigset_t *set, int sig);

/**
 * @brief 向一个通过 sigemptyset() 或 sigfillset() 初始化过的信号集中删除指定信号.
 *
 * @param set 信号集
 * @param sig 要删除的信号
 *
 * @return 返回函数执行状态
 * @retval 0 函数执行成功
 * @retval -1 函数执行失败
 *
 * @see sigemptyset()
 * @see sigfillset()
 * @see sigaddset()
 * @see sigismember()
 */
int
sigdelset(sigset_t *set, int sig);

/**
 * @brief 测试给定的信号是否存在于指定的信号集中
 *
 * @param set 信号集
 * @param sig 要测试的信号
 *
 * @return 返回测试状态
 * @retval 1 要测试的信号 @p sig 存在于信号集 @p set 中.
 * @retval 0 不存在
 *
 * @see sigemptyset()
 * @see sigfillset()
 * @see sigaddset()
 * @see sigdelset()
 */
int
sigismember(const sigset_t *set, int sig);

/**
 * @brief 修改进程的信号掩码
 *
 * 通过调用 sigprocmask() 函数, 可以随时向进程的信号掩码中增加或移除信号.
 *
 * SUSv3 规定, 如果有任何等待信号因对 sigprocmask() 的调用而解除了锁定,
 * 那么在调用此函数返回前至少会传递一个信号. 换言之, 如果解除了对某个等待信号的锁定,
 * 那么会立刻将该信号传递给进程.
 *
 * 系统调用将忽略试图阻塞 `SIGKILL` 和 `SIGSTOP` 信号的请求.
 * 如果试图阻塞这些信号, sigprocmask() 函数既不会予以关注, 也不会产生错误.
 *
 * 继承关系:
 *
 * fork() | exec | 线程
 * ------ | ---- | ----
 * 是     | 是   | 否
 *
 * @param how 指定了修改进程的信号掩码方式:
 *   - `SIG_BLOCK`: 将 @p set 指向的信号集内的所有信号添加到当前的信号掩码中.
 *   - `SIG_UNBLOCK`: 将 @p set 指向的信号集内的所有信号从当前信号掩码中移除.
 *   即使要解除的信号当前并未处于阻塞状态, 也不会返回错误.
 *   - `SIG_SETMASK`: 将 @p set 指向的信号集设置为当前的信号掩码.
 * @param set 要设置的信号集
 * @param oldset 若不为 NULL, 则其指向一个 @ref sigset_t 结构缓冲区,
 * 用于返回之前的信号掩码.
 *
 * @return 返回函数执行状态
 * @retval 0 函数执行成功
 * @retval -1 函数执行失败
 */
int
sigprocmask(int how, const sigset_t *set, sigset_t *oldset);

/**
 * @brief 获取当前进程中正处于阻塞的信号集
 *
 * 如果修改了对等待信号的处置, 那么当后来解除对信号的锁定时, 将根据新的处置来处理信号.
 *
 * @param sig 用于保存获取到的信号集, 在通过调用 sigismember()
 * 来确定某个信号是否正处于阻塞状态.
 *
 * @return 返回函数执行状态
 * @retval 0 函数执行成功
 * @retval -1 函数执行失败
 */
int
sigpending(sigset_t *sig);

/**
 * @brief 设置信号处理函数
 *
 * @param sig 想要获取或设置的信号编号. 该参数可以是除去 `SIGKILL` 和 `SIGSTOP`
 * 之外的所有信号.
 * @param act 一个指向 @ref sigaction 结构体的指针, 用于描述如何处置信号 @p sig.
 * 如果仅对现有的信号处置感兴趣, 可将该参数设置为 NULL.
 * @param oldact 获取之前的信号处置信息, 可以设置为 NULL.
 *
 * @return 返回函数执行状态
 * @retval 0 函数执行成功
 * @retval -1 函数执行失败
 *
 * @see signal()
 */
int
sigaction(int sig, const struct sigaction *act, struct sigaction *oldact);

/**
 * @brief 等待信号
 *
 * 该函数将暂停进程的执行, 直至信号处理器函数中断该函数调用为止
 * (或者直至一个未处理信号终止进程为止).
 *
 * @return 总是返回 -1, 并将 `errno` 设置为 `EINTR`.
 */
int
pause(void);

/**
 * @brief 异常终止进程
 *
 * 函数 abort() 通过产生 `SIGABRT` 信号来终止调用进程.
 * 对 SIGABRT 的默认动作是产生核心转储文件并终止进程.
 *
 * SUSv3 要求, 无论阻塞和忽略 `SIGABRT` 信号, abort() 函数的调用均不受影响.
 */
void
abort(void);

/**
 * @brief 将信号处理函数设置在备选栈中.
 *
 * 需要指定 @ref sigaction 中的 `SA_ONSTACK` 标志位.
 *
 * @param sigstack 指向一个 @ref stack_t 类型的结构指针,
 * 用于设置新备选信号栈位置及属性信息. 当 @p old_sigstack 不为 NULL 时,
 * 该参数可以设置为 NULL.
 * @param old_sigstack 若不为 NULL, 则返回上一备选栈信息.
 *
 * @return 返回函数执行状态
 * @retval 0 函数执行成功
 * @retval -1 函数执行失败
 */
int
sigaltstack(const stack_t sigstack, stack_t *old_sigstack);

/**
 * @brief 修改信号的 SA_RESTART 标志
 *
 * @param sig 要修改的信号
 * @param flag 设置的标志位:
 *   - `1`: 中断阻塞的系统调用的执行
 *   - '0': 自动重启阻塞的系统调用
 *
 * @return 返回函数执行状态
 * @retval 0 函数执行成功
 * @retval -1 函数执行失败
 *
 * @see sigaction()
 *
 * @note SUSv4 标记 siginterrupt() 为已废止, 并推荐使用 sigaction() 作为替代.
 */
int
siginterrupt(int sig, int flag);

/**
 * @brief 使用掩码来等待信号
 *
 * 将以 @p mask 所执行的信号集来替换进程的信号掩码, 然后挂起进程的执行,
 * 直到捕获到信号, 并从信号处理器中返回. 一旦处理器返回, sigsuspend()
 * 会将进程信号掩码恢复为调用前的值. 相当于
 * @code{.c}
 * sigprocmask(SIG_SETMASK, &mask, &prevMask);
 * pause()
 * sigprocmask(SIG_SETMASK, &prevMask, NULL);
 * @endcode
 * 但 sigsuspend() 是一个原子操作.
 *
 * @param mask 要阻塞的信号集
 *
 * @return 返回 -1, error 的值:
 *   - `EINTR`: 被信号中断
 *   - `EFAULT`: @p mask 指向了无效的地址
 */
int
sigsuspend(const sigset_t *mask);
