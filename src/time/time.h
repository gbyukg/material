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
 * @file time.h
 * @brief 日期相关函数
 *
 * 日历时间: 无论地址位置如何, UNIX 系统内部对时间的表示方式均是以自 Epoch
 *
 * 时区: 不同国家, 甚至同一国家的不同地区都使用了不同的时区.<BR>
 * 为了使用千变万化的时区, 系统没有将其直接编码于程序或函数, 而定义在了系统文件
 * `/usr/share/zoneinfo` 目录下的文件中.<BR>
 * 系统的本地时间由 `/etc/localtime` 文件定义的. <BR>
 * 为运行中的程序指定一个时区, 需要将 TZ 环境变量设置为由冒号(:)和时区名称组合的字符串,
 * 如: `TZ=":Pacific/Auckland"`
 * 其中时区名称定义于 `/usr/share/zoneinfo` 中. 设置时区会自动影响到函数:
 * ctime(), localtime(), mktime() 和 strftime(). <BR>
 * 为了获取当前的时区设置, 上述函数都会调用 tzset(), 该函数首先检查环境变量 TZ.
 * 如果尚为设置该变量, 那么就采用 `/etc/localtime` 中定义的默认时区来初始化时区.
 * 如果 TZ 环境变量的值为空, 或无法与时区文件名相匹配.
 *
 * 地区(Locale): 同时区一样, 种类浩瀚且多变, 因此是由系统定义的. 地区信息维护与
 * `/usr/share/local`(在一些发行版本中为 `/usr/lib/local`) 之下的目录层次结构中.
 *
 * 以来的秒数来度量的, Epoch 亦即通用协调时间(UTC, 以前也称为格林威治标准时间,
 * 过 GMT)的 1970-01-01 日早晨零点.
 * @image html time.png
 * @author Lock
 * @version V1.0
 * @date 2016-01-10
 */



#include <sys/time.h>

/**
 * @brief 时间类型
 *
 * 在 32 位 Linux 系统中, 它使一个有符号整数, 可以表示的日期范围从
 * 1901/12/13 20:45:52 至 2038/1/19 03:14:07.<BR>
 * 64 位系统则没有这中问题.
 *
 * @see http://lxr.free-electrons.com/source/include/linux/types.h#L69
 */
typedef __kernel_time_t time_t;

/**
 * @brief 用秒数保存的时间
 *
 * @see gettimeofday()
 */
struct timeval {
    time_t      tv_sec;  //!< 从 1970.1.1:00:00:00 秒开始的秒数
    suseconds_t tv_usec; //!< 微秒级的精度
};

/**
 * @brief 保存分解的日期和时间的结构体
 */
struct tm {
    int tm_sec;     //!< 秒数 (0-60)
    int tm_min;     //!< 分钟 (0-60)
    int tm_hour;    //!< 小时 (0-23)
    int tm_mday;    //!< 日期 (1-31)
    int tm_mon;     //!< 月份 (0-11)
    int tm_year;    //!< 年份 (自从1900开始算起, 其值为当前年份 - 1900)
    int tm_wday;    //!< 星期 (Sunday = 0)
    int tm_yday;    //!< 一年中的第几天(0-365; 1月1号 = 0)
    int tm_isdst;   //!< 夏令时标识符，实行夏令时的时候，tm_isdst为正. 不实行夏令时的进候，tm_isdst为0；不了解情况时，tm_isdst()为负。
};

/**
 * @brief 用于保存进程时间的结构
 *
 * 数据类型 clock_t 使用时钟计时单元(clocl tick)为单位度量时间的整型值,
 * 习惯用语计算 @ptr tms 结构体的4个字段. 可以调用 sysconf(_SC_CLK_TCK)
 * 来获得没秒包含的时钟计时单元数, 然后用这个数组除以 clock_t 转换成秒.
 */
struct tms {
    clock_t tms_utime;  //!< 用户模式下 CPU 所占用时间
    clock_t tms_stime;  //!< 内核模式下 CPU 所占用时间
    clock_t tms_cutime; //!< 用户 CPU 与 子进程用户 CPU 时间总和
    clock_t tms_cstime; //!< 内核 CPU 与 子进程内核 CPU 时间总和
};

/**
 * @brief 用于 nanosleep() 函数.
 */
struct timespec {
    time_t tv_sec;  //!< 秒数
    log    tv_nsec; //!< 纳秒值
};

/**
 * @brief 获取当前的日历时间
 *
 * @param tv 用于保存获取到的日历时间
 * @param tz 历史遗留产物, 应该总是设置为 NULL
 *
 * @return 返回函数的执行状态
 * @retval 0 函数执行成功
 * @retval -1 函数执行失败
 *
 * @see time()
 */
int
gettimeofday(struct timeval *tv, struct timezone *tz);

/**
 * @brief 返回自 Epoch 以来的秒数.
 *
 * <time.h>
 *
 * 返回的值与 gettimeofday() 中返回的 @ref timeval 结构体中的 @ref tv_sec 相同.
 *
 * @param timep 如果该参数不为 NULL, 还会将返回的秒数置于 @p timep 所指向的位置.
 * 不过通常将该参数设置为 NULL
 *
 * @return 返回自 Epoch 以来的秒数
 * @retval -1 函数执行失败
 *
 * @code{.c}
 * t = time(NULL);
 * @endcode
 * @see gettimeofday()
 */
time_t
time(time_t *timep);

/**
 * @brief 返回一个可打印的日期格式
 *
 * <time.h>
 *
 * 该函数返回一个<B>静态的</B>字符串指针, 长度为 26 个字节的字符串, 内涵标准格式的日期和时间, 如:
 * Web Jun  8 14:22:30 2015<BR>
 * 该字符串包含了换行符和终止符.<BR>
 * 由于返回的指针是静态分配的, 因此多次对 ctime() 函数调用会覆盖上次获取的值.<BR>
 * 同时对 gmtime(), localTime() 或 asctime() 函数的调用都可能会覆盖其由. 换言之,
 * 这些函数可以共享返回的字符数组和 @ref tm 结构体.
 *
 * @param timep 设置要打印的时间
 *
 * @return 返回一个静态分配的字符串指针, 包含换行符和字符串结尾符.
 * @retval NULL 函数执行失败.
 */
char *
ctime(const time_t *timep);

/**
 * @brief 将日历时间转换为分解时间 @ref tm
 *
 * 该函数返回的结果是经静态分配的, 因此主要后续操作对该结构体的更改以及其他函数调用对它的影响.
 *
 * @param timep 要转换的日历时间
 *
 * @return 返回一个经<B>静态</B>分配的 @ref tm 结构
 * @retval NULL 函数执行失败
 */
struct tm *
gmtime(const time_t *timep);

/**
 * @brief 将日历时间转换为分解时间 @ref tm
 *
 * 该函数在转换过程中同时考虑本地系统中的时区和夏令时设置.<BR>
 * 该函数返回的结果是经静态分配的, 因此主要后续操作对该结构体的更改以及其他函数调用对它的影响.
 *
 * @param timep 要转换的日历时间
 *
 * @return 返回一个经<B>静态</B>分配的 @ref tm 结构
 * @retval NULL 函数执行失败
 */
struct tm *
localtime(const time_t *timep);

/**
 * @brief 将分解时间转换成日历时间
 *
 * 该函数可能会修改 @p timeptr 所指向的结构体, 至少会确保对 tm_wday 和 tm_day
 * 字段的设置, 会与其他输入字段的值对应起来.<BR>
 * 同时 @p timeptr 中的任意字段的值超出了范围, 都会对其进行相应的修正.
 *
 * @param timeptr 指向 @ref tm 结构的指针
 *
 * @return 返回获取到的日历时间
 * @retval -1 函数执行失败
 */
time_t
mktime(struct tm *timeptr);

/**
 * @brief 将分解时间转换成与 ctime() 返回结构一样的打印时间
 *
 * @param timeptr 执行 @ref tm 的结构的指针
 *
 * @return 返回一个经由静态分配的字符串指针
 * @retval NULL 函数执行失败.
 */
char *
asctime(const struct tm *timeptr);

/**
 * @brief 将分解时间转换成可打印的字符串
 *
 * 该函数提供了更加精确的控制来制定日期字符串输出的样式
 *
 * @param outstr 保存函数返回的日期字符串
 * @param maxsize 制定了 @p outstr 的最大长度
 * @param format 如何格式化日期字符串
 * @param timeptr 指向一个 @ref tm 的分解时间结构体指针
 *
 * @return 返回在 @p outstr 中替换掉的字符串个数
 * @retval 0 函数执行失败
 */
size_t
strftime(char *outstr, size_t maxsize, const char *format,
        const struct tm *timeptr);

/**
 * @brief 服务于定时器函数
 */
struct itimerval {
    struct timeval it_interval; //!< 设置触发器触发周期的时间,如果该字段所指向的 @ref timeval 中的值都为0, 则定时器为一次性的.
    struct timeval it_value;    //!< 指定距离定时器到期的延迟时间. 如果该字段所指向的 @ref timeval 中的值都为0, 则屏蔽之前所有的定时器.
};

/**
 * @brief 创建一个间隔式定时器.
 *
 * <sys/time.h>
 *
 * 这种定时器会在未来某个时间点到期, 并与此后(可选地)每隔一段时间到期一次.<BR>
 * 进程只能拥有 @p which 可以指定的三种定时器中的一种. 当第二次调用 setitimer()
 * 时, 修改已有定时器的属性要符合 @p which 中的类型.<BR>
 * 通过 setitimer() 函数创建的定时器可以跨越 exec() 调用而得以保存, 但由 fork()
 * 创建的子进程并不集成该定时器.
 *
 * @param which 指定何种类型的定时器:
 *   - `ITIMER_REAL` 创建以真实时间倒计时的定时器. 到期时会产生 @ref SIGALARM 信号.
 *   - `ITIMER_VIRTUAL` 创建以进程虚拟时间(用户模式下的 CPU 时间)倒计时的定时器.
 *   到期会产生信号 @ref SIGVTALRM.
 *   - `ITIMER_PROF` 创建一个 profiling 定时器, 以进程时间(用户态与内核态 CPU 时间的总和)倒计时,
 *   到期会产生 @ref SIGPROF 信号.
 * @param new_value 通过 @ref itimerval 结构, 设置定时器为一次性定时器还是周期性定时器.<BR>
 * @param old_value 如果该参数不为 NULL, 则用于保存调用 setitimer() 函数之前系统的定时器信息.
 *
 * @return 返回函数的执行状态
 * @retval 0 函数执行成功
 * @retval-1 函数执行失败
 *
 * @see alarm()
 */
int
setitimer(int which, const struct itimerval *new_value,
        struct itimerval *old_value);

/**
 * @brief 返回由 @p which 指定的定时器的当前状态
 *
 * @param which 指定何种类型的定时器.
 * @param curr_value 保存获取到的定时器信息.
 *
 * @return 返回函数执行状态
 * @retval 0 函数执行成功.
 * @retval -1 函数执行失败.
 */
int
getitime(int which, struct itimerval *curr_value);

/**
 * @brief 简单的创建一次性定时器函数
 *
 * 定时器到期时, 会响调用进程发送 @ref SIGALRM 信号.<BR>
 * 调用 alarm() 会覆盖对定时器的前一个设置.
 * @param seconds 定时器到期的秒数. 如何设置为 0, 则屏蔽现有的定时器.
 *
 * @return 返回定时器的前一个设置距离到期的剩余秒数, 如果未设置定时器, 则返回0.
 *
 * @note alarm() 函数有可能与 setitimer() 在同一进程中有可能会共享同一定时器,
 * 因此系统中只能选择这两个函数中的一个函数用于设置定时器.
 *
 * @see setitimer()
 */
unsigned int
alarm(unsigned int seconds);

/**
 * @brief 低分辨率休眠
 *
 * <unistd.h>
 *
 * 暂停调用今晨的执行达数秒之久, 或在捕获到信号后恢复进程的运行.<BR>
 * 如果休眠正常结束, 返回返回0. 如果因为信号而中断休眠, sleep() 将返回剩余的(未休眠的)秒数.<BR>
 * 由于系统负载原因, 内核可能会在完成 slepp() 的一段(通常很短)时间后才对进程重新加以调度.
 *
 * @param seconds 指定要休眠的时间(秒).
 *
 * @return 返回 0 或者未休眠的秒数.
 *
 * @note sleep() 在有些系统上可能是通过 alarm() 函数来实现, 为了考虑可移植性,
 * 应避免这3个函数同时出现.
 */
unsigned int
sleep(unsigned int seconds);

/**
 * @brief 高分辨率休眠
 *
 * #define _POSIX_C_SOURCE 199309<BR>
 * <time.h>
 *
 * 相对于 sleep() 函数, 能够以更高分辨率来设置休眠间隔时间.
 *
 * SUSv3规定, 该函数不可以使用信号来实现. 这意味着, 与 sleep() 不同,
 * 即使与 alarm() 或 setitimer() 混用, 也不会危及程序的可移植性.
 *
 * @param request 指向一个 @ref timespec 结构,设置休眠时间.
 * @param remain 指向一个 @ref timespec 结构, 如果不为 NULL, 则返回剩余的休眠时间.
 *
 * @return 返回函数执行状态
 * @retval 0 休眠完成
 * @retval -1 函数执行失败
 * @retval n 剩余未休眠的时间
 */
int
nanosleep(const struct timespec *request, struct timespec *remain);
