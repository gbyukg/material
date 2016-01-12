/**
 * @file file.h
 * @author Lock
 * @date 29 Dec 2015
 * @brief 文件系统 I/O 相关函数
 *
 * @image html filesystem.png
 *
 * # 文件 I/O 缓冲区
 * 缓冲区分为 <B>用户空间缓冲区</B> 与 <B>内核缓冲区高速缓存</B>.
 * 可以通过控制这两个缓冲区域, 来影响文件的读写效率或是一些行为.
 * 如 read() 和 write() 函数的行为.
 *
 * ### stdio 库缓冲区
 * 当操作磁盘文件时, 缓冲大块数据以减少系统调用( read(), write()), C 语言函数库的 I/O 函数
 * 比如( fprintf(), fscanf(), fgets(), fputs(), fgetc())
 * 正是这么做的. 因此, 使用 stdio 库可以使编程者免于自行处理对数据的缓冲.
 *
 * ### 内核缓冲
 * 强制刷新内湖缓冲区到输出文件是可能的, 例如数据库应用要确保在继续操作前将输出真正的写入磁盘.
 * SUSv3 的两种同步方式:
 *   - `synchronized I/O data integrity completion`: 保证数据完整性,
 *   仅仅确保数据和一小部分文件属性(如文件大小)被正确地写入到了磁盘上.
 *   - `synchronized I/O file integrity completion`: 保证文件完整性,
 *   确保数据以及文件的所有属性都已经被正确的写入到了磁盘上.
 */

#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

/**
 * @def STDIN_FILENO
 * <unistd.h>
 *
 * 标准输入文件描述符
*/
#define	 STDIN_FILENO	0	/* standard input file descriptor */

/**
 * @def STDOUT_FILENO
 * <unistd.h>
 *
 * 标准输出文件描述符
*/
#define	STDOUT_FILENO	1	/* standard output file descriptor */

/**
 * @def STDERR_FILENO
 * <unistd.h>
 *
 * 标准错误文件描述符
*/
#define	STDERR_FILENO	2	/* standard error file descriptor */

/**
 * @def BUFSIZ
 *
 * 系统默认缓冲区大小<BR>
 * 系统会自动设定该大小为最适合的大小来作为输入输出缓冲区的大小.
 * 并且保证至少为 `256` 个字节.<BR>
 * 也可以通过查看 fstat() 函数返回的结构中的 `st_blksize` 字段来设定最适合系统缓冲区大小的值.
 */
#define BUFSIZ 1

/**
 * @brief 关闭指定的文件描述符
 *
 * #include <unistd.h>
 *
 * 文件描述符属于有限资源,当一个文件描述符不再使用时, 手动关闭该文件描述符是良好的编程习惯.
 *
 * @param fd 指定要关闭的文件描述符
 *
 * @return 返回函数执行的状态
 * @retval 0 函数执行成功
 * @retval -1 函数执行失败
 */
int
close(int fd);

/**
 * @brief 复制一个打开的文件描述符 @p oldfd.
 * 函数返回的文件描述符与 @p oldfd 都指向同一打开的文件句柄, 并且系统保证描述符一定是编号值最低的未使用的文件描述符.
 *
 * @param oldfd 将要被复制的文件描述符
 *
 * @return 函数执行成功返回一个新复制出来的文件描述符
 * @retval -1 函数执行失败
 */
int
dup(int oldfd);

/**
 * @brief 使用给定的值复制一个打开的文件描述符.
 * 与 dup() 函数功能一样, 唯一不同的是可以通过 @p newfd 来指定新复制的文件描述符编号.<BR>
 * 如果又 @p newfd 参数指定的编号文件描述符之前已经打开, 那么 dup2() 会首先将其关闭.
 * 值得注意的是, dup2() 会忽略掉在关闭期间出现的任何错误. 故此, 更为安装的做法是:
 * 在调用 dup2() 之前, 若 @p newfd 已经打开, 应该显示调用 close() 函数来关闭.<BR>
 * 如果 @p newfd 与 @p oldfd 相同, 那么 dup2() 函数将什么都不会做, 直接返回 @p oldfd.
 *
 * @param oldfd 将要被复制的文件描述符
 * @param newfd 新复制的文件描述符的编号
 *
 * @return 函数执行成功返回复制出来的文件描述符
 * @retval -1 函数执行失败
 *
 * @see dup2()
 * @see dup3()
 */
int
dup2(int oldfd, int newfd);

/**
 * @brief 在复制文件描述符时同时指定一个 @p flags 参数.
 *
 * @param oldfd 将要被复制的文件描述符
 * @param newfd 新复制的文件描述符的编号
 * @param flags 只支持一个标志 `O_CLOEXEC`, 这将促使内核为新文件描述符设置
 * `close-on-exec` 标志位.
 *
 * @return 函数执行成功返回复制出来的文件描述符
 * @retval -1 函数执行失败
 *
 * @see dup()
 * @see dup3()
 */
int
dup3(int oldfd, int newfd, int flags);

/**
 * @brief 对一个打开的文件描述符执行一系列控制操作.
 *
 * #include <fcntl.h>
 *
 * @param fd 一个已打开的文件描述符
 * @param cmd 一系列控制参数:
 *   - `F_GETFL`
 *
 * @retval -1 函数执行失败.
 *
 * @see dup()
 * @see dup2()
 */
int
fcntl(int fd, int cmd, ...);

/**
 * @brief 改变文件偏移量
 *
 * #include <unistd.h>
 *
 * 对于每个打开的文件, 系统内核会记录其文件偏移量, 用于指示下一次执行 read() 或
 * write() 操作的文件起始位置. 文件的第一个字节的偏移量为 0.<BR>
 * 文件打开时, 会将文件偏移量设置为指向文件开始, 以后每次 read() 或 write()
 * 调用将自动对其进行调整, 以指向已读取或以数据后的下一字节. 因此连续的 read()
 * 和 write() 调用将按顺序递进, 对文件进行操作.
 *
 * @param fd 文件描述符
 * @param offset 指定了一个以字节为单位的数值. 用于设置文件偏移量. off_t 为有符号整数.
 * @param whence 指定了要按照哪个基准点进行偏移, 可用的值:
 *   - `SEEK_SET` 从文件头部起始点开始的 offset 个字节
 *   - `SEEK_CUR` 从当前文件偏移量
 *   - `SEEK_END` offset 参数应该从文件最后一个字节之后的下一个字节算起
 *
 * @return 返回新的文件偏移量
 * @retval -1 函数执行失败
 */
off_t
lseek(int fd, off_t offset, int whence);

/**
 * @brief 创建临时文件
 *
 * #include <stdlib.h>
 *
 * 创建的临时文件拥有者对其拥有读写权限(其他用户没有任何权限), 且打开文件时使用了
 * `O_EXCL` 标志, 以保证调用者以独占方式访问文件.
 *
 * @param template 临时文件路径, 其中最后6个字符必须为 `XXXXXX`. 这6个字符将被替换,
 * 以保证文件名的唯一性, 且修改后的字符串将通过 @p template 参数传回. 因为会对传入的
 * @p template 参数进行修改, 所以必须将其指定为字符串数组, 而非字符串常量
 *
 * @code{.c}
 * char template[] = "/tmp/somestringXXXXXX";
 * int fd = mkstemp(template);
 * ...
 * unlink(template); // 删除创建的临时文件
 * if (close(fd) == -1)
 *     errExit("close");
 * @endcode
 *
 * @return 返回创建的临时文件的文件描述符
 * @retval -1 函数执行失败
 *
 * @see tmpfile()
 */
int
mkstemp(char *template);

/**
 * @brief 打开文件
 *
 * #include <sys/stat.h><BR>
 * #include <fcntl.h>
 *
 * 打开通过参数 @p pathname 指定的文件, 必须指定 `O_RDONLY`, `O_WRONLY` 和 `O_RDWR`
 * 中的一个标志位.
 *
 * @param pathname 要打开的文件的路径信息
 * @param flags 打开文件时指定的一些特性:
 *   - `O_RDONLY` 只读方式打开, 可通过 fcntl() 函数的 `F_GETFL` 获取.
 *   - `O_WRONLY` 只写方式打开, 可通过 fcntl() 函数的 `F_GETFL` 获取.
 *   - `O_RDWR` 读写方式打开, 可通过 fcntl() 函数的 `F_GETFL` 获取.
 *   - ---
 *   - `O_CLOEXEC` 设置 close-on-exec 标志位, 该标志位用于创建子进程时, 在子进程中自动关闭该文件描述符.
 *   - `O_CREAT` 如果要打开的文件不存在, 则创建文件, 此时 @p mode 参数用于指定创建文件时的权限位.
 *   - `O_DIRECT`
 *   - `O_DIRECTORY` 如果给定的路径参数不是目录, 则调用失败.
 *   - `O_EXCL` 与 `O_CREAT` 标志位一起使用, 如果指定的文件已经存在, 则调用失败,
 *   并返回错误, 错误号 errno 为 EEXIST. 该操作将文件的检查存在与创建成为一个原子性的操作.
 *   - `O_LARGEFILE` 用于打开大文件.
 *   - `O_NOATIME` 当通过 read() 读取文件时禁止更新文件的访问时间. 要使用该标志位,
 *   要么调用进程的有效用户ID与文件的拥有者相匹配, 要么进程需要用于特权.
 *   否则函数调用失败, 错误号 errno 为 EPERM.
 *   - `O_NOCTTY` 如果 @p pathname 引用的是终端设备, 则不将该设备分配作为次进程的控制终端.
 *   - `O_NOFOLLOW` 默认情况下, 如果给定的文件是一个链接, 该函数会通过链接找到根文件,
 *   该标志位如果是一个链接, 则报错.
 *   - `O_TRUNC` 如果文件存在, 且为只写或读写, 打开文件的同时截断文件内容, 即清空文件.
 *   - ---
 *   - `O_APPEND` 打开文件时自动将文件指针移动到文件结尾, 用于向文件中追加内容.
 *   - `O_ASYNC` 当对于 open() 调用所返回的文件描述符可以实施I/O操作时,
 *   系统会产生一个信号通知进程. 这一特性被称为信号驱动I/O, 仅对特定类型文件有效,
 *   如 socket, 终端, FIIFOS.
 *   - `O_NONBLOCK` 打开文件时设置该文件为非阻塞模式.
 *     - 当 open() 调用不能立即打开文件, 则返回错误, 而非陷入阻塞.
 *     - 调用 open() 成功后, 后续的 I/O 操作也是非阻塞的. 若 I/O 系统调用未能立即完成,
 *     则可能会只传输部分数据, 或者系统调用失败, 并返回 `EAGAIN` 或 `EWOULDBLOCK` 错误.
 *   - `O_DSYNC`: 要求写操作按照 synchronized I/O data integrity 来执行,
 *   类似于 fdatasync().
 *   - `O_SYNC` 使每次 write 等待物理I/O操作完成, 包括由该 write 操作引起的文件属性更新所需的I/O.
 *   即 synchronized I/O file integrity completion, 类似于 fsync() 函数.<BR>
 *   该参数会极大影响后续对文件操作的 I/O 效率.
 * @param mode 当使用 open() 创建一个文件时, 该标志位用于确定新创建按文件权限位.
 *   - `S_IRWXU`: owner RWX
 *   - `S_IRUSR`: owner R
 *   - `S_IWUSR`: owner W
 *   - `S_IXUSR`: owner X
 *   - `S_IRWXG`: group RWX
 *   - `S_IRGRP`: group R
 *   - `S_IWGRP`: group W
 *   - `S_IXGRP`: group X
 *   - `S_IRWXO`: other RWX
 *   - `S_IROTH`: other R
 *   - `S_IWOTH`: other W
 *   - `S_IXOTH`: other X
 *   - `S_ISUID`: set user id on execution
 *   - `S_ISGID`: set group id on execution
 *   - `S_ISVTX`: save swapped text even after use
 *
 * @return 成功返回打开文件对应的文件描述符, 系统保证了返回的文件描述符是最小整数.
 * @retval -1 函数执行失败, 并相应设置 `errno` 来表示具体错误原因
 *
 * @note 同一进程下的所有线程共享同一文件描述符.
 *
 * @see fileno();
 * @see fopen();
 * @see freopen()
 * @see fdopen();
 */
int
open(const char *pathname, int flags, mode_t mode);

/**
 * @brief 从指定位置开始读取
 *
 * #include <unistd.h>
 *
 * 使设置偏移量与读取操作成为原子操作
 *
 * @param fd 文件描述符
 * @param buf 用于保存读取到的字节
 * @param count 期待的要读取的字节数
 * @param offset 文件偏移量
 *
 * @return 返回实际读取到的字节数
 * @retval -1 函数执行失败
 *
 * @see read()
 * @see lseek()
 */
ssize_t
pread(int fd, void *buf, size_t count, off_t offset);

/**
 * @brief 从指定位置开始写入
 *
 * #include <unistd.h>
 *
 * 使设置偏移量与写入操作成为原子操作
 *
 * @param fd 文件描述符
 * @param buf 用于保存要写入的字节
 * @param count 期待的要写入的字节数
 * @param offset 文件偏移量
 *
 * @return 返回实际写入到文件的字节数
 * @retval -1 函数执行失败
 *
 * @see write()
 * @see lseek()
 */
ssize_t
pwrite(int fd, void *buf, size_t count, off_t offset);

/**
 * @brief 读取数据
 *
 * #include <unistd.h>
 *
 * 从文件描述符 @p fd 所指代的打开文件中读取数据.<BR>
 * 该函数返回的读取到的字符数有可能小于 @p count 指定的请求的字节数.
 *
 * @param fd open() 返回的所打开的文件描述符
 * @param buffer 用于保存读取到的数据
 * @param count 期待读取到的字节数
 *
 * @return 返回实际的读取到的字节数.
 * @retval 0 EOF, 文件结尾
 * @retval -1 函数执行失败
 *
 * @code{.c}
 * char buffer[MAX_READ + 1]; // 多出的一个字节用于保存字符串的终止符
 * ssize_t numRead;
 *
 * numRead = read(STDIN_FILENO, buffer, MAX_READ);
 * buffer[numRead] = '\0'; // 在读取到的字符串尾部添加终止字符
 * @endcode
 *
 * @note 由于该函数不会自动分配内存空间来保存读取到的数据, 因此在使用前必须手动创建一个内存区域来保存读取到的数据
 * @note read() 并不像 printf() 函数那样, 自动在读取到的数据结尾自动添加表示结尾的字符 `\0`,
 * 这是因为 read() 函数可以从文件中读取任意序列的字节. 有些情况下, 输入信息可能是文本数据,
 * 也有可能是二进制数据, 故无法遵从 C 语言对字符串处理的约定, 在字符串尾部追加标示字符串结束的空字符.
 * 因此需要手动在读取到的字符串尾部添加终止符.
 *
 * @note read()函数并不是直接从磁盘文件中读取数据, 内核会从磁盘文件中读取数据,
 * 并存储到内核缓冲区中. 当调用 read() 时, 则是从该缓冲区读取数据, 直至把缓冲区中的所有内容全部读取完毕,
 * 这时内核会将文件的下一段内容读入缓冲区高速缓存.(这里有大量的预读算法用于优化读取)
 *
 * @see write()
 * @see pread()
 */
int
read(int fd, void *buffer, size_t count);

/**
 * @brief 创建一个临时文件
 *
 * #include <stdio.h>
 *
 * 以读写方式创建一个名称唯一的临时文件, 同时设置了 `O_EXCL` 标志.
 * 文件流关闭后将自动删除临时文件.
 *
 * @return 返回一个新创建的文件流, 供 stdio 库函数使用.
 * @retval NULL 函数执行失败
 *
 * @see mkstemp()
 */
FILE *
tmpfile(void);

/**
 * @brief 截断文件
 *
 * #include <unistd.h>
 *
 * 该函数要求在打开文件是要包含写操作权限, 且该系统调用不会修改文件偏移量.
 *
 * @param pathname 要被截断的文件路径, 如果文件名为符号链接, 则将对其进行解引用.
 * @param length 截取后文件的大小
 *   - 若文件当前长度大于 @p length, 调用将丢弃超出部分;
 *   - 若文件当前长度小于 @p length, 调用将在文件尾部添加一些列空字节或是一个文件空洞.
 *
 * @return 返回函数执行状态
 * @retval 0 函数执行成功
 * @retval -1 函数执行失败
 *
 * @see ftruncate()
 */
int
truncate(const char *pathname, off_t length);

/**
 * @brief 截断文件
 *
 * #include <unistd.h>
 *
 * @param fd 要被截断的文件描述符.
 * @param length 截取后文件的大小
 *   - 若文件当前长度大于 @p length, 调用将丢弃超出部分;
 *   - 若文件当前长度小于 @p length, 调用将在文件尾部添加一些列空字节或是一个文件空洞.
 *
 * @return 返回函数执行状态
 * @retval 0 函数执行成功
 * @retval -1 函数执行失败
 *
 * @see truncate()
 */
int ftruncate(int fd, off_t length);

/**
 * @brief 写入数据
 *
 * 实际写入的字符数可能小于期待所写入的字符数, 这被称为部分写, 造成部分写的原因有可能是磁盘空间已满,
 * 或是进程资源对文件大小的限制.<BR>
 * 对 write() 函数调用成功, 并不代表数据已经被写入到文件中, 这是因为内核会对磁盘的
 * I/O 操作进行缓存处理.
 *
 * @param fd open() 返回的所打开的文件描述符
 * @param buffer 指向一个要写入的字符串指针
 * @param count 期待要写入的字节数
 *
 * @return 返回实际写入的字符个数
 * @retval -1 函数执行失败
 *
 * @note write() 并不会直接发起磁盘操作, 而是仅仅在用户空间缓冲区与内核缓冲区高速缓存之间复制数据.<BR>
 * 执行完 write() 函数后, 该函数立即返回, 数据只是被保存到了内核缓冲区高速缓存之中,
 * 之后等待某一时刻等待内核将其缓冲区里的数据刷新到磁盘上.<BR>
 * 如果再此期间, 另一进程试图访问这几个字节, 那么内核将自动从缓冲区高速缓存中提供这些数据,
 * 而不是从文件中(读取过期的内容)
 *
 * @see read()
 * @see pwrite()
 */
int
write(int fd, void *buffer, size_t count);

/**
 * @brief 设置一个 stdio 流的缓冲模式
 *
 * #include <stdio.h>
 *
 * 控制 stdio 库使用缓冲的形式
 *
 * @param stream 要修改的文件流.
 * @param buf 两种情况:
 *   - 如果不为 NULL, 那么其指向 @p size 大小的内存块以作为 stream 的缓冲区.
 * 因为 stdio 库将要使用 @p buf 指向的缓冲区, 所以应该以 <B>动态</B> 或
 * <B>静态</B> 在堆中为该缓冲区分配一块内存空间(使用 malloc() 或类似函数),
 * 而不是分配在栈上的函数本地变量. 否则函数返回时将销毁其栈针导致混乱.<BR>
 *   - 如果 @p buf 为NULL, 那么 `stdio` 库自动分配一个缓冲区. SUSv3 允许但不强制库实现使用
 *   size 来确定其缓冲区的大小. glibc 实现会在该场景下忽略 @p size 参数.
 * @param mode 指定了缓冲类型:
 *   - `_IONBF`: 不对 I/O 进行缓冲, 每个 stdio 库函数将立即调用 write() 或者
 *   read(), 并且忽略 @p buf 和 @p size 参数, 可以分别指定两个参数为 `NULL` 和 `0`.
 *   stderr 默认属于这一类, 从而保证错误能立即输出
 *   - `_IOLBF`: 采用行缓冲 I/O. 指代终端设备的流默认属于这种类型. 对于输出流,
 *   在输出一个换行符(除非缓冲区已经填满)前将缓冲数据. 对于输入流, 每次读取一行数据.
 *   - `_IOFBF`: 采用全缓冲 I/O. 单次读, 写数据(通过 read() 或 write()) 的大小与该缓冲区相同.
 *   指代磁盘的流默认采用此模式.
 * @param size 配合 @p buf 一起使用, 确定缓冲区大小.
 *
 * @retval 0 函数执行成功
 * @retval 非0 函数执行失败
 *
 * @note 该函数必须在调用任何其他 stdio 函数之前调用.
 * setvbuf() 调用将影响后续在 @p stream 流上进行的所有 stdio 操作.
 *
 * @see setbuf()
 * @see setbuffer()
 * @see fflush()
 */
#include <stdio.h>
int
setvbuf(FILE *stream, char *buf, int mode, size_t size);

/**
 * @brief 类似 setvbuf()
 *
 * 只是仅提供了两种缓冲设置: 无缓冲(`_IONBF`) 或 全缓冲(`_IOFBF`).
 *
 * @param stream 要修改的文件流.
 * @param buf 当该参数为 NULL 时, 设置 @p stream 为 `_IONBF`, 否则设置为 `_IOFBF`,
 * 且缓冲区大小为 @ref BUFSIZ.
 *
 * @return 该函数无返回结果.
 *
 * @see setvbuf()
 * @see setbuffer()
 * @see fflush()
 */
void
setbuf (FILE *stream, char *buf);

/**
 * @brief 类似 setvbuf, 但允许调用者指定 buf 缓冲区大小.
 *
 * @param stream 要修改的文件流.
 * @param buf 缓冲区
 * @param size 如果 @p buf 不为 `NULL`, 则用来指定缓冲区大小
 *
 * @return 无返回值
 *
 * @see setvbuf()
 * @see setbuf()
 * @see fflush()
 */
void
setbuffer(FILE *stream, char *buf, size_t size);

/**
 * @brief 刷新缓冲区
 *
 * 无论当前采用何种缓冲模式, 在任何时候, 都可以使用 fflush() 库函数强制将
 * stdin 输出流中的数据(即通过 write()) 刷新到<B>内核缓冲区</B>中.<BR>
 * 此函数将刷新指定指定 @p stream 的输出缓冲区.<BR>
 * 也可以将该函数应用于输入流, 但这将导致内核丢去当前所有已经缓冲的输入数据.
 * (当程序下一次尝试从流中读取数据时, 将重新装满缓冲区.)<BR>
 * 当关闭相应流时, 将自动刷新其 stdio 缓冲区.
 *
 * @param stream 指定要刷新的流文件, 若此参数为 `NULL`, 则刷新所有的 stdio 缓冲区.
 *
 * @return 返回函数执行状态.
 * @retval 0 函数执行成功
 * @retval EOF 函数执行失败
 */
int
fflush(FILE *stream);

/**
 * @brief Synchronized I/O file integrity completion
 *
 * <unistd.h>
 *
 * 该系统调用将使缓冲数据和与打开的 @p fd 相关的所有数据都刷新到磁盘上.<BR>
 * 该函数仅在对磁盘设备(或者至少是磁盘的高速缓存)的传递完成后, fsycn() 调用才会返回.
 *
 * @param fd 指定要操作的文件标示符
 *
 * @return 返回函数执行状态
 * @retval 0 函数执行成功
 * @retval -1 函数执行失败
 *
 * @see sync()
 * @see fdatasync()
 */
int
fsync(int fd);

/**
 * @brief Synchronized I/O data integrity completion
 *
 * <unistd.h>
 *
 * 该系统函数可能会减少对磁盘操作的次数, 因为会减少某些文件属性的传递.
 *
 * @param fd 指定要操作的文件标示符
 *
 * @return 返回函数执行状态
 * @retval 0 函数执行成功
 * @retval -1 函数执行失败
 *
 * @see fsync()
 * @see sync()
 */
int
fdatasync(int fd);

/**
 * @brief 更新全部内核缓冲区
 *
 * <unistd.h>
 *
 * sync() 系统调用会使包含更新文件信息的所有内核缓冲区(即数据块, 指针块, 元数据等)
 * 刷新到磁盘上.
 *
 * @return 该函数无返回值.
 *
 * @see fsync()
 * @see fdatasync()
 */
void
sync(void);

/**
 * @brief 返回文件描述符
 *
 * <stdio.h>
 *
 * @param stream 要转换的文件流
 *
 * @return 返回文件描述符
 * @retval -1 函数执行失败
 *
 * @see fdopen()
 */
int
fileno(FILE *stream);

/**
 * @brief 返回文件流
 *
 * <stdio.h>
 *
 * 根据给定的文件描述符, 创建一个使用该描述符进行文件 I/O 的相应流.
 *
 * @param fd 给定的文件描述符
 * @param mode 与 fopen() 函数中 mode 参数含义相同: r, w, a.
 * 如果该参数与文件描述符 @p fd 的访问模式不一致, 函数调用将失败.
 *
 * @return 返回文件流
 * @retval NULL 函数执行失败
 *
 * @see open()
 * @see fileno()
 * @see fopen()
 * @see freopen()
 */
FILE *
fdopen(int fd, const char *mode);

/**
 * @brief 根据文件路径名打开一个文件流
 *
 * <stdio.h>
 *
 * @param pathname 文件路径信息
 * @param type 指定对该 I/O 流的读写方式:
 *   - `r` 或 `rb`: 只读(O_RDONLY)
 *   - `w` 或 `wb`: 把文件截断至0长, 或为只写而创建(O_WRONLY|O_CREAT|O_TRUNC)
 *   - `a` 或 `ab`: 追加: 为在文件尾部而打开, 或为写而创建(O_WRONLY|O_CREAT|O_APPEND)
 *   - `r+` 或 `r+b` 或 `rb+`: 为读和写而打开(O_RDWR)
 *   - `w+` 或 `w+b` 或 `wb+`: 把文件截断至 0 长, 或为读写而打开(O_RDWR|O_CREAT|O_TRUNC)
 *   - `a+` 或 `a+b` 或 `ab+`: 为在文件尾读和写而打开或创建(O_RDWR|O_CREAT|O_APPEND)
 *
 * @see open()
 * @see fileno();
 * @see freopen()
 * @see fdopen();
 */
FILE *
fopen(const char *restrict pathname, const char *restrict type);

/**
 * @brief 打开一个文件流
 *
 * <stdio.h>
 *
 * 在一个指定的流上打开一个指定的文件, 如若该流已经打开, 则先关闭该流.
 * 若该流已经定向, 则使用 freopen() 清除该定向.
 * 此函数一般用于将一个指定的文件打开为一个预定义的流: 标准输入, 标准输出或标准错误.
 *
 * @param pathname
 * @param type
 * @param fp
 *
 * @return
 *
 * @see open()
 * @see fopen()
 * @see fdopen()
 */
FILE *
fropen(const char *restrict pathname, const char *restrict type,
        FILE *restrict fp);

/**
 * @brief 关闭打开的流
 *
 * @param fp 指定要关闭的流
 *
 * @return 返回函数执行状态
 * @retval 0 函数执行成功
 * @retval EOF 出错
 */
int
fclose(FILE *fp);
