/* hello3.c
 * 使用refresh和sleep函数实现动画效果
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
    addstr("Hello, world");
    if(i%2 == 1)
      standend();
    sleep(1);
    refresh();
  }
  endwin();
}
