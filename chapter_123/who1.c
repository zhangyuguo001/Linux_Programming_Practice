/*
 *  *who1.c -who命令的第一次版本
 *   *open,read UTMP file,and show results
 *    *P35~36：open,read，close的相关用法
 */
#include <stdio.h>
#include <utmp.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#define SHOWHOST

void show_info(struct utmp*);

int main()
{
	struct utmp current_record;//将信息读到这里
	int utmpfd;//文件描述符
	int reclen = sizeof(current_record);
	if((utmpfd = open(UTMP_FILE,O_RDONLY)) == -1)
		{
			perror(UTMP_FILE);//perror是一个系统函数，处理系统报错
			exit(1);
			}
	while(read(utmpfd,&current_record,reclen) == reclen)
		show_info(&current_record);
		close(utmpfd);
		return 0;
	}	
	
	/*
  	 *show info()显示current_record的内容
 	*/
	void show_info(struct utmp*utbufp)
	{
		printf("% -8.8s",utbufp->ut_name);
		printf(" ");
		printf("% -8.8s",utbufp->ut_line);
		printf(" ");
		printf("% -10ld",utbufp->ut_time);
		printf(" ");
		#ifdef SHOWHOST 
		printf("(%s)",utbufp->ut_time);
		#endif
		printf("\n");
		}
