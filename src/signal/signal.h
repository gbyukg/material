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
 */
int
kill(pid_t pid, int sig);