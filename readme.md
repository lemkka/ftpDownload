# Linux下ftp多线程下载程序
## 1.概要介绍
这是一个ftp多线程下载程序<br>
一共分为三个文件：ftp.h,ftp.c和main.c<br>
其中ftp.h包含ftp下载所需要使用的函数的声明<br>
ftp.c包含这些函数的具体实现方式<br>
main.c是整个程序入口<br>
## 2.实现思路
程序是通过socket套接字配合ftp命令结合实现的，主要思路是先连接上ftp服务器，然后通过ftp_list命令得到需要下载的文件信息，从而得到文件的大小，再根据指定线程数将文件进行分片，最终创建多个线程分别下载文件
## 3.启动方式
在linux命令行运行如下命令：<br>
gcc -o ftpDownload ftp.c ftp.h main.c -lpthread
