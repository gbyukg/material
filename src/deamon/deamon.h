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
 * @file deamon.h
 * @brief 创建守护进程
 *
 * 创建一个守护进程, 需要完成以下几个步骤:
 *   - 进程执行一个 fork(), 之后父进程退出, 子进程继续执行. 这样做可以确保子进程不是进程组首进程,
 *   这样才可以执行下面的步骤: 调用 setsid() 创建一个新的会话.
 *   进程组首进程调用 setsid() 创建新会话会调用失败.
 *   - 在子进程中执行 setsid() 开启一个新的会话, 使之与当前控制终端断开连接.
 *   此时子进程会成为新会话的会话首进程.
 *   - 由于此时的子进程是新会话中的首进程, 子进程在后续的操作中可能会无意中打开一个控制终端,
 *   为了防止这种情况的发生, 可以采用以下两种方式:
 *     - 在所有可能应用到一个终端设备上的 open() 调用中指定 `O_NOCTTY` 标记
 *     - 在 setsid() 调用之后再次执行 fork() 创建出孙子进程, 在让该子进程退出,
 *     孙子进程继续执行. 这样就确保了孙子进程不会是新会话的会话首进程,
 *     进程永远不会重亲请求一个控制终端.
 *   - 调用 umask() 清除进程的 umask, 确保当 deamon 创建文件和目录时拥有所需的权限.
 *   - 修改进程的当前工作目录, 一般会修改为跟目录(`/`). 因为当程序运行时,
 *   程序的所在目录将无法被卸载.
 *   - 关闭 deamon 从父进程继承来的所有打开着的文件描述符, 包括0, 1, 2 这3个标准文件.
 *   - 在关闭了0, 1, 2 标准文件之后, deamon 通常会打开 `/dev/null`, 并使用 dup2()
 *   使这3个文件描述符指向 `/dev/null` 的文件描述符, 这么做是为了防止一些库中可能会使用这些标准文件符,
 *   但是因为找不到而发生错误的情况.
 *
 * @author Lock
 * @version V1.0
 * @date 2016-02-03
 *
 * @example deamon.c
 */

