/* hello4.c
 * 创造移动的假象
 */
#include <stdio.h>
#include <stdlib.h>
#include <curses.h>

main()
{
  int i;
  initscr();
  clear();
  for(i=0;i<LINES;i++)
  {
    move(i,i+i);
    if(i%2 == 1)
      standout();
    addstr("Hello,world");
    if(i%2 == 1)
      standend();
    refresh();
    sleep(1);
    move(i,i+i);
    addstr("            ");
  }
  endwin();
}
