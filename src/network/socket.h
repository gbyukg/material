/**
 * @file socket.h
 * @author Lock
 * @date 29 Dec 2015
 * @brief 网络编程相关函数
 *
 * @image html socket_connect.png
 *
 */

/**
 * @example unixDomainStreamClient.c
 * @example unixDomainStreamServer.c
 *
 * UNIX domain 中的流 socket
 */

#include <netdb.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <sys/socket.h>

/**
 * @def AF_UNIX
 * <sys/socket.h>
 *
 * 允许同一主机上的应用程序之间进行通讯, 地址格式为路径名.
 *
 * 地址结构: @ref sockaddr_un
 *
 * @see http://lxr.free-electrons.com/source/include/linux/socket.h#L161
*/
#define AF_UNIX 1

/**
 * @def AF_INET
 * <sys/socket.h>
 *
 * 使用 IPv4 连接.
 *
 * 地址结构: @ref sockaddr_in
 *
 * @see http://lxr.free-electrons.com/source/include/linux/socket.h#L163
*/
#define AF_INET 2

/**
 * @def AF_INET6
 * <sys/socket.h>
 *
 * 使用 IPv6 连接.
 *
 * 地址结构: @ref sockaddr_in6
 *
 * @see http://lxr.free-electrons.com/source/include/linux/socket.h#L171
*/
#define AF_INET6 10

/**
 * @def SOCK_STREAM
 * @brief 流连接方式
 *
 * <sys/socket.h>
 *
 * 流 socket 提供了一个可靠的双向的字节流通信信道.
 *
 * @see
 *
*/
#define	SOCK_STREAM	1

/**
 * @def SOCK_DGRAM
 * @brief 数据报 socket
 *
 * <sys/socket.h>
 *
*/
#define	SOCK_DGRAM	2

/**
 * @def SOMAXCONN
 * @brief 系统中允许未决连接的最大数量
 *
 * <sys/socket.h>
 *
 * @see http://lxr.free-electrons.com/source/include/linux/socket.h#L252
 */
#define	SOMAXCONN	128

/**
 * @brief 通用地址结构
 *
 * 由于每种 socket domain 都使用了不同的地址格式. 如 UNIX domain socket 使用路径名,
 * 而 Inetnet domain socket 使用了 IP 地址和端口号. 对于各种 socket domain
 * 都需要定义一个不同的结构类型来存储 socket 地址. 而有些函数为了能够处理所有这些
 * 类型的 domain, 只接受一种参数, 所以定义了该模板. 而在具体处理指定 socket domain
 * 的时候, 需要具体转换成特定类型的 socket domain addr.
 *
 * @see sockaddr_un
 * @see sockaddr_in
 * @see sockaddr_in6
 * @see http://lxr.free-electrons.com/source/include/linux/socket.h#L29
 */
struct sockaddr {
    sa_family_t sa_family;
    char        sa_data[14];
};

/**
 * @brief UNIX domain 地址结构
 *
 * 在 UNIX domain 中, socket 地址以路径名来表示.
 *
 * 通过将 sun_path 的第一个字节指定为 null 字节(\0), 可以创建一个抽象绑定,
 * 抽象绑定并不会真正的在文件系统中创建 socket 文件. 优势:
 *   - 无需担心与文件系统中的即有名字产生冲突
 *   - 无需在使用完 socket 后手动删除 socket 文件. 当 socket 关闭后, 会自动删除
 *   这个抽象名
 *   - 无需为 socket 创建一个文件系统路径名了
 *
 * @code{.c}
 * const char *SOCKNAME = "/tmp/mysock";
 * sockaddr_un addr;
 * memset(&addr, 0, sizeof(sockaddr_un));
 * addr.sun_family = AF_UNIX;
 * // 长度为 sun_path 的总长度 - 1, 至少预留一个用于保存结尾的空字符
 * strncpy(addr.sun_path, SOCKNAME, sizeof(addr.sun_path) - 1);
 * @endcode
 *
 * @note SUSv3 并没有规定 sun_path 字段的大小. 早期的 BSD 实现使用 108 和 104
 * 字节, 而现代一点的实现则使用了 92 个字节. 所以为了编写可移植行的程序, 应该
 * 采用最小值. 并且在想这个字段写入数据时使用 [snprintf()](http://man7.org/linux/man-pages/man3/printf.3.html)
 * 或 [strncpy()](http://man7.org/linux/man-pages/man3/strncpy.3.html)
 * 以避免缓冲区溢出.
 *
 * @see http://lxr.free-electrons.com/source/include/uapi/linux/un.h#L8
 */
struct sockaddr_un {
    sa_family_t sun_family; //!< 总是 AF_UNIX
    char        sun_path[102];     //!< 以空指针结尾的存放 socket 路径名路径字符串
};

/**
 * @brief IPv4 地址存储结构
 *
 * IPv4 地址结构 sockaddr_in 中存储IP地址, 保存的是二进制.
 *
 * 可使用 inet_pton() 与 inet_ntop() 函数将 IP 地址在二进制与十进制(人类可读的,用 . 分割)形式之间进行转换
 *
 * @see http://lxr.free-electrons.com/source/include/uapi/linux/in.h#L84
 */
struct in_addr {
    in_addr_t s_addr; //!< 无符号 4字节的 32 位整数
};

/**
 * @brief IPv4 地址结构
 *
 * 定义在 <netinet/in.h> 头文件中
 *
 * @see http://lxr.free-electrons.com/source/include/uapi/linux/in.h#L230
 */
struct sockaddr_in {
    sa_family_t    sin_family; //!< 总是 AF_INET
    in_port_t      sin_port;   //!< 16 位端口号
    struct in_addr sin_addr;   //!< IPv4 地址, 指向 in_addr 结构
};

/**
 * @brief 用于 IPv6 IP 地址.
 *
 * @see http://lxr.free-electrons.com/source/include/uapi/linux/in6.h#L32
 */
struct in6_addr {
    unit8_t s6_addr[16]; //!< 16 个字节, 等于 128 位
};

/**
 * @brief IPv6 地址结构
 *
 * 定义在 <netinet/in.h> 头文件中
 *
 * @see http://lxr.free-electrons.com/source/include/uapi/linux/in6.h#L49
 */
struct sockaddr_in6 {
    sa_family_t     sin6_family;   //!< 总是 AF_INET6
    in_port_t       sin6_port;     //!< 16 位端口号
    unit32_t        sin6_flowinfo; //!< IPv6 flow 信息
    struct in6_addr sin6_addr;     //!< IPv6 地址结构
    unit32_t        sin6_scop_id;  //!< Scope ID
};

/**
 * @brief 开始接受客户端的请求
 *
 * 在文件描述符 sockfd 引用的监听流 socket 上接受一个接入连接. 如果在调用 accept() 时不存在未决的连接,
 * 那么调用就会阻塞直到有连接请求到达为止.
 *
 * accept() 函数会创建一个新 socket, 并且正是这个新 socket 会与执行 connect() 的对等socket进行了连接.
 *
 * @param sockfd 通过调用 socket() 函数获得的 socket 文件描述符
 * @param addr 用于返回客户端的地址信息, 根据 socket domain的不同,
 * 可能是 @ref sockaddr_un, @ref sockaddr_in, @ref sockaddr_in6 其中的一个,
 * 如果不关心客户端地址信息, 可以设置 NULL.
 * @param addrlen @p addr 地址结构的大小. 如果不关心客户端地址, 设置为 0.
 *
 * @return 返回一个 socket 文件描述符用来表示与客户端的连接
 * @retval -1 失败
 *
 * @see http://man7.org/linux/man-pages/man2/accept.2.html
 */
int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

/**
 * @brief 将 socket 绑定到地址
 *
 * 一般来讲, 将服务器的地址绑定到一个众所周知的 IP 上--即一个固定的与服务器通信的客户端提前就应该直到服务器的 IP 地址.
 *
 * 当用来绑定 UNIX domain socket 时, bind() 会在文件系统中创建一个条目. 文件所有权
 * 将根据常规的文件创建规则来设定文件权限. 这个文件会被标记为一个 socket,
 * 在 stat 结构的 st_mode 字段中的文件类型部分返回值 S_IFSOCK.
 *
 * 无法将 socket 绑定到一个已有的路径上面, 否则 bind() 失败并返回 EADDRINUSE 错误.
 *
 * @param sockfd 通过调用 socket() 函数获得的 socket 文件描述符
 * @param addr 指向一个该 socket 绑定到的地址结构. 传入这个参数的结构取决于
 * socket domain.
 * @param addrlen 指定 @p addr 结构的大小
 *
 * @return 返回函数执行状态
 * @retval 0 成功
 * @retval -1 失败
 *
 * 示例:
 * @code{.c}
 * struct sockaddr_un addr;
 * bind (sfd, (const struct sockaddr *)addr, sizeof(struct sockaddr_un));
 * @endcode
 *
 * @see socket()
 * @see http://man7.org/linux/man-pages/man2/bind.2.html
 */
int
bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

/**
 * @brief 连接到服务器端
 *
 * connect() 系统调用将文件描述符 sockfd 引用的主动 socket 连接到地址通过 @p addr
 * 和 @p addrlen 指定的监听 socket 上.
 *
 * @param sockfd 通过调用 socket() 函数获得的 socket 文件描述符
 * @param addr 指向一个该 socket 绑定到的地址结构. 传入这个参数的结构取决于
 * socket domain.
 * @param addrlen 指定 @p addr 结构的大小
 *
 * @return 返回函数执行状态
 * @retval 0 成功
 * @retval -1 失败
 *
 * 示例:
 * @code{.c}
 * struct sockaddr_in addr;
 * memset(&addr, 0, sizeof(struct sockaddr_in));
 * // 返回对端 socket 的 IPv4 地址结构
 * connect(sfd, (struct sockaddr_in *)&addr, sizeof(struct sockaddr_in));
 * @endcode
 *
 * @see http://man7.org/linux/man-pages/man2/connect.2.html
 */
int
connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

/**
 * @brief 将 socket 引用的流 socket 标记为被动.
 *
 * 通过 socket() 创建的 socket 是主动的(客户端). 通过该函数将 socket 更改为 被动式的(服务器).
 *
 * @param sockfd 通过调用 socket() 函数获得的 socket 文件描述符
 * @param backlog 一个客户端完全有可能在服务器调用 accept() 之前调用 connect() 函数向服务器发送一个连接请求, 此时服务器无法处理这个客户端发来的连接,
 * 这种连接被称为未决的连接(pending connection).
 * 服务器会将未决连接保存起来, 待服务器之后处理. backlog 就是用来设置可以保存未决连接的最大数量.
 * 在这个限制之内的连接请求会立即成功. 之外的连接请求就会阻塞知道一个未决的连接被接受(通过 accept()), 并从未决连接队列删除为止.
 * @ref SOMAXCONN 常量定义了系统中允许接受的未决连接的最大数量. 但是也可以通过 /proc/sys/net/core/somaxconn 文件来调整这个限制.
 *
 * @return 返回函数执行状态
 * @retval 0 成功
 * @retval -1 失败
 *
 * @note 无法在一个已经连接的 socket 上(即已经成功执行 connect() 的 socket 或者由 accept() 调用返回的 socket)执行 listen()
 *
 * @see http://man7.org/linux/man-pages/man2/listen.2.html
 */
int
listen(int sockfd, int backlog);

/**
 * @brief 接收数据报 socket 上的消息
 *
 * @param sockfd 通过调用 socket() 函数获得的 socket 文件描述符
 * @param buffer 数据接收缓冲区
 * @param length 缓冲区长度
 * @param flag
 * @param src_addr 用于返回客户端的地址信息, 根据 socket domain的不同,
 * 可能是 @ref sockaddr_un, @ref sockaddr_in, @ref sockaddr_in6 其中的一个,
 * 如果不关心客户端地址信息, 可以设置 NULL.
 * @param addrlen @p addr 地址结构的大小. 如果不关心客户端地址, 设置为 0.
 *
 * @return 返回获取到的字节数
 * @retval 0 EOF, 文件末尾
 * @retval -1 失败
 *
 * @see sendto()
 * @see http://man7.org/linux/man-pages/man2/recvfrom.2.html
 */
ssize_t
recvfrom(int sockfd,
        void *buffer,
        size_t length,
        int flag,
        struct sockaddr *src_addr,
        socklen_t *addrlen);

/**
 * @brief 向数据报 socket 中发送数据
 *
 * @param sockfd 通过调用 socket() 函数获得的 socket 文件描述符
 * @param dest_addr 用于指定服务器端的地址信息, 根据 socket domain的不同,
 * 可能是 @ref sockaddr_un, @ref sockaddr_in, @ref sockaddr_in6 其中的一个,
 * @param addrlen @p dest_addr 大小
 *
 * @return 返回发送的字节数
 * @retval -1 失败
 *
 * @see recvfrom()
 * @see http://man7.org/linux/man-pages/man2/sendto.2.html
 */
ssize_t
sendto(int sockfd,
        const void *buffer,
        size_t length,
        int flags,
        const struct sockaddr *dest_addr,
        socklen_t addrlen);

/**
 * @brief 创建一个 socket.
 *
 * @param domain 指定 socket 的通信domain, 可选值:
 *   - @ref AF_UNIX 在同一主机上通过路径名进行通信. 地址结构 @ref sockaddr_un
 *   - @ref AF_INET 通过 IPv4 网络连接起来的主机. 32 位 IPv4地址 + 16 位端口号. 地址结构 sockaddr_in
 *   - @ref AF_INET6 通过 IPv6 网络连接起来的主机. 128 位 IPv4地址 + 16 位端口号. 地址结构 sockaddr_in6
 * @param type 指定 socket 类型, 可选值:
 *   - @ref SOCK_STREAM(数据流 socket TCP)
 *     - 可靠的: 可以保证发送者传输的数据会完整无缺地达到接受应用程序.
 *     - 双向的: 数据可以在两个 socket 之间任意方向上传送.
 *     - 字节流: 表示与管道一样不存在消息边界的概念.
 *   - @ref SOCK_DGRAM(数据报 socket UDP) 消息边界得到了保留, 但是数据传输是不可靠的. 消息的到达可能是无序的,重复的或者根本无法到达.
 * @param protocol 本书中一直默认使用0
 * @return 在成功时返回一个引用在后续系统调用中会用到的新创建的 socket 文件描述符
 * @retval n 大于等于 0 的文件描述符
 * @retval -1 执行失败
 *
 * 示例:
 * @code{.c}
 * int sfd = socket(AF_UNIX, SOCKET_STREAM, 0); // 创建TCP 类型的 UNIX domain socket
 * int sfd = socket(AF_INET, SOCKET_STREAM, 0); // 创建 IPv4 domain socket
 * int sfd = socket(AF_INET6, SOCKET_STREAM, 0); // 创建 IPv6 domain socket
 * @endcode
 *
 * @note 在一些代码中可能会看到 PF_UNIX 而不是 AF_UNIX 常量.
 * 在这种上下文中, AF 表示 "地址族(address famili)", PF 表示 "协议族(protocol family)".
 * 在一开始的时候, 设计人员相信单个协议族可以之多多个协议族, 即 PF 可以包含 AF_UNIX, AF_INET 和 AF_INET6 三个协议族.
 * 但现实是残酷的, 满足不了需求. 所以将现有的 PF_ 常量定义成对应的 AF_ 常量.
 *
 * @see http://man7.org/linux/man-pages/man2/socket.2.html
 */
int
socket(int domain, int type, int protocol);

/**
 * @include socket.c
 *
 * include a file
 */

