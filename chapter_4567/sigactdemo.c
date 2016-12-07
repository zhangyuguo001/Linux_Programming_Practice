/* sigactdemo.c
 * sigaction新信号的用法演示
 * 演示：处理SINGINT信号时，如何阻塞SIGQUIT信号
 */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#define INPUTLEN 100

void inthandler();

int main()
{
 struct sigaction newhandler;
 sigset_t blocked;
 char x[INPUTLEN];
 
 newhandler.sa_handler = inthandler;
 newhandler.sa_flags = SA_RESETHAND | SA_RESTART;

 sigemptyset(&blocked);			//clear all bits
 sigaddset(&blocked,SIGQUIT);		//add SIGQUIT to list
 newhandler.sa_mask = blocked;		//store blockmask
 
 if(sigaction(SIGINT,&newhandler,NULL) == -1)
   perror("sigaction");
 else
   while(1)
   {
     fgets(x,INPUTLEN,stdin);
     printf("input: %s",x);
   } 
 return 0;
}

void inthandler(int s)
{
  printf("Called with signal %d\n",s);
  sleep(s);
  printf("done handling signal %d\n",s);
}
