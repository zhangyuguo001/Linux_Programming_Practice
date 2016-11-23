/* hello.c
 * 简单演示curses的使用
 */
#include <stdio.h>
#include <stdlib.h>
#include <curses.h>

main()
{
  initscr();	//初始化curses库和tty
  clear();	//清屏  
  move(10,20);
  addstr("Hello ,world");	//在当前位置画字符串
  move(LINES-1,0);
  refresh();
  getch();		 	//等待用户输入
  endwin();			//关闭curses并重置tty  
}
