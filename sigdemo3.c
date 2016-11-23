/* sigdemo3.c
 * 关于测试多个信号的问题
 * 问题1：当信号来时，处理函数还正常工作吗？
 * 问题2：信号处理函数X工作，信号X来时会发生什么
 * 问题3：信号处理函数X工作，信号y来时会发生什么
 * 问题4：读取文件时（read()被调用时），信号来时会发生什么
 */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#define INPUTLEN 100

void inthandler(int );
void quithandler(int );

int main(int ac,char *av[])
{
  char input[INPUTLEN];
  int nchars;
  
  signal(SIGINT,inthandler);	//处理ctrl-c中断信号
  signal(SIGQUIT,quithandler);	//处理ctrl-\中断信号
  do{
    printf("\nType a message\n");
    nchars = read(0,input,(INPUTLEN-1));
    if(nchars == -1)
      perror("read returned an error");
    else{
      input[nchars] = '\0';
      printf("You typed: %s",input);
    }
  }
  while(strncmp(input,"quit",4) != 0) ;
  return 1;
}

void inthandler(int s)
{
  printf("Received signal %d..waiting\n",s);
  sleep(2);
  printf("Leaving inthandler \n");
}

void quithandler(int s)
{
  printf("Recevied signal %d..waiting\n",s);
  sleep(3);
  printf("Leaving quithandler\n");
}


