/*
 *  *who2.c -who命令的第二次版本
 */
#include <stdio.h>
#include <utmp.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>

//#define SHOWHOST
void showtime(long);
void show_info(struct utmp *);

int main()
{
	struct utmp utbuf;//将信息读到这里
	int utmpfd;//文件描述符
	if((utmpfd = open(UTMP_FILE,O_RDONLY)) == -1)
		{
			perror(UTMP_FILE);//perror是一个系统函数，处理系统报错
			exit(1);
			}
	while(read(utmpfd,&utbuf,sizeof(utbuf)) == sizeof(utbuf))
		show_info(&utbuf);
		close(utmpfd);
		return 0;
	}	
	
	/*
  	 *show info()显示current_record的内容
 	*/
	void  show_info(struct utmp*utbufp)
	{
		if(utbufp->ut_type != USER_PROCESS)
		  return;
		printf("% -8.8s",utbufp->ut_name);
		printf(" ");
		printf("% -8.8s",utbufp->ut_line);
		printf(" ");
		showtime(utbufp->ut_time);
		printf(" ");
		#ifdef SHOWHOST 
		if(utbufp->ut_host[0] != '\0')
			printf("(%s)",utbufp->ut_host);
		#endif
		printf("\n");
		}
	void showtime(long timeval)
	//ctime 将时间以日常生活的时间方式显示
	{
	    	char *cp;
	  	cp = ctime(&timeval);//将时间传递给字符串
		printf("%12.12s",cp+4);//从第4个位置输出12个字母
	}
