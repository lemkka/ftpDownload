#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "ftp.h"
#include <pthread.h>
#define THREADS 7
 
 
//gcc -o ftp ftp.h ftp.c main.c


int *test(Args* args)
{
	printf("%d  %d\n",args->index,args->totalLen);
	return 0;
}
 
int main(){
 
    char host[13] = {0};
    memcpy(host,"192.168.180.128",strlen("192.168.180.128"));  //ip
    int port = 21;
    char user[10] = {0};
    memcpy(user,"admin",strlen("admin"));   //用户
    char pwd[10] = {0};
    memcpy(pwd,"admin",strlen("admin"));    //密码
    int ret = ftp_connect(host, port, user,pwd );   //连接
    char s[32] = {0};
    char d[32] = {0};
    char file[] = "test.txt";
 
    long long unsigned int fileSize = 0;
	
 
    memcpy(s,file,strlen(file)); //ftp上的源文件
 
    memcpy(d,file,strlen(file));  //下载下来的保存文件路径
	
	unsigned long long fileLen=0; //得到文件的大小 
	fileLen=ftp_list(ret,s);
	printf("%lld\n",fileLen);
	struct timespec ts1,ts2;
	float time_s,time_ns,time_ms;
	clock_gettime(CLOCK_MONOTONIC,&ts1);
	pthread_t tid[THREADS];
	int i;
	for(i=0;i<THREADS;i++)
	{
		Args* args=(Args*)malloc(sizeof(Args));
		args->s=s;
		args->d=d;
		//args->totalLen=fileLen/THREADS;
		args->startPos=fileLen/THREADS*i;
		args->index=i;
		if(fileLen%THREADS==0)
		{
			args->totalLen=fileLen/THREADS;
		}
		else
		{
			if(i!=THREADS-1)
			{
				args->totalLen=fileLen/THREADS;
			}
			else
			{
				args->totalLen=fileLen-fileLen/THREADS*i;
			}
		}
		printf("%lld  %lld\n",args->startLen,args->totalLen);
		if(pthread_create(&tid[i],NULL,(void *)ftp_retrfile,args) != 0)   
    	{
	        perror("线程创建失败!\n");
	        return 0;
    	}  
	}
	for(i=0;i<THREADS;i++)
    {
		int res=pthread_join(tid[i],NULL);     //用于等待子线程结束后主函数才能停止      
		printf("down is %d\n",res);
	} 
	clock_gettime(CLOCK_MONOTONIC,&ts2);
	time_s = ts2.tv_sec - ts1.tv_sec;  //得到秒的时间
    time_ns = (ts2.tv_nsec - ts1.tv_nsec); //得到纳秒的时间
    //由于总的时间=time_s+time_ns
    //为了显示方便，将总的时间统一转化为毫秒
    time_ms = time_s*1000+time_ns/1000000;
	printf("%ld %ld\n",time_s,time_ns);
    printf("run time is %fms\n",time_ms);
	//int down=pthread_join(tid[0],NULL);     //用于等待子线程结束后主函数才能停止      
    //int down = ftp_retrfile(args);
    printf("\ndown = %d,fileSize=%d",i,(int)fileSize);

  
 
    return  0;
 
}
