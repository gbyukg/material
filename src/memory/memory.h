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
 * @file memory.h
 * @brief 内存相关函数
 *
 * @image html process_memory.png
 *
 * @author Lock
 * @version V1.0
 * @date 2016-01-09
 */

/**
 * @brief 调整内存边界
 *
 * <unistd.h>
 *
 * 将堆的内存边界设置为 @p end_data_segment 所指定的位置. 由于虚拟内存以 页
 * 为单位进行分配, @p end_data_segment 实际会四舍五入到下一个内存页的边界处.<BR>
 * 当试图将 program break 设置为一个低于其初始值(即低于 @p end)的位置时,
 * 有可能会导致无法预知的行为.
 *
 * @param end_data_segment 指定要设置的 program break 的位置.
 *
 * @return 返回函数执行的状态
 * @retval 0 函数执行成功
 * @retval -1 函数执行失败
 *
 * @see sbrk()
 */
int
brk(void *end_data_segment);

/**
 * @brief 增加 program break
 *
 * <unistd.h>
 *
 * 该函数将 program break 在原有地址上增加 @p increment 的大小.
 *
 * @param increment 指定要增加的地址的大小.
 *
 * @return 返回前一个 program break 的地址. 换言之, 如果 program break 增加,
 * 那么返回值是指向这块新分配内存起始位置的指针.
 *
 * @code{.c}
 * // 返回当前 program break 地址
 * sbrk(0)
 * @endcode
 *
 * @see brk()
 */
void *
sbrk(intptr_t increment);

/**
 * @brief 分配内存
 *
 * <stdlib.h>
 *
 * 在堆上分配一块大小为 @p size 的内存空间供用户使用, 在使用过后,
 * 该内存空间必须使用 free() 释放.<BR>
 * 新分配的内存并不会被初始化. 且返回内存块所采用的字节对齐方式,
 * 总是事宜于高效访问任何类型的 C 语言数据结构. 在大多数硬件架构上,
 * 这实际意味着 malloc 是基于 8 字节或 16 字节边界来分配内存的.
 *
 * @param size 要分配内存的大小
 *
 * @return 返回一个指向新分配出来的内存空间的指针
 * @retval NULL 内存分配失败.
 *
 * @see free()
 * @see calloc()
 */
void *
mallco(size_t size);

/**
 * @brief 释放分配的内存
 *
 * @param ptr 指向要释放的内存的指针
 */
void
free(void *ptr);

/**
 * @brief 给一组相同对象分配内存.
 *
 * <stdlib.h>
 *
 * 该函数用于给一组相同的对象分配内存, 并将分配的内存初始化为 0.
 *
 * @param numiterms 要分配对象的数量
 * @param size 每个对象所占的大小
 *
 * @return 返回一个指向新分配出来的内存空间的指针
 * @retval NULL 内存分配失败
 *
 * @code{.c}
 * // 分配一块用于保存 10 个整形变量的内存空间
 * p = calloc(10, sizeof(int));
 * @endcode
 *
 * @see malloc()
 */
void *
calloc(size_t numiterms, size_t size);

/**
 * @brief 调整内存
 *
 * <stdlib.h>
 *
 * 该函数通常用于增加之前通过 malloc() 函数分配出来的内存块的大小.<BR>
 * 对于新增加出来的内存区域, 并不会对其进行初始化操作.
 *
 * @param ptr 指向需要调整大小的内存块的指针
 * @param size 指定所需调整大小的期望值.
 *
 * @return 返回一个指向调整后的内存块空间的指针
 * @retval NULL 函数执行失败, 内存分配失败.
 */
void *
realloc(void *ptr, size_t size);

/**
 * @brief 用于在堆上分配内存.
 *
 * <alloca.h>
 *
 * 不同于 malloc() 系列函数, 该函数通过增加栈帧的大小从栈上分配内存.<BR>
 * 根据定义, 当前调用函数的栈帧委屈堆栈的顶部, 因此这种方法是可行的.
 * 因此, 帧的上方存在扩展空间, 只需修改堆栈指针的值即可.<BR>
 * 由于是在栈上分配的内存空间, 属于自动变量类型, 因此在函数执行完毕以后,
 * 会自动释放该块内存, 而用并且也绝对不可以使用 free() 函数来释放该块内存空间.
 * 程序会进行自动释放.<BR>
 * 若调用 alloca() 造成堆栈溢出, 则程序的行为无法预知.
 *
 * @param size 指定在堆上分配的字节大小.
 *
 * @return 返回一个指向新分配出来的内存区域的指针.
 * @retval NULL 函数执行失败, 内存分配失败.
 */
void *
alloca(size_t size);
