# TCP-example

## 说明
## Introduction

更新。  
update.  

c_tcp_v1, cpp_tcp是以前写的。  
c_tcp_v1, cpp_tcp were written before.  

新增c_tcp_v2。采用类似cpp_tcp的风格的c代码。  
add c_tcp_v2. use c code with a style similar to cpp_tcp.

## 已知问题
## Known Problem

目前，它是最基础的阻塞单线程IO模型, 且不具备实用性。  
at present, it is the most basic blocking single thread IO model and doesn't have practicality.

client的接口设计并不好 或许可以参照Qt的接口风格。
The interface design of the client is not good, perhaps we can refer to Qt's interface style.

## 未来将实现：
## achieve in the future:

c_tcp_v3: epoll, 阻塞, 水平触发, 单线程。  
C_tcp_v3: epoll, blocked, horizontally triggered, single threaded.  

c_tcp_v4: epoll, 无阻塞, 边缘触发, 单线程。  
C_tcp_v4: epoll, non blocking, edge triggered, single threaded.  

c_tcp_v5: epoll, 无阻塞, 边缘触发, 多线程。  
c_tcp_v5: epoll, non blocking, edge triggered, multi-threaded.  

c_tcp_v6: epoll, 无阻塞, 边缘触发, 协程。  
c_tcp_v6: epoll, non blocking, edge triggered, coroutine.  
