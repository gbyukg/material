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
