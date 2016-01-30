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
 * @file pthread.h
 * @brief POSIX 线程
 *
 * 一个进程中可以包含多个线程, 每个线程都会独立执行相同程序,
 * 且共享同一份全局内存区域, 其中包括初始化数据段, 未初始化数据段,
 * 以及堆内存段.
 *
 * 相对于多进程来说, 线程有更多的优势:
 *   - 速度更快. fork() 即便使用了写时复制技术, 仍然需要复制诸如内存页表和文件描述符表之类的多重进程属性,
 *   这意味着 fork() 调用在时间上的开销依然不菲.
 *   线程是基于 closen() 函数实现的, fork() 复制的诸多属性, 在线程中本来就是共享的.
 *   特别是, 既无需采用写时复制来复制内存页, 也无需复制制表页.
 *   这就是线程的创建要快于进程的创建 10 至 100 倍. 因为这一点,
 *   可以大量创建线程而无需太过于担心 CPU 和内存不足的问题.
 *   - 线程之间能够方便, 快速的共享信息. 只需将数据复制到共享(全局或堆)变量中即可.
 *   不过应当避免多个线程之间试图同时修改同一变量的情况发生.
 *
 * 共享属性 | 非共享属性
 * -------- | ---------
 * 进程ID(process ID) 和 父进程ID | 线程 ID
 * 进程组 ID 与 会话 ID | 信号掩码 (signal mask)
 * 控制终端 | 线程特有数据
 * 进程凭证 (process credential) (用户 ID 和 组 ID) | 信号备选栈 (sigaltstack())
 * 打开的文件描述符 | errno 变量
 * 由 fcntl() 创建的记录所 (record lock) | 浮点型 (floating-point) 环境
 * 信号处置 | 实时调度策略 和 优先级
 * 文件系统相关的信息: 文件权限掩码 (umask), 当前工作目录和根目录 | CPU 亲和力
 * 间隔定时器(setitimer()) 和 POSIX 定时器(timer_create()) | 能力
 * 系统 V 信号量撤销值 | 栈, 本地变量和函数的调用链接 (linkage) 信息
 * 资源限制 | <BR>
 * CPU 时间消耗 (由 times() 返回) | <BR>
 * 资源消耗 (由 getrusage() 返回) | <BR>
 * nice 值 (由 setpriority() 和 nice() 设置) | <BR>
 *
 * @author Lock
 * @version V1.0
 * @date 2016-01-30
 */

/**
 * @brief 创建线程
 *
 * <pthread.h>
 *
 * 创建新的线程之后, 新线程会从 @p start 所指定的函数开始执行.
 *
 * @param thread 指向 pthread_t 类型的缓冲区, 在 pthread_create() 返回前,
 * 会在此保存一个该线程的唯一标识. 后续的 Pthreads 函数将使用该表示来引用此线程.<BR>
 * SUSv3 规定, 在新线程开始执行之前, 实现无需对 @p thread 参数所指向的缓冲区进行初始化,
 * 即新线程可能会在 pthread_create() 函数返回之前就已经开始执行.
 * 如新线程需要获取自身的线程 DI, 则只能通过调用 pthread_self() 方法.
 * @param attr 指向 `pthread_attr_t` 对象的指针, 该对象指定了新线程的各种属性.
 * 如果该属性设为 NULL, 那么创建新线程时将使用各个默认属性.
 * @param start 执行一个返回类型为 `void *` 的函数指针, 新创建的线程会从该函数开始执行,
 * 当函数执行完毕, 该线程也将结束.
 * @param arg 为执行 @p start 函数时传递的参数. 一般情况下,
 * 该参数应当指向一个全局或堆变量, 也可以将其设置为 NULL.
 * 将参数类型设置为 `void`, 意味着可以将指向任意类型对象的指针传递给 @p start() 函数.
 * 如果要传递多个参数, 可以将 @p arg 指向一个结构, 结构中保存了要传递的多个参数.
 *
 * @return 返回函数的执行状态
 * @retval 0 函数执行成功
 */
int
pthread_create(pthread_t *thread, const pthread_attr_t *attr,
        void *(*start)(void *), void *arg);
