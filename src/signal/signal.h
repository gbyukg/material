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
