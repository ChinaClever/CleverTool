/*
 * libSys.h
 * linux常用头文件
 *  Created on: 2016年12月15日
 *      Author: Lzy
 */

#ifndef LIBSYS_H_
#define LIBSYS_H_

//POSIX标准定义的头文件
#include <dirent.h>        //目录项
#include <fcntl.h>         //文件控制
#include <fnmatch.h>    //文件名匹配类型
#include <glob.h>    //路径名模式匹配类型
#include <grp.h>        //组文件
#include <netdb.h>    //网络数据库操作
#include <pwd.h>        //口令文件
#include <regex.h>    //正则表达式
#include <tar.h>        //TAR归档值
#include <termios.h>    //终端I/O
#include <unistd.h>    //符号常量
#include <utime.h>    //文件时间
#include <wordexp.h>    //字符扩展类型

//-------------------------
#include <arpa/inet.h>    //INTERNET定义
#include <net/if.h>    //套接字本地接口
#include <netinet/in.h>    //INTERNET地址族
#include <netinet/tcp.h>    //传输控制协议定义

//-------------------------
#include <sys/mman.h>    //内存管理声明
#include <sys/select.h>    //Select函数
#include <sys/socket.h>    //套接字借口

#include <sys/stat.h>    //文件状态
#include <sys/times.h>    //进程时间
#include <sys/types.h>    //基本系统数据类型
#include <sys/un.h>    //UNIX域套接字定义
#include <sys/utsname.h>    //系统名
#include <sys/wait.h>    //进程控制

//------------------------------
//POSIX定义的XSI扩展头文件
#include <cpio.h>    //cpio归档值
#include <dlfcn.h>    //动态链接
#include <fmtmsg.h>    //消息显示结构

#include <ftw.h>        //文件树漫游
#include <iconv.h>    //代码集转换使用程序
#include <langinfo.h>    //语言信息常量
#include <libgen.h>    //模式匹配函数定义

#include <monetary.h>    //货币类型
//#include <ndbm.h>    //数据库操作
#include <nl_types.h>    //消息类别

#include <poll.h>   // 轮询函数
#include <search.h>   // 搜索表

#include <strings.h>   // 字符串操作
#include <syslog.h>    //系统出错日志记录
#include <ucontext.h>   // 用户上下文
#include <ulimit.h>    //用户限制
#include <utmpx.h>   // 用户帐户数据库

//-----------------------------
#include <sys/ipc.h>    //IPC(命名管道)
#include <sys/msg.h>    //消息队列
#include <sys/resource.h> //资源操作
#include <sys/sem.h>    //信号量
#include <sys/shm.h>    //共享存储
#include <sys/statvfs.h>    //文件系统信息
#include <sys/time.h>    //时间类型
#include <sys/timeb.h>    //附加的日期和时间定义
#include <sys/uio.h>    //矢量I/O操作

//------------------------------
//POSIX定义的可选头文件
#include <aio.h>        //异步I/O
#include <mqueue.h>    //消息队列
#include <pthread.h>    //线程
#include <sched.h>    //执行调度
#include <semaphore.h>    //信号量
#include <spawn.h>     //实时spawn接口
#include <stropts.h>   // XSI STREAMS接口
//#include <trace.h>    // 事件跟踪

#endif /* LIBSYS_H_ */
