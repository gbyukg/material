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
 * 日历时间: 无论地址位置如何, UNIX 系统内部对时间的表示方式均是以自 Epoch
 * 以来的秒数来度量的, Epoch 亦即通用协调时间(UTC, 以前也称为格林威治标准时间,
 * 过 GMT)的 1970-01-01 日早晨零点.
 * @author Lock
 * @version V1.0
 * @date 2016-01-10
 */

#include <sys/time.h>

/**
 * @brief 用秒数保存的时间
 */
struct timeval {
    time_t      tv_sec;  //<! 从 1970.1.1:00:00:00 秒开始的秒数
    suseconds_t tv_usec; //<! 微秒级的精度
};

/**
 * @brief 保存日期和时间的结构体
 */
struct tm {
    int tm_sec;     //<! 秒数 (0-60)
    int tm_min;     //<! 分钟 (0-60)
    int tm_hour;    //<! 小时 (0-23)
    int tm_mday;    //<! 日期 (1-31)
    int tm_mon;     //<! 月份 (0-11)
    int tm_year;    //<! 年份 (自从1900开始算起, 其值为当前年份 - 1900)
    int tm_wday;    //<! 星期 (Sunday = 0)
    int tm_yday;    //<! 一年中的第几天(0-365; 1月1号 = 0)
    int tm_isdst;   //<! 夏令时标识符，实行夏令时的时候，tm_isdst为正. 不实行夏令时的进候，tm_isdst为0；不了解情况时，tm_isdst()为负。
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
 *
 * @return 返回自 Epoch 以来的秒数
 * @retval -1 函数执行失败
 *
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
