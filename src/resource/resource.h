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
 * @file resource.h
 * @brief 进程资源
 * @author Lock
 * @version V1.0
 * @date 2016-02-03
 */

/**
 * @brief 用于保存进程资源信息的结构体
 */
struct rusage {
    struct timeval re_utime;    //!< 用户 CPU 占用时间
    struct timeval re_stime;    //!< 系统 CPU 占用时间
    long ru_maxrss;             //!< maximum resident set size
    long ru_ixrss;              //!< integral shared memory size
    long ru_idrss;              //!< integral unshared data size
    long ru_isrss;              //!< integral unshared stack size
    long ru_minflt;             //!< page reclaims
    long ru_majflt;             //!< page faults
    long ru_nswap;              //!< swaps
    long ru_inblock;            //!< block input operations
    long ru_oublock;            //!< block output operations
    long ru_msgsnd;             //!< messages sent
    long ru_msgrcv;             //!< messages received
    long ru_nsignals;           //!< signals received
    long ru_nvcsw;              //!< voluntary context switches
    long ru_nivcsw;             //!< involuntary context switches
};

/**
 * @brief 获取调用进程或其子进程用掉的各类系统资源的统计信息.
 *
 * @param who 指定要查询的进程, 可选值为:
 *   - `RUSGAE_SELF`: 返回调用进程相关信息.
 *   - `RUSAGE_CHILDREN`: 返回调用进程所有被终止和处于等待状态的子进程相关信息.
 *   - `RUSAGE_THREAD`: 返回调用线程的相关信息.
 * @param res_usage 指向一个 @ref rusage 结构的指针, 用于保存获取到的信息.
 *
 * @return 返回函数执行状态
 * @retval 0 函数执行成功.
 * @retval -1 函数执行失败.
 */
int
getrusage(int who, struct rusage *res_usage);

