看了接口，很多的定义和协议的实现以及底层协议息息相关。
如果不了解底层，也确实很难理解。
现在的目标是，让它能用，把遇到的问题先解决掉。
后续再补充相关知识，去扩充和完善对协议底层的理解。

int socket (int domain, int type, int protocol); noexept

    domain: 协议族。在<bits/socket.h中有详细定义>
        常用：
        PF_INET IPv4
        PF_INET6 IPv6
        PF_LOCAL 本地回环

    type: 通信语义 在<socket_type.h中有详细定义>

        SOCK_STREAM    字节流 双向连接 有序 可靠 多次发送会被缓冲合并发送
        SOCK_SEQPACKET 字节流 双向链接 有序 可靠 每次发送都会单独调用底层send
        SOCK_DGRAM     数据报 无连接 无序 不可靠 固定最大长度
        SOCK_RDM       数据报 无连接 无序 可靠 固定最大长度？
        SOCK_RAW       原始网络协议访问

        在此参数上用 | 加一个 SOCK_NONBLOCK 可以改为非阻塞
        在此参数上用 | 加一个 SOCK_CLOEXEC 可以在子线程中关闭该socket ？

    protocol: 协议 一般情况下都是填0，自动选择协议。

    return: 失败返回-1 成功返回一个socket id

借鉴：
    linux manual 2 socket
    https://blog.csdn.net/samsung12345678/article/details/90375695

/* Send N bytes of BUF to socket FD.
   Returns the number sent or -1.
   This function is a cancellation point and therefore not marked with __THROW.  */
extern ssize_t send (int __fd, const void *__buf, size_t __n, int __flags);

/* Send N bytes of BUF on socket FD to peer at address ADDR (which is ADDR_LEN bytes long).
   Returns the number sent, or -1 for errors.
   This function is a cancellation point and therefore not marked with __THROW.  */
extern ssize_t sendto (int __fd, const void *__buf, size_t __n, int __flags, __CONST_SOCKADDR_ARG __addr, socklen_t __addr_len);

/* Send a message described MESSAGE on socket FD.
   Returns the number of bytes sent, or -1 for errors.
   This function is a cancellation point and therefore not marked with __THROW.  */
extern ssize_t sendmsg (int __fd, const struct msghdr *__message, int __flags);

/* Send a VLEN messages as described by VMESSAGES to socket FD.
   Returns the number of datagrams successfully written or -1 for errors.
   This function is a cancellation point and therefore not marked with __THROW.  */
extern int sendmmsg (int __fd, struct mmsghdr *__vmessages, unsigned int __vlen, int __flags);

/* Read N bytes into BUF from socket FD.
   Returns the number read or -1 for errors.
   This function is a cancellation point and therefore not marked with __THROW.  */
extern ssize_t recv (int __fd, void *__buf, size_t __n, int __flags);

/* Read N bytes into BUF through socket FD.
   If ADDR is not NULL, fill in *ADDR_LEN bytes of it with tha address of the sender,
   and store the actual size of the address in *ADDR_LEN.
   Returns the number of bytes read or -1 for errors.
   This function is a cancellation point and therefore not marked with __THROW.  */
extern ssize_t recvfrom (int __fd, void *__restrict __buf, size_t __n, int __flags, __SOCKADDR_ARG __addr, socklen_t *__restrict __addr_len);



/* Structure describing messages sent by 'sendmsg' and received by 'recvmsg'.  */
struct msghdr {
    void *msg_name;		    /* Address to send to/receive from.  */
    socklen_t msg_namelen;	/* Length of address data.  */

    struct iovec *msg_iov;	/* Vector of data to send/receive into.  */
    size_t msg_iovlen;		/* Number of elements in the vector.  */

    void *msg_control;		/* Ancillary data (eg BSD filedesc passing). */
    size_t msg_controllen;	/* Ancillary data buffer length.
                    !! The type should be socklen_t but the
                    definition of the kernel is incompatible
                    with this.  */

    int msg_flags;		/* Flags on received message.  */
};

/* Structure for scatter/gather I/O.  */
struct iovec {
    void *iov_base;	/* Pointer to data.  */
    size_t iov_len;	/* Length of data.  */
};

extern ssize_t send (int __fd, const void *__buf, size_t __n, int __flags);
extern ssize_t recv (int __fd, void *__buf, size_t __n, int __flags);

extern ssize_t sendto (int __fd, const void *__buf, size_t __n,
		       int __flags, __CONST_SOCKADDR_ARG __addr,
		       socklen_t __addr_len);
extern ssize_t recvfrom (int __fd, void *__restrict __buf, size_t __n,
			 int __flags, __SOCKADDR_ARG __addr,
			 socklen_t *__restrict __addr_len);

shutdown：
    /* Shut down all or part of the connection open on socket FD.
    HOW determines what to shut down:
        SHUT_RD   = No more receptions;
        SHUT_WR   = No more transmissions;
        SHUT_RDWR = No more receptions or transmissions.
    Returns 0 on success, -1 for errors.  */
    extern int shutdown (int __fd, int __how) __THROW;

    close() 关闭套接字 从内存中清除 tcp协议会自动触发关闭连接的操作
    close 会立即发送fin包 不管输入缓冲区是否有数据 可能会造成缓冲区数据丢失
    shutdown() 关闭连接 套接字依然存在 直到close才会从内存中清除
    shutdown 会等待缓冲区传输完毕再发送fin包
    https://blog.csdn.net/carey_csdn/article/details/101711637
    但依然需要调用close()来减少socket描述符的引用计数
    https://www.cnblogs.com/JohnABC/p/7238241.html
    一个人做的抓包试验，确实会丢缓冲区的数据。
    https://zhuanlan.zhihu.com/p/187463956

