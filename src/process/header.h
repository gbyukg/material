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
 * 每创建一个新进程, 内核会按顺序将一下一个可用的进程号分配给其使用,
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
 * 子进程会获得父进程所有文件描述符的副本. 这些副本的创建方式类似于 dup(),
 * 这也意味着父子进程将共享文件的偏移量以及文件状态标志.
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
 * @see eixt()
 */
void
exit(int status);
