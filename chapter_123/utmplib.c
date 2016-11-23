/*utmplib.c
 *为文件的读取提供缓冲区
 *主要的函数有：
 *	utmp_open(filename) 有错时返回-1
 *	utmp_next() 返回指向下一数据的指针、
 *	utmp_close  关闭文件
 */
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <utmp.h>
#include <stdlib.h>

#define NRECS 16
#define NULLUT ((struct utmp *)NULL)
#define UTSIZE (sizeof(struct utmp))

static char utmpbuf[NRECS * UTSIZE]; //缓冲区的大小：16个数据
static int num_recs; 		     //缓冲区数据的个数
static int cur_rec;		     //缓冲区已被使用的数据个数
static int fd_utmp = -1;	     //初始文件描述符

int utmp_open(char *filename)        //返回类型int是我自己加上的，书上没有
				     //后来验证没有int 也是可以无误运行
{
  fd_utmp = open(filename, O_RDONLY);//系统调用open
  cur_rec = num_recs = 0;
  return fd_utmp;
}

struct utmp * utmp_next()
{
  struct utmp *recp;
  if(fd_utmp == -1)
    return NULLUT;
  if(cur_rec == num_recs && utmp_reload() == 0)   //A&&B,A假则判定假，B不执行
  return NULLUT;
  
  recp = (struct utmp *)&utmpbuf[cur_rec * UTSIZE];
  cur_rec++;
    return recp;

}

int utmp_reload()
//调用系统函数read，读数据到缓冲区
{
  int amt_read;
  amt_read = read(fd_utmp, utmpbuf, NRECS * UTSIZE);
  num_recs = amt_read/UTSIZE;
  cur_rec = 0;
    return num_recs;
}

void utmp_close()
{
  if(fd_utmp != -1)
    close(fd_utmp);
} 


