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
 * @file header.h
 * @brief 系统进程相关函数
 * @author Lock
 * @version V1.0
 * @date 2016-01-09
 */

#include <unistd.h>

/**
 * @brief 获取当前进程 ID
 *
 * <unistd.h>
 *
 * 该函数返回的数据类型为 `pid_t`, 一般定义成整型,
 * 专门用于存储进程 ID 号.
 *
 * 每创建一个新进程, 内核会按顺序将下一个可用的进程号分配给其使用,
 * 当进程号达到系统最大限度时(如: Linux 32767), 内核将重置进程号计数器,
 * 以便从小整数开始分配, 但是在重置进程号计数器后,
 * 是从一个指定的整数开始搜索, 而不是从 0 开始,
 * 因为低位整数被系统进程所占用, 直接跳过这些整数.
 *
 * Linux 下可以通过修改 `/proc/sys/kernel/pid_max` 文件来调整内核最大进程号.
 *
 * @return 返回进程 ID 号
 *
 * @note 该函数永远会执行成功
 *
 * @see getppid();
 */
pid_t
getpid(void);

/**
 * @brief 获取父进程 ID 号
 *
 * <unistd.h>
 *
 * 该函数用于返回当前进程的父进程的 ID 号.
 *
 * 所有进程一直到用 getppid(), 最终都将得到进程号为 1 的根进程.
 *
 * @return 返回 pid_t 类型的进程号, 同 getpid() 一样,
 * 该函数用于返回成功.
 *
 * @see getpid()
 */
pid_t
getppid(void);

/**
 * @brief 获取环境变量
 *
 * <stdlib.h>
 *
 * 获取指定 @p name 的环境变量的值
 *
 * @param name 指定要获取的环境变量的名称
 *
 * @return 返回一个字符串指针
 * @retval NULL 无指定的环境变量
 *
 * @note SUSv3 规定, 应用程序不应该修改 getenv() 函数返回的字符串,
 * 这是由于该字符串实际上属于环境变量的一部分(即 name=value 字符串的 value 部分).
 * @note SUSv3 允许 getenv() 函数的实现使用静态分配的缓冲区返回执行结果.
 * 后续对 getenv(), setenv(), putenv() 或者 unsetenv() 的函数调用可以重写该缓冲区.
 * 如果想要保留获取到的环境变量, 应先将返回的字符串复制到其他位置,
 * 之后方可对上述函数发起调用.
 *
 * @see putenv()
 * @see setenv()
 * @see unsetenv()
 * @see clearenv()
 */
char *
getenv(const char *name);

/**
 * @brief 设置环境变量
 *
 * 该函数会将将 @p string 直接设置为环境变量的一部分, 因此在执行完该函数后如果对该字符串内容进行修改,
 * 将会影响到后续该环境变量的值.<BR>
 * 也正是如此, 应该将 @p string 参数设置为静态变量或者全局变量, 保证该字符串不会被消除.
 *
 * @param string 指向 `name=value` 形式的字符串.
 *
 * @return 返回函数执行状态
 * @retval 0 函数执行成功
 * @retval 非0 函数执行失败
 *
 * @code{.c}
 * // 设置环境变量 NAME=ZZL;
 * static char *setName = "NAME=ZZL";
 * putenv(setName);
 * @endcode
 *
 * @see getenv()
 * @see putenv()
 * @see setenv()
 * @see unsetenv()
 * @see clearenv()
 */
int
putenv(char *string);

/**
 * @brief 设置环境变量
 *
 * <stdlib.h>
 *
 * 不同于 putenv() 函数, setenv() 在执行时会自动复制其参数 @p name 和 @p value
 * 的值, 因此后续对这两个参数的任何改动都不会影响到环境变量 @p name 的值.
 *
 * @param name 要设置的环境变量名称
 * @param value 新环境变量对应的值
 * @param overwrite 用于决定是否重写已经存在的环境变量的值:
 *   - 非0 总是将环境变量 @p name 的值设置为 @p value
 *   - 0 如果环境变量 @p name 已经存在, 则不做任何操作, 保留原环境变量 @p name 的值.
 *
 * @return 返回函数状态执行结果
 * @retval 0 函数执行成功
 * @retval -1 函数执行失败
 *
 * @code{.c}
 * // 设置环境变量 NAME=ZZL
 * setenv("NAME", "ZZL", 1);
 * @endcode
 *
 * @see getenv()
 * @see putenv()
 * @see setenv()
 * @see unsetenv()
 * @see clearenv()
 */
int
setenv(const char *name, const char *value, int overwrite);

/**
 * @brief 移除环境变量
 *
 * <stdlib.h>
 *
 * 从系统环境变量中移除 @p name 环境变量.
 *
 * @param name 指定要移除的环境变量名称.
 *
 * @return 返回函数执行的结果
 * @retval 0 函数执行成功
 * @retval -1 函数执行失败
 *
 * @see getenv()
 * @see putenv()
 * @see setenv()
 * @see unsetenv()
 * @see clearenv()
 */
int
unsetenv(const char *name);

/**
 * @brief 清除所有环境变量
 *
 * #define _BSD_SOURCE
 * <stdlib.h>
 *
 * 该函数实际就是将全局变量 environ 变量赋值为 NULL.
 *
 * @return 返回函数执行的结果
 * @retval 0 函数执行成功
 * @retval 非0 函数执行失败
 *
 * @see getenv()
 * @see putenv()
 * @see setenv()
 * @see unsetenv()
 * @see clearenv()
 */
int
clearenv(void);

/**
 * @brief 创建新进程
 *
 * 当创建完一个子进程后, 父进程与子进程都会从 fork() 的返回处继续执行.
 *
 * 子进程执行与父进程相同的代码段, 即共享父进程的代码段, 但是有自己独立的栈段,
 * 数据段以及堆的拷贝. 子进程的栈, 数据以及栈段开始时是对父进程内存相应个部分的完全复制.
 *
 * 在子进程创建完成后, 系统会先执行父进程程序还是子进程程序是完全无法预测的.
 *
 * 子进程继承自父进程的属性:
 *   - 父进程中所有打开的文件描述符的副本, 这些副本的创建方式类似于 dup(),
 *   这也意味着父子进程将共享文件的偏移量以及文件状态标志.
 *   - 父进程中注册的退出处理程序, 参见 atexit() 函数.
 *
 * 当子进程结束后, 会转化为僵尸进程, 内核会为停止的子进程保留一些最基本的信息,
 * 如 PID, 终止状态, 资源使用数据等信息. 这些信息会在父进程调用了 wait() 系列函数后释放掉.
 * 这些资源势必会占用系统资源, 如果执行长时间程序, 并产生了大量的子进程,
 * 如果不调用 wait() 系列函数捕获停止的子进程, 有可能会导致资源被占满.<BR>
 * 即使是 `SIGKILL` 也无法杀死僵尸进程.
 *
 *
 * @return 根据返回值确定是在父进程中还是子进程中:
 *   - 父进程中, 返回新创建的子进程的 进程ID
 *   - 子进程中, 返回0
 * @retval -1 创建子进程失败, 失败的原因可能是:
 *   - 进程数量超出了当前真实用户可创建的最大进程数的限制.
 *   - 系统进程总数达到了最大限制.
 */
pid_t
fork(void);

/**
 * @brief 退出进程
 *
 * <unistd.h>
 *
 * 退出进程的执行, 并返回给父进程一个标识码, 用于标识进程执行的状态.
 * 一般 0 用于表示执行成功, 非0 表示执行失败.
 *
 * 虽然可将 0~255 之间的任意整数付给 @p status, 但是当将大于 128 的值作为退出状态容易引发混乱,
 * 因为如果进程是被信号终止, 那么退出状态值则是 128+信号编号 的值.
 *
 * 调用 _exit() 的程序总是会成功终止, 因此该函数无返回值.
 *
 * @param status 标志了进程的终止状态, 虽然该参数是 int 类型,
 * 但是仅低 8 位可供父进程使用.
 *
 * @see exit()
 */
void
_exit(int status);

/**
 * @brief 退出进程的库函数
 *
 * <stdlib.h>
 *
 * exit() 与 _Exit() 函数存在于 <stdlib.h> 头文件中, 而 _exit() 则存在于 <unistd.h> 头文件中,
 * 之所以存储在不同的头文件中, 是因为 exit() 与 _Exit() 是由 ISO C 说明的,
 * _exit() 是由 POSIX.1 说明的.
 *
 * 该函数是对 _exit() 函数的封装, 执行流程如下:
 *   - 调用退出处理程序(通过 atexit() 和 on_exit() 注册的函数)
 *   - 循环调用 fclose() 关闭所有打开的流文件, 刷新 `stdio` 流缓冲区
 *   - 使用由 @p status 提供的值执行 _exit() 系统调用
 *
 * @param status 指定的程序退出状态码
 *
 * @note 该函数属于非异步信号安全函数, 因为该函数会刷新 stdio 流缓冲区.
 *
 * @see eixt()
 * @see atexit()
 */
void
exit(int status);

/**
 * @brief 注册退出处理程序
 *
 * 该函数会将 @p func 添加到一个函数列表中, 进程终止时会调用该函数列表的所有函数.
 *
 * 可以注册多个退出处理函数(甚至将同一函数注册多次). 当调用 exit() 时,
 * 这些注册的函数的执行顺序与注册顺序相反.
 *
 * 在调用退出处理函数时, 一旦某个处理函数无法返回(无论是调用了 _exit() 还是进程接收到了信号而终止),
 * 那么就不会在调用剩余的处理程序.
 *
 * 在某些系统下, 如果处理程序中调用了 exit() 函数, 则有可能导致死循环的发生,
 * 因此处理程序中应当禁止使用 exit() 函数.
 *
 * SUSv3 规定, 要求系统实现应允许一个进程能够注册至少 32 个退出处理程序.
 * 可通过 `sysconf(_SC_ATEXIT_MAX)` 获取当前系统允许注册退出程序的最大个数.
 *
 * 子进程会继承父进程注册的退出程序, 而进程调用 exec() 时,
 * 会移除所有已注册的退出处理函数.
 *
 * 退出处理程序一经注册, 则无法取消其执行, 可以通过设置全局变量来禁止某个退出处理程序的执行.
 *
 * @param func 程序退出时要执行的函数地址
 *
 * @return 返回函数执行状态
 * @retval 0 函数执行成功
 * @retval 非0 函数执行失败
 */
int
atexit(void (*func)(void));

/**
 * @brief 等待子进程
 *
 * <sys/wait.h>
 *
 * 该函数用于获取子进程结束状态信息, 当执行该函数时, 如果当前没有子进程结束执行,
 * 则阻塞系统调用, 直到某个子进程终止.
 * 如果在运行 wait() 函数之前已经有子进程终止, 则 wati() 马上返回.
 * 内核将会为父进程下所有子进程的运行总量追加到进程 CPU 时间, 参照 times() 函数.
 *
 * 调用 wait(), waitpid() 或 waitid() 等函数之后, 同时会释放掉子进程用于让父进程捕获的一些信息,
 * 如果子进程结束后, 父进程没有调用该系列函数, 则该子进程将会成为僵尸进程.
 *
 * @param status 若该参数不为 NULL, 则子进程如何终止的信息会通过该参数指向的整形变量返回.
 *
 * @return 函数执行的状态, 若函数执行成功, 返回捕获到的子进程的 进程ID
 * @retval -1 函数执行失败, 失败的原因可能是进程中已经无子进程在执行,
 * 此时 errno 会被设置为 `ECHILD`
 *
 * @code{.c}
 * // 等待所有子进程结束
 * while ((childPid = wait(NULL)) != -1)
 *     continue;
 * if (errno != ECHILD)
 *     exit(1);
 * @endcode
 */
pid_t
wait(int *status);

/**
 * @brief 等待子进程结束
 *
 * 相对于 wait() 函数, 该函数更具有优势
 *   - 可以通过子进程的 进程ID 来指定要等待的子进程
 *   - 即使没有子进程终止, 也可以不阻塞系统的运行
 *   - 可以捕获到子进程因某个信号(如 SIGSTOP 或 SIGTTIN)而停止,
 *   或是已经停止子进程收到 SIGCONT 信号后恢复执行的情况.
 *
 * @param pid 可有多中不同选项:
 *   - @p pid 大于 0: 表示等待进程 ID 为 @p pid 的子进程.
 *   - @p pid 等于 0: 等待与调用进程同一进程组的所有子进程.
 *   - @p pid 小于 -1: 等待进程组标示符与 @p pid 绝对值相等的所有子进程
 *   - @p pid 等于 -1: 等待任意子进程.
 * @param status 参考 wait()
 * @param options 位掩码, 可以包含0个或多个如下标志
 *   - `WUNTRACED`: 除了返回终止子进程的信息外, 还返回因信号而停止的子进程信息
 *   - `WCONTINUED`: 返回那些因收到 SIGCONT 信号而恢复执行的已停止子进程的状态信息
 *   - `WNOHANG`: 如果 @p pid 指定的子进程并未发生状态改变, 则立即返回, 而不会阻塞.
 *   这种情况下, waitpid() 返回0. 如果调用进程并无与 @p pid 匹配的子进程,
 *   则 waitpid() 报错, 并将 errno 设置为 ECHILD.
 *
 * @return 返回捕获到的子进程ID
 */
pid_t
waitpid(pid_t pid, int *status, int options);

/**
* @brief 与 waitpid() 作用一样, 但是提供了更多的选项用于等待不同状态的子进程通过
* @p options 参数.
*
* @param idtype 与 @p id 结合使用, 用来确定监控那些子进程, 可用值:
*   - `P_ALL`: 等待任意子进程, 此时将忽略 @p id 参数
*   - `P_PID`: 等待进程 ID 为 @p id 的子进程
*   - `P_PGID`: 等待进程组 ID 为 @p id 下的任意子进程
* @param id 与 @p idtype 结合使用
* @param infop
* @param options 通过 RO 操作, 指定一个或多个下列值, 来获取不同状态的子进程:
*   - `WEXITED`: 获取所有终止的进程, 包括正常终止以及因信号中断的子进程.
*   - `WSTOPPED`: 捕获因信号而停止的进程.
*   - `WCONTINUED`: 捕获因 SIGCONT 信号而重新运行的子进程.
*   - `WNOHANG`: 与 waitpid() 功能一样, 如果设定了该选项, 则函数调用立即返回,
*   而不会阻塞系统, 此时函数调用返回 0, 如果调用进程并无进程号为 @p id 的子进程,
*   函数调用返回失败, 并将 errno 设置为 `ECHILD`.<BR>
*   由于 waitid() 执行成功后返回 0, 而并非捕获到的子进程ID, 当指定该选项时,
*   即使没有子进程结束, waitid() 同样会返回 0, 这样讲无法得知子进程是否已经结束.
*   此时通过判断 @p infop 参数是否为空来判断子进程是否已经结束, 如果结束,
*   则会将相关信息填充到 @p infop 结构中, 如果子进程没有结束, 则不会填充该结构体,
*   因此可以通过判断 @p infop 是否为空来判断子进程是否已经结束. 有些系统实现了如果子进程没有结束,
*   则自动将 @p infop 置为 NULL, 但有些系统并没有实现该功能, 因此, 为了可移植性,
*   在调用 waitid() 之前, 应当手动将 @p infop 置空.
*   - `WNOWAIT`: 当一个子进程的结束被 wait() 系列函数捕获到后, 系统就会释放掉子进程保留的一些最基本的信息,
*   如果指定了该选项, 子进程状态会被返回, 但不会释放掉子进程相关信息,
*   之后仍然可以使用 wait() 系列函数获取到该子进程.
*
* @return 返回函数执行状态
* @retval 0 函数执行成功, 与 waitpid() 不同, 并不返回获取到的子进程 ID
* @retval -1 函数执行失败. 如果 @p id 指定的子进程不存在, errno 将被设置为 `ECHILD`.
*
* @see wait()
* @see waitpid()
* @see http://man7.org/linux/man-pages/man2/waitid.2.html
*/
int
waitid(idtype_t idtype, id_t id, siginfo_t *infop, int options);

/**
 * @brief 执行新程序
 *
 * exec 系列函数执行新程序, 该函数的调用可将新程序加载到当前执行进程的内存空间.
 * 在这一操作过程中, 将丢弃旧有的程序, 而进程的栈, 数据以及堆段会被新程序的相应部件所替换.
 *
 * 调用 execve() 之后, 因为同一进程仍然存在, 所有 进程ID 并未发生改变.
 *
 * 由于是对调用程序取而代之, 对 exec 系列函数的调用永远不会返回,
 * 而且也无需检查 exec 系列函数的返回值, 因为该值总是雷打不动地等于 -1.
 * 实际上, 一旦函数返回, 就表明发生了错误.
 *
 * exec 系列函数中,
 *   - `p` 代表允许只提供路径名, 而不用提供要执行文件的完整路径,
 *   此时系统会在环境变量 PATH 设置的目录下搜索 @p pathname 文件.
 *   如果文件名中包含 `/`, 则将其试为绝对或相对路径, 此时将不再使用 PATH 系统环境变量.
 *   - `l` 代表以字符串列表形式来指定参数, 而不使用数组来描述 `argv` 列表.
 *   首个参数对应于新程序 main() 函数的 `argv[0]` 参数, 因而通常与参数 @p filename
 *   或 @p pathname 的 basename 部分相同. 必须以 NULL((char *)NULL) 指针来终止参数列表.
 *   - `e` 代表允许开发者通过 envp 为新程序显示指定环境变量, 其中 envp 是一个以 NULL 结束的字符串指针数组.
 *   其他 exec 系列函数将使用调用者当前的环境变量作为新程序的环境.
 *
 * exec 系列函数执行时, 会将现有进程的文本段丢弃. 该文本段可能包含了由调用进程创建的信号处理程序.
 * 既然处理器函数已经丢失, 内核就将对所有已有信号的处置重置为 SIG_DFL.
 * 而对所有其他信号(即将处置为 SIG_IGN 或 SIG_DFL 的信号)的处置则保持不变.
 * 这也符合了 SUSv3 的要求.<BR>
 * 对于在执行 exec 系列函数之前,若忽略了 `SIGCHLD` 信号, 有些系统会在新程序中继续忽略该信号,
 * 有些系统则会设置为 `SIG_DFL`, 而 SUSv3 并没有对此说明, 为了程序的一致性,
 * 在调用 exec 系列函数, 应将 `SIGCHLD` 处理函数设置为 `SIG_DFL`. 此外,
 * 程序也不应当假设对 `SIGCHLD` 处理的初始设置是 `SIG_DFL` 之外的其他值.<BR>
 * 在调用 execve() 期间, 进程信号掩码以及挂起的(pending)信号的设置均得以保存.
 * 这一特性将允许对新程序的信号进行阻塞和排队处理.
 *
 * @param pathname
 * @param argv[]
 * @param envp[]
 *
 * @return 若函数执行成功, 则没有返回值.
 * @retval -1 函数执行失败, 当出错时候, 可通过 errno 获取错误原因, errno 可能的值有:
 *   - `EACCES`:
 *   - `ENOENT`: @p pathname 所指代的文件不存在.
 *   - `ENOEXEC`: 尽管对 @p pathname 所指代的文件赋予了可执行权限,
 *   但系统却无法识别其文件格式.
 *   - `ETXTBSY`: 存在一个或多个进程已经以写入方式打开 @p pathname 所指代的文件.
 *   参考 open() 函数.
 *   - `E2BIG`: 参数列表和环境变量所需空间总和超出了允许的最大值.
 *
 * @see execve()
 * @see execle()
 * @see execlp()
 * @see execvp()
 * @see execv()
 * @see execl()
 */
int
execve(const char *pathname, char *const argv[], char *const envp[]);

/**
 * @brief
 *
 * @param pathname
 * @param arg
 * @param char *
 * @param envp[]
 *
 * @return
 *
 * @see execve()
 * @see execle()
 * @see execlp()
 * @see execvp()
 * @see execv()
 * @see execl()
 */
int
execle(const char *pathname, const char *arg, (char *)NULL, char *const envp[]);

/**
 * @brief 执行外部函数
 *
 * @code{.c}
 * execlp(argv[1], argv[1], "hello world", (char *)NULL);
 * @endcode
 *
 * @param filename
 * @param arg
 * @param ...
 *
 * @return
 *
 * @see execve()
 * @see execle()
 * @see execlp()
 * @see execvp()
 * @see execv()
 * @see execl()
 */
int
execlp(const char *filename, const char *arg, ... /*, (char *) NULL */);

/**
 * @brief
 *
 * @param filename
 * @param argv[]
 *
 * @return
 *
 * @see execve()
 * @see execle()
 * @see execlp()
 * @see execvp()
 * @see execv()
 * @see execl()
 */
int
execvp(const char *filename, char *const argv[]);

/**
 * @brief
 *
 * @param pathname
 * @param argv[]
 *
 * @return
 *
 * @see execve()
 * @see execle()
 * @see execlp()
 * @see execvp()
 * @see execv()
 * @see execl()
 */
int
execv(const char *pathname, char *const argv[]);

/**
 * @brief 执行外部函数
 *
 * @code{.d}
 * execl("/usr/bin/printenv", "printenv", "USER", "SHELL", (char *)NULL);
 * @endcode
 *
 * @param pathname
 * @param arg
 *
 * @return
 *
 * @see execve()
 * @see execle()
 * @see execlp()
 * @see execvp()
 * @see execv()
 * @see execl()
 */
int
execl(const char *pathname, const char *arg, ... /* , (char *) NULL */);
