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
 * 以便从小整数开始分配.
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
 * 当创建完一个子进程后, 父进程与子进程都会从 fork() 的返回出继续执行.
 *
 * 子进程执行与父进程相同的代码段, 即共享父进程的代码段, 但是有自己独立的栈段,
 * 数据段以及堆的拷贝. 子进程的栈, 数据以及栈段开始时是对父进程内存相应个部分的完全复制.
 *
 * 在子进程创建完成后, 系统会先执行父进程程序还是子进程程序完全是无法预测的.
 *
 * 子进程继承自父进程的属性:
 *   - 父进程中所有打开的文件描述符的副本, 这些副本的创建方式类似于 dup(),
 *   这也意味着父子进程将共享文件的偏移量以及文件状态标志.
 *   - 父进程中注册的退出处理程序, 参见 atexit() 函数.
 *
 *
 * @return 根据返回值确定是在父进程中还是子进程中:
 *   - 父进程中, 返回新创建的子进程的 进程ID
 *   - 子进程中, 返回0
 * @retval -1 创建子进程失败, 失败的原因可能是:
 *   - 进程数量超出了当前真是用户可创建的最大进程数的限制.
 *   - 系统进程总数达到了最大限制.
 */
pid_t
fork(void);

/**
 * @brief 退出进程
 *
 * 退出进程的执行, 并返回给父进程一个标识码, 用于标识进程执行的状态.
 * 一般 0 用于表示执行成功, 非0 表示执行失败.
 *
 * 虽然可将 0~255 之间的任意整数付给 @p status, 当将大于 128 的值最为退出状态容易引发混乱,
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
 * 该函数是对 _exit() 函数的封装, 执行流程如下:
 *   - 调用退出处理程序(通过 atexit() 和 on_exit() 注册的函数)
 *   - 刷新 `stdio` 流缓冲区
 *   - 使用由 @p status 提供的值执行 _exit() 系统调用
 *
 * @param status 指定的程序退出状态码
 *
 * @note 非异步信号安全函数, 因为该函数会刷新 stdio 流缓冲区.
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
 * 可以注册多个退出处理函数(设置将同一函数注册多次). 当调用 exit() 时,
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
 * 在执行该函数时, 如果当前没有子进程结束执行, 则阻塞系统调用, 直到某个子进程终止.
 * 如果在运行 wait() 函数之前已经有子进程终止, 则 wati() 马上返回.
 * 内核将会为父进程下所有子进程的运行总量追加到进程 CPU 时间, 参照 times() 函数.
 *
 * @param status 若该参数不为 NULL, 则子进程如何终止的信息会通过该参数指向的整形变量返回.
 *
 * @return 函数执行的状态, 若函数执行成功, 返回捕获到的子进程的 进程ID
 * !retval -1 函数执行失败, 失败的原因可能是进程中已经无子进程在执行,
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
