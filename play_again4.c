/* paly_again4.c
 * 使用非阻塞模式实现超时响应
 * 增加了非阻塞模式
 * 重置中断模式
 * 相比于上一版本增加对信号的响应，信号处理函数的使用
 */
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>

#define ASK "Do you want another transation"
#define TRIES 3			//最多尝试次数为3次
#define SLEEPTIME 2		//每次尝试的等候时间
#define BEEP putchar('\a')	//提醒用户

void ctrl_c_handler(int );
main()
{
  int response;
  tty_mode(0);
  set_cr_noecho_mode();
  set_nodelay_mode();
  signal(SIGINT,ctrl_c_handler);	//中断信号的处理
  signal(SIGQUIT,SIG_IGN);		//ignore QUIT信号
  response = get_response(ASK,TRIES);
  tty_mode(1);
  return response;
}

get_response(char *question,int maxtries)
//等待输入y/n,或者最大次数尝试后返回
{
  int input;
  printf("%s(y/n)",question);
  fflush(stdout);   	//force output
  while(1)
  {
    sleep(SLEEPTIME);
    input = tolower(get_ok_char()); //get next chr*
    if(input == 'y')
      return 0;
    if(input == 'n')
      return 1;
    if(maxtries-- == 0)
      return 2;
    BEEP;
  }
}

get_ok_char()
{
  int c;
  while((c = getchar()) != EOF && strchr("yYnN",c) == NULL)
    ;
  return c;
}

set_cr_noecho_mode()
{
  struct termios ttystate;
  tcgetattr(0,&ttystate);
  ttystate.c_lflag &= ~ICANON;	//no buffering
  ttystate.c_lflag &= ~ECHO;	//没有回显
  ttystate.c_cc[VMIN] = 1;
  tcsetattr(0,TCSANOW,&ttystate);
}

set_nodelay_mode()
{
  int termflags;
  termflags = fcntl(0,F_GETFL);	 //读取当前设置
  termflags |= O_NDELAY;	 //将nodelay位打开
  fcntl(0,F_SETFL,termflags);	 //配置设置
}

tty_mode(int how)
{
  static struct termios original_mode;
  static int original_flags;
  if(how == 0)
  {
    tcgetattr(0,&original_mode);
    original_flags = fcntl(0,F_GETFL);
  }else{
    tcsetattr(0,TCSANOW,&original_mode);
    fcntl(0,F_SETFL,original_flags);
  }
}

void ctrl_c_handler(int signum)
//信号中断时，恢复初始的tty设置
{
  tty_mode(1);
  exit(1);
}
