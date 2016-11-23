/* who3.c 
 * 带有缓冲流的who3命令，速度上会有提升
 *
 * 注：在原本who2.c中主机名（或是ip地址）是不打印的
 *     who3.c中加了缓存功能后，便可以打印，原因暂时未明确
 *     2016年10月9日16:00:48
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <utmp.h>
#include <fcntl.h>
#include <time.h>
#include "utmplib.c"

#define SHOWHOST

void show_info(struct utmp *);
void showtime(time_t);
int main()
{
  struct utmp *utbufp,*utmp_next();

  if(utmp_open(UTMP_FILE) == -1)
  {
    perror(UTMP_FILE);
    exit(1);
  }
  while ((utbufp = utmp_next()) != (struct utmp *)NULL)
    show_info(utbufp);
  utmp_close();
  return 0;
}


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
