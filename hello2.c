/* hello2.c
 * 带有稍微复杂的显示效果
 */
#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
main()
{
  int i;
  initscr();
  clear();
  for(i=0;i<=LINES;i++)
  {
    move(i,i+i);
    if(i%2 == 1)
      standout();
    addstr("Hello,world");
    if(i%2 == 1)
      standend();
  }
  refresh();
  getch();
  endwin();
}
