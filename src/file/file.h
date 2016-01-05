/**
 * @file file.h
 * @author Lock
 * @date 29 Dec 2015
 * @brief 文件系统 I/O 相关函数
 *
 * @image html filesystem.png
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
 *   - `O_DSYNC`
 *   - `O_NONBLOCK` 打开文件时设置该文件为非阻塞模式.
 *     - 当 open() 调用不能立即打开文件, 则返回错误, 而非陷入阻塞.
 *     - 调用 open() 成功后, 后续的 I/O 操作也是非阻塞的. 若 I/O 系统调用未能立即完成,
 *     则可能会只传输部分数据, 或者系统调用失败, 并返回 `EAGAIN` 或 `EWOULDBLOCK` 错误.
 *   - `O_SYNC` 使每次 write 等待物理I/O操作完成, 包括由该 write 操作引起的文件属性更新所需的I/O.
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
 */
int
write(int fd, void *buffer, size_t count);
