/* echostate.c
 * 说明终端是否被设置成回显字符的
 * 模式。读取设置，测试位，并报告结果
 */
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>

void main() 
{
  struct termios info;
  int rv;
  rv = tcgetattr(0, &info);	//获取相关设置到info中
  if(rv == -1)
  {
    perror("tcgetattr");
    exit(1);
  }
  if(info.c_lflag & ECHO)
   printf("echo is on ,since its bit is 1\n");
  else
  printf("echo is OFF ,since its bits is 0\n");
}
