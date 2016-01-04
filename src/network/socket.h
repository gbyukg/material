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
 * @example unixDomainDgramClient.c
 * @example unixDomainDgramServer.c
 * @example inetDomainDgramClient.c
 * @example inetDomainDgramServer.c
 * @example getIpAddress.c
 * @example test.c
 * @example test_client.c
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
 * @brief IPv4 通配地址
 *
 * 如果位于一台多宿主机上的应用程序只将 socket 绑定到其中一个主机 IP 地址上,
 * 那么该 socket 只能接受发送到该 IP 地址上的 UDP 数据报或 TCP 连接请求,
 * 如果想要能够接收任意一个 IP 地址的连接, 将 socket 绑定到统配 IP 地址上即可.
 *
 * 大多数实现将其定义成了 0.0.0.0
 *
 * @see http://lxr.free-electrons.com/source/include/uapi/linux/in.h#L273
 */
#define INADDR_ANY              ((unsigned long int) 0x00000000)

/**
 * @brief IPv4 回环地址
 *
 * 127.0.0.1 一般被设定为回环地址(loopback), 网络中127.0.0.1/8 中的所有地址都可以
 *被指定成 IPv4 的回环地址
 *
 * @see http://lxr.free-electrons.com/source/include/uapi/linux/in.h#L285
 */
#define INADDR_LOOPBACK         0x7f000001      /* 127.0.0.1   */

/**
 * @brief IPv4 字符串展现形式的最大长度
 *
 * 可用于 inet_ntop() 函数
 *
 * @see http://lxr.free-electrons.com/source/include/linux/inet.h#L51
 */
#define INET_ADDRSTRLEN         (16)

/**
 * @brief IPv6 字符串展现形式的最大长度
 *
 * 可用于 inet_ntop() 函数
 *
 * @see http://lxr.free-electrons.com/source/include/linux/inet.h#L52
 */
#define INET6_ADDRSTRLEN        (48)

/**
 * @def SOCK_STREAM
 * @brief 流连接方式
 *
 * <sys/socket.h>
 *
 * 流 socket 提供了一个可靠的双向的字节流通信信道, 这表示:
 *   - 可靠的: 可以保证发送者传输的数据会完整无缺地达到接受应用程序.
 *   - 双向的: 数据可以在两个 socket 之间任意方向上传送.
 *   - 字节流: 表示与管道一样不存在消息边界的概念.
 *
 * 流 socket 的正常工作需要一对相互连接的 socket, 因此流 socket
 * 通常被称为面向连接的. 一个流 socket 只能与一个对等的 socket 进行连接
*/
#define	SOCK_STREAM	1

/**
 * @def SOCK_DGRAM
 * @brief 数据报 socket
 *
 * <sys/socket.h>
 *
 * 数据报 socket 允许数据被称为数据报的消息的形式进行交换.
 * 在数据报 socket 中, 消息边界得到了保留, 但在网络中, 数据的传输是不可靠的,
 * 消息的到达是无序的, 重复的或者根本无法到达.
 *
 * 但是对于 UNXI domain 来说确实可靠的, 因为数据的传输发生在内核.
 * 所有的消息都会按发送顺序到达, 并且不会发生重复.
 *
 * 同时数据报 socket 更是一种无连接 socket, 与流 socket 连接不同, 一个数据报
 * socket 在使用时, 无需与另一个 socket 连接.
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
 * @brief 保存地址结构信息
 *
 * 用于设置和保存 getaddrinfo() 函数根据主机名和服务器名获取到的地址结构信息
 *
 * <netdb.h>
 */
struct addrinfo {
    int    ai_flags;        //!< 在获取地址结构信息使用
    int    ai_family;       //!< @ref AF_INET 或 @ref AF_INET6
    int    ai_socktype;     //!< @ref SOCK_STREAM 或 @ref SOCK_DGRAM
    int    ai_protocol;
    size_t ai_addrlen;      //!< ai_addr 指向的 socket 地址结构的大小
    char   *ai_canonname;
    struct sockaddr *ai_addr; //!< socket 地址结构, IPv4 时指向 @ref in_addr, IPv6 时指向 @ref in6_addr
    struct addrinfo *ai_next; //!< 用于指向下一个 @ref addrinfo 结构
};

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
    sa_family_t sun_family;     //!< 总是 AF_UNIX
    char        sun_path[102];  //!< 以空指针结尾的存放 socket 路径名路径字符串
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
    in_port_t      sin_port;   //!< 16 位端口号, 需要转换成网络字节序
    struct in_addr sin_addr;   //!< IPv4 地址, 指向 in_addr 结构, 需要转换成网络字节序
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
    in_port_t       sin6_port;     //!< 16 位端口号, 需要转换成网络字节序
    unit32_t        sin6_flowinfo; //!< IPv6 flow 信息
    struct in6_addr sin6_addr;     //!< IPv6 地址结构, 需要转换成网络字节序
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
 * @see bind()
 * @see connect()
 * @see listen()
 * @see socket()
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
 * @see accept()
 * @see connect()
 * @see listen()
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
 * @see accept()
 * @see bind()
 * @see listen()
 * @see socket()
 * @see http://man7.org/linux/man-pages/man2/connect.2.html
 */
int
connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

/**
 * @brief 释放 addrinfo 列表
 *
 * getaddrinfo() 函数会动态地为 result 引用的所有结构分配内存, 其结构是调用者
 * 必须要在不再需要这些结构时释放他们.
 */
void
freeaddrinfo(struct addrinfo *resutl);

/**
 * @brief 为函数 getaddrinfo() 提供错误诊断
 *
 * 当 getaddrinfo() 函数在发生错误时会返回一个错误列表, 用来标明错误原因.
 *   - `EAI_ADDRFAMILY`: 在 `hints.ai_family` 中不存在 host 的地址.
 *   - `EAI_AGAIN`: 名字解析过程中发生临时错误
 *   - `EAI_BADFLAGS`: 在 `hints.ai_family` 中指定了一个无效的标记
 *   - `EAI_FAIL`: 访问名字服务器时发生了无法恢复的故障
 *   - `EAI_FAMILY`: 不支持在 `hints.ai_family` 中指定的地址族
 *   - `EAI_MEMORY`: 内存分配故障
 *   - `EAI_NODATA`: 没有与 host 关联的地址中规定, 但大多数实现都对其进行了定义
 *   - `EAI_NONAME`: 未知的 host 或 service, 或者 host 和 service 都为 NULL,
 *   或者指定了 `AI_NUMERICSERV` 同时 `service` 没有指向一个数值字符串
 *   - `EAI_OVERFLOW`: 参数缓冲区溢出
 *   - `EAI_SERVICE`: `hints.ai_socktype` 不支持指定的 service
 *   - `EAI_SOCKTYPE`: 不支持指定的`hints.ai_socktype`
 *   - `EAI_SYSTEM`: 不支持指定的 `hints.ai_socktype`
 */
const char
*gai_strerror(int errcode);

/**
 * @brief 根据主机名获取地址结构列表
 *
 * 将获取到的地址结构列表保存到 @p result 参数中,
 * 需要注意的是, @p result 是一个结构列表而不是单个结构.
 *
 * @param host 包含一个主机名或一个以 IPv4 点分十进制标记或 IPv6 十六进制字符串
 * 标记的数值地址字符串.
 * @param service 包含一个服务名或一个十进制端口号
 * @param hints 指向一个 @ref addrinfo 结构, 该结构规定了选择通过 result 返回的
 * socket 地址结构的标准, 此时只能设置 @ref addrinfo 结构中的 `ai_flags`,
 * `ai_family`, `ai_socktpe` 以及 `ai_protocol` 字段.<BR>
 * `hints.ai_family` 字段指定了使用返回的 socket 地址结构的域, 其取值可以使
 * @ref AF_INET 或 @ref AF_INET6. 如果需要获取所有种类 socket 地址结构,
 * 那么可以将这个字段的值指定为 `AF_UNSPEC`.<BR>
 * `hints.ai_socktype` 字段指定了使用返回的 socket 地址结构的 socket 类型,
 * 可使用的值有: @ref SOCK_DGRAM, 表示查询将会在 UDP 服务上执行,
 * @ref SOCK_STREAM, 表示查询将会 TCP 服务上执行,
 * 或者指定为0, 表示任意类型的 socket 都可以接受.<BR>
 * `hints.ai_flags` 是一个位掩码, 用于改变 getaddrinfo() 的行为. 是下面中的一个
 * 或多个取 OR 的来的.
 *   - `AI_ADDRCONFIG`
 *   - `AI_ALL`
 *   - `AI_CANONNAME`
 *   - `AI_NUMERICHOST`: 强制将 host 解释成一个数值地址字符串. 这个常量用于在
 *   不必要解析名字时防止进行名字解析.
 *   - `AI_NUMERICSERV`: 强制将 service 解释成一个数值端口号. 这个标记用于防止
 *   调用任意的名字解析服务器, 因为当 service 为一个数值字符串时种种调用是没有
 *   必要的.
 *   - `AI_PASSIVE`: 如果 host 为 NULL, 则返回一个适合被动式打开的 socket 地址
 *   结构. 通过 @p result 返回的 socket 地址结构的 IP 地址部分将会包含一个通配
 *   IP 地址(即 @ref INADDR_ANY 或 @ref IN6ADDR_ANY). <BR>
 *   如果没有设置这个标记, 那么通过 @p result 返回的地址结构将能用于
 *   connect() 和 sendto(); 如果 host 为 NULL, 那么返回的 socket 地址结构中的
 *   IP 地址将会被设置成回环 IP 地址.
 *
 * @param result 获取一个动态分配的包含 @ref addrinfo 结构的链表.
 *
 * @return 返回函数执行状态
 * @retval 0 成功
 * @retval 非0 失败
 *
 * @note 由于 @p result 是动态分配, 使用完之后需要 调用 freeaddrinfo()
 * 函数手动释放.
 *
 * @see freeaddrinfo
 */
int
getaddrinfo(const char *host,
        const char *service,
        const struct addrinfo *hints,
        struct addrinfo **result);

/**
 * @brief 返回本地套接字地址
 *
 * @param sockfd 套接字文件描述符
 * @param addr 指向 sockaddr 结构体的指针, 而结构体包含着套接字的地址.
 * @param addrlen 指向用于保存 @p addr 大小的指针, 在函数执行成功后用于保存
 * @p addrlen 实际的大小
 *
 * @return 返回函数执行状态
 * @retval 0 成功
 * @retval 非0 失败
 */
int
getsockname(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

/**
 * @brief 返回对端套接字地址
 *
 * @param sockfd 套接字文件描述符
 * @param addr 指向 sockaddr 结构体的指针, 而结构体包含着套接字的地址.
 * @param addrlen 指向用于保存 @p addr 大小的指针, 在函数执行成功后用于保存
 * @p addrlen 实际的大小
 *
 * @return 返回函数执行状态
 * @retval 0 成功
 * @retval 非0 失败
 */
int
getpeername(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

/**
 * @brief 主机字节序转换成网络字节序
 *
 * 由于 CPU 存在大小端模式, 不同的主机对字节的存放顺序有可能不同.
 *   - 主机字节序: 在特定住自己上使用的字节序
 *   - 网络字节序: 在网络中使用的统一的字节序, 大端模式
 *
 * 由于端口号和 IP 地址必须在网络中的所有主机之间传递并且需要被理解,
 * 因此在将端口和 IP 存入 socket 地址结构之前必须要使用统一的网络字节序进行转换.
 *
 * 将 16 位的主机字节序转换成 16 位的网络字节序
 *
 * @return 返回 16 位的网络字节序
 *
 * @see htonl()
 * @see ntohs()
 * @see ntohl()
 */
uint16_t
htons(uint16_t host_uint16);

/**
 * @brief 主机字节序转换成网络字节序
 *
 * 将 32 位的主机字节序转换成 32 位的网络字节序
 *
 * @return 返回 32 位的网络字节序
 *
 * @see htons()
 * @see ntohs()
 * @see ntohl()
 */
uint32_t
htonl(uint32_t host_uint32);

/**
 * @brief IP 转 二进制
 *
 * 将十进制点分割的 IP 地址转换成网络字节序的二进制 IP 地址, p 表示展现(presentation),
 * n 表示网络(network)
 *
 * @param domain 可用参数为 @ref AF_INET 或 @ref AF_INET6
 * @param src_str 以字符串形式展示的 十进制点分隔 的 IP 地址
 * @param addrptr 针对 @p domain 参数指定的值来指向一个 @ref in_addr 结构或
 * @ref in6_addr 结构
 *
 * @return 判定函数是否执行成功
 * @retval 1 成功
 * @retval 0 @p src_str 参数格式不正确
 * @retval -1 函数执行失败
 *
 * @see inet_ntop()
 */
int
inet_pton(int domain, const char *src_str, void *addrptr);

/**
 * @brief 二进制转 IP
 *
 * 将二进制的网络字节序的 IP 地址转换成十进制点分隔的 IP 地址.
 *
 * @param domain 可用参数为 @ref AF_INET 或 @ref AF_INET6
 * @param addrptr 针对 @p domain 参数指定的值来指向一个 @ref in_addr 结构或
 * @ref in6_addr 结构
 * @param dst_str 用于保存得到的以 NULL 结尾的十进制点分隔的 IP 地址的字符串
 * @param len 指定 @p dst_str 缓冲区的大小. 如果该值太小, 则返回 NULL,
 * 并将errno 设置成 ENOSPC, 可以使用预定义好的两个常量 @ref INET_ADDRSTRLEN
 * 和 @ref INET6_ADDRSTRLEN, 这两个常量标示出了 IPv4 和 IPv6 地址的展现字符串
 *的最大长度.
 *
 * @return 返回一个指向 展现形式的IP地址 的指针
 * @retval NULL 函数执行失败
 *
 * @see inet_ntop()
 */
const char *
inet_ntop(int domain,
        const void *addrptr,
        char *dst_str,
        size_t len);

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
 * @see accept()
 * @see bind()
 * @see connect()
 * @see socket()
 * @see http://man7.org/linux/man-pages/man2/listen.2.html
 */
int
listen(int sockfd, int backlog);

/**
 * @brief 网络字节序转换成主机字节序
 *
 * 将 16 位的网络字节序转换成 16 位的主机字节序
 *
 * @return 返回 16 位的主机字节序
 *
 * @see htons()
 * @see htonl()
 * @see ntohl()
 */
uint16_t
ntohs(uint16_t net_uint16);

/**
 * @brief 网络字节序转换成主机字节序
 *
 * 将 32 位的网络字节序转换成 16 位的主机字节序
 *
 * @return 返回 32 位的主机字节序
 *
 * @see htons()
 * @see htonl()
 * @see ntohs()
 */
uint32_t
ntohl(uint32_t net_uint16);

/**
 * @brief 专用于套接字的系统调用
 *
 * @param sockfd
 * @param buffer
 * @param length
 * @param flags 用于修改 I/O 操作的行为, 可选值有:
 *   - `MSG_DONTWAIT`: 让 recv() 以非阻塞方式执行. 如果没有数据可用, 那么 recv()
 *   不会阻塞而是立刻返回, 伴随的错误码为 `EAGAIN`. 可以通过 fcntl() 把套接字设置为覅阻塞模式(O_NONBLOCK)从而达到相同的目的,
 *   区别在于 `MSG_DONTWAIT` 允许我们在每次调用中控制非阻塞行为.
 *   - `MSG_OOB`: 在套接字上接受带外数据
 *   - `MSG_PEEK`: 从套接字缓冲区中获取一份请求字节的副本, 但不会将请求的字节从缓冲区中实际溢出.
 *   这份数据稍后可以由其他的 recv() 或 read() 调用重新读取.
 *   - `MSG_WAITALL`: 通常, recv() 调用返回的字节数比请求的字节数(@p length 参数指定)要少,
 *   而那些字节实际上还在套接字中. 指定了 `MSG_WAITALL` 标记后将导致系统调用阻塞,
 *   直到成功接受到 length 个字节. 但是, 就算指定了该标记, 在遇到下面情况时,
 *   返回的字节数仍然可能会小于 @p length 长度, 情况为:
 *     - 捕捉到一个信号
 *     - 连接流的对端终止了连接
 *     - 遇到了带外数据字节
 *     - 从数据报套接字接收到的消息长度小于 @p length 长度字节
 *     - 套接字上出现了错误
 *
 * @return 返回获取到的字节数
 * @retval 0 文件结尾 EOF
 * @retval -1 函数执行失败
 */
ssize_t
recv(int sockfd, void *buffer, size_t length, int flags);

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
 * @brief 专用于套接字的系统调用
 *
 * @param sockfd
 * @param buffer
 * @param length
 * @param flags 可选值:
 *   - `MSG_DONTWAIT`: 让 send() 以非阻塞方式执行. 如果数据不能立刻传输(因为套接字发送缓冲区已满),
 *   那么该调用不会阻塞, 而是调用失败, 伴随的错误码为 EAGAIN.
 *   - `MSG_MORE` 在 TCP 套接字上, 这个标记实现的效果同套接字选项 TCP_SORK 完成的功能相同.
 *   区别在于该标记可以在每次调用中对数据进行栓塞处理.<BR>
 *   如果该选项应用于数据报套接字上时, 在连续的 send() 或 sendto() 调用中传输的数据,
 *   数据将被打包成一个单独的数据报. 仅当下一次调用中没有指定该标记时数据才会被传输出去.
 *   `MSG_MORE` 标记对 UNIX 域套接字没有任何效果.
 *   - `MSG_NOSIGNAL`: 当在已连接的流式套接字上发送数据时, 如果连接的另一端已经关闭了,
 *   指定该标记后将不会产生 SIGPIPE 信号. 想法, send() 调用将会失败,
 *   伴随的错误码为 EPIPE. 这和忽略 SIGPIPE 信号所得到的行为相同.
 *   区别在于该标记可以在每次调用中控制信号发送的行为.
 *
 * @return 返回发送出去的字节数
 * @retval -1 函数执行失败
 *
 * @see recv
 */
ssize_t
send(int sockfd, const void *buffer, size_t length, int flags);

/**
 * @brief 通过零拷贝的方式在网络中传输文件
 *
 * 包含在 <sys/sendfile.h>
 *
 * 通过 read() 和 write() 函数完全可以达到文件传输的目的, 但是如果是传输大的文件,
 * 这种方法就会显得非常不高效, 系统首先需要通过 read() 将文件内容从缓冲区缓存写入用户空间缓冲区,
 * 在通过 write() 函数从用户缓冲区写到内核空间, 以此才能通过套接字进行传输.
 * 如果应用程序在发起传输之前根本不对文件内容做任何处理的话, 那么这种两步式的处理就是一种浪费.<BR>
 * sendfiel() 就是为了防止这种多余的将文件在用户空间和内核空间进行多余的复制操作而诞生的.
 * @image html sendfile.png
 *
 * @param out_fd 指向一个套接字
 * @param in_fd 被输出的文件描述符, 该文件描述符必须是可以进行 mmap() 操作的.
 * @param offset 如果参数 @p offset 不为 NULL, 他应该指向一个 off_t 值, 该值指定了起始文件的偏移量,
 * 意即从 @p in_fd 指向的文件的这个位置开始, 可以传输字节. 它不会更改 @p in_fd 的偏移量.<BR>
 * 如果该参数为 NULL, 那么从 @p in_fd 传输的字节就从当前文件偏移量处开始,
 * 且在传输时会更新文件偏移量以反映出已传输的字节数.
 * @param count 指定要传输的字节总数, 如果被传输的文件总长度小于 @p count,
 * 函数将返回实际被传输的字节数.
 *
 * @return 返回被传输的字节数
 * @retval -1 函数执行失败
 *
 * @note 不能通过该函数在两个套接字之间传送数据
 */
ssize_t
sendfile(int out_fd, int in_fd, off_t *offset, size_t count);

/**
 * @brief 向数据报 socket 中发送数据
 *
 * @param sockfd 通过调用 socket() 函数获得的 socket 文件描述符
 * @param buffer
 * @param length
 * @param flags
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
 * @brief 关闭 socket 连接
 *
 * 在调节子上调用 close() 会将双向通道的两端都关闭. 有时候, 只关闭连接的一端也是有用处的,
 * 这样数据只能在一个方向上通过套接字传输.
 *
 * 与 close() 不同, 该函数是基于文件的, 而不是文件描述符.
 * @code{.c}
 * fd2 = dup(sockfd);
 * close(fd2); // fd2 依然处于连接状态.
 *
 * shutdown(sockfd, SHUT_RDWR); // fd2 也会被关闭.
 * @endcode
 *
 * @param sockfd 通过调用 socket() 函数获得的 socket 文件描述符
 * @param how 设置如何关闭 socket, 可选值有:
 *   - `SHUT_RD`: 关闭连接的读端. 之后的读操作将返回文件结尾(0).
 *   数据仍然可以写入到套接字上.
 *   - `SHUT_WR`: 关闭连接的写端. 一旦对端的应用程序已经将所有剩余的数据读取完毕,
 *   它就会检测到文件结尾. 后续对本地套接字的操作将产生 SIGPIPE 信号以及 EPIPE
 *   错误. 而由对端写入的数据仍然可以在套接字上读取. 换句话说, 这个操作允许我们在仍然能读取对端发回给我们的数据,
 *   通过文文件结尾来通知对端应用程序本地的写断已经关闭了.
 *   - `SHUT_RDWR`: 将连接的读端和写端都不安比. 这等同于先执行 `SHUT_RA`,
 *   跟着在执行一次 `SHUT_WR`.
 *
 * @return 返回一个确定函数是否执行成功的数值
 * @retval 0 函数执行成功
 * @retval -1 函数执行失败
 *
 * @note shutdown() 函数并不会关闭文件描述符, 就算参数 @p how 指定为 `SHUTO_RDWR`,
 * 要关闭文件描述符, 必须另外调用 close().
 */
int
shutdown(int sockfd, int how);

/**
 * @brief 创建一个 socket.
 *
 * @param domain 指定 socket 的通信domain, 可选值:
 *   - @ref AF_UNIX 在同一主机上通过路径名进行通信. 地址结构 @ref sockaddr_un
 *   - @ref AF_INET 通过 IPv4 网络连接起来的主机. 32 位 IPv4地址 + 16 位端口号. 地址结构 sockaddr_in
 *   - @ref AF_INET6 通过 IPv6 网络连接起来的主机. 128 位 IPv4地址 + 16 位端口号. 地址结构 sockaddr_in6
 * @param type 指定 socket 类型, 可选值:
 *   - @ref SOCK_STREAM(数据流 socket TCP)
 *   - @ref SOCK_DGRAM(数据报 socket UDP) 消息边界得到了保留, 但是数据传输是不可靠的. 消息的到达可能是无序的,重复的或者根本无法到达.
 * @param protocol 本书中一直默认使用0
 *
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
 * @see accept()
 * @see bind()
 * @see listen()
 * @see connect()
 * @see http://man7.org/linux/man-pages/man2/socket.2.html
 */
int
socket(int domain, int type, int protocol);
