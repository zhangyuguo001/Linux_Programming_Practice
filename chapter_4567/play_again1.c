/* play_again1.c
 * 改进：关闭规范输入
 */
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>

#define QUESTION "Do you want anther transaction"

main()
{
  int response;
  tty_mode(0);	//保存tty mode
  set_crmode();	//设置为chr-by-chr mode
  response = get_response(QUESTION);
  printf("\n");
  tty_mode(1);	//恢复tty mode
  return response;
}

int get_response(char *question)
//相比于上一版本，增加了对non y/n的处理
{
  int input;
  printf("%s (y/n)?",question);
  while(1)
  {
    switch(input = getchar())
    {
      case 'y':
      case 'Y': return 0;
      case 'n':
      case 'N': 
      case EOF: return 1;
      default:
        printf("\ncannot understand %c,",input);
	printf("Please type y or n \n");
    }
  }
}

set_crmode()
//将标准输入的规范模式改成chr-by-by模式
{
  struct termios ttystate;
  tcgetattr(0,&ttystate);		//读取当前设置
  ttystate.c_lflag &= ~ICANON;		//屏蔽缓冲区功能
  ttystate.c_cc[VMIN] = 1;		//每次输入得到一个字母
  tcsetattr(0,TCSANOW,&ttystate);	//配置当前设置
}

tty_mode(int how)
//how=0,保存当前tty设置模式；how=1,恢复tty设置模式（体会static的用法）
{
  static struct termios original_mode;
  if(how == 0)
    tcgetattr(0,&original_mode);
  else
    return tcsetattr(0,TCSANOW,&original_mode);
}
