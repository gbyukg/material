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
