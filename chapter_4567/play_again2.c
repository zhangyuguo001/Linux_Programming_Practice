/* play_again2
 * 相比于上一版本增加了忽略非法键的功能
 */

#include <stdio.h>
#include <stdlib.h>
#include <termios.h>

#define QUESTION "Do you wsnt anthor trsnsaction"

main()
{
  int response;
  tty_mode(0);
  set_cr_noecho_mode();  
  response = get_response(QUESTION);
  printf("\n");
  tty_mode(1);
  return response;
}

int get_response(char * question)
{
  printf("%s(y/n)?",question);
  while(1)
  {
    switch(getchar())
    {
     case 'y':
     case 'Y':return 0;
     case 'n':
     case 'N':
     case EOF:return 1;
    }
  }
}

set_cr_noecho_mode()
{
  struct termios ttystate;
  tcgetattr(0,&ttystate);
  ttystate.c_lflag &= ~ICANON;
  ttystate.c_lflag &= ~ECHO;
  ttystate.c_cc[VMIN] = 1;
  tcsetattr(0,TCSANOW,&ttystate);
}

tty_mode(int how)
{
  static struct termios original_mode;
  if(how == 0)
    tcgetattr(0,&original_mode);
  else
    return tcsetattr(0,TCSANOW,&original_mode);
}
