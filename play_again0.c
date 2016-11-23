/* play_again0.c
 * 从用户那里得到yes OR no
 */

#include <stdio.h>
#include <stdlib.h>
#include <termios.h>

#define QUESTION "Do you want anthor transaction"
int get_response(char *);

int main()
{
  int response;
  response = get_response(QUESTION);
  return response;
}

int get_response(char *question)
//给出问题，等待用户输入y/n;0表示y,1表示n
//使用getchar函数，忽略非y/n的输入
{
  printf("%s(y/n)?",question);
  while(1)
  {
    switch(getchar())
    {
      case 'y':
      case 'Y': return 0;
      case 'n':
      case 'N':
      case EOF: return 1;
    }
  }
}
