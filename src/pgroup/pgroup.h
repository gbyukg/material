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
 * @file pgroup.h
 * @brief 进程组, 会话和作业
 *
 * ## 进程组
 * 进程组是由一个或多个拥有同一 进程组标识符(PGID) 的进程组成.
 * 进程组ID 类型与 进程ID 一样都是 `pid_t`.
 *
 * 每个进程组都拥有一个 <B>进程组首进程</B>, 首进程是创建该进程组的进程,
 * 进程组ID 就是该首进程的 ID.
 *
 * 新进程会继承父进程所属的进程组ID.
 *
 * 进程组拥有一个生命周期, 其开始时间就是进程组首进程创建的时间,
 * 结束时间是进程组中最后一个进程结束的时间.
 * 一个进程可能是因为执行结束而退出进程组, 也可能是加入另一个进程组而退出当前的进程组.
 * 首进程不一定是进程组中最后一个退出进程组的进程.
 *
 * ## 会话
 * 会话是 <B>进程组</B> 的集合, 每个会话都拥有一个会话标识符(SID), 其下的所有进程都拥有相同的会话ID,
 * 会话标识符与进程组ID一行, 是一个类型为 `pid_t` 的数字.<BR>
 * 会话首进程是创建该会话的进程, 其进程ID会成为 会话ID. 新进程会继承父进程的 会话ID.
 *
 * 一个会话中的所有进程共享一个控制终端. 控制终端会在会话首进程打开一个终端设备时被建立.
 * 一个终端最多可能会成为 <B>一个</B> 会话的控制终端.<BR>
 * 在任意时刻, 会话中其中一个 <B>进程组</B> 会成为前端进程组, 其他进程组会成为后台进程组.
 * 只有前端进程组才能从控制终端中读取输入.<BR>
 * 当用户在控制终端输入其中一个信号生成终端字符后, 该信号会被发送给前端进程组中的所有进程.
 * 这些字符包括:
 *   - `SIGINT`: 中断(通常是 Control-C 生成)
 *   - `SIGQUIT`: abort 信号(通常是 Control-\ 生成)
 *   - `SIGSTP`: 进程挂起(通常是 Control-Z 生成)
 *
 * 当到控制终端的连接建立起来之后, 会话首进程会成为该终端的控制进程.<BR>
 * 成为控制进程的组要标志是: 当断开与终端之间的连接时内核会向该进程发送一个 `SIGHUP` 信号.
 *
 * ## 作业
 *
 * @author Lock
 * @version V1.0
 * @date 2016-02-02
 */

/**
 * @brief 获取进程组 ID
 *
 * 进程组ID 类型与 进程ID 相同, 都为 `pid_t` 类型.
 *
 * @return 返回获取到的进程组 ID, 该函数永远会执行成功.
 */
pid_t
getpgrp(void);

/**
 * @brief 修改进程的 进程组ID
 *
 * 如果 @p pid 和 @p pgid 指定了同一个进程(即 pgid 为0 或者与 ID 为 @p pid 的进程的进程ID匹配),
 * 那么就会创建一个新进程组, 并且指定的进程会成为这个新组的首进程.<BR>
 * 如果不同, 则会将进程 @p pdi 从一个进程组中移到另一个进程组中.
 *
 * @param pid 要修改的 进程ID. 如果为 0, 则修改当前的调用进程. 对于该参数, 还存在一些限制:
 *   - @p pid 必须为调用进程自身或者他的子进程, 否则会导致 `ESRCH` 错误.
 *   - 在组之间移动进程时, 调用进程, @p pid 进程, 目标进程组必须要属于同一会话组,
 *   否则会导致 `EPERM` 错误.
 *   - @p pid 必须不能够是 会话首进程.
 *   - @p pid 一个进程在其子进程执行了 exec() 系列函数之后就无法修改该子进程的进程组 ID 了,
 *   否则会导致 `EACCES` 错误.
 * @param pgid 为进程 @p pid 指定的新进程组ID. 如果为0,
 * @p pid 的进程组的值会被设置成 @p pid 的值.
 *
 * @return 返回函数执行状态
 * @retval 0 函数执行成功
 * @retval -1 函数执行失败
 */
int
setpgid(pid_t pid, pid_t pgid);
