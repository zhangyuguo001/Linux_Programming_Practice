/* bounceld.c
 * 向程序中添加用户的速度和方向控制，以产生动画效果
 * handler产生动画效果
 * 主程序从键盘读取输入
 * 编译注意：gcc bounceld set_ticker.c -l curses -o bounceld
 * curses是一个关于界面效果的库
 * 控制键f和s
 */

#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <signal.h>
#include <string.h>

#define MESSAGE "hello"
#define BLANK "      "

int row; 	//当前的行
int col;	//当前的列
int dir;	//目的

int main()
{
  int delay;	//bigger >= slower
  int ndelay;	//new delay
  int c;	//user input
  void move_msg(int);
  
  initscr();	//初始化curses和tty
  crmode();
  noecho();
  clear();

  row = 10;
  col = 0;
  dir = 1;
  delay = 200;

  move(row,col);
  addstr(MESSAGE);
  signal(SIGALRM,move_msg);
  set_ticker(delay);

  while(1)
  {
    ndelay = 0;
    c = getch();
    if(c == 'Q') break;
    if(c == ' ') dir = -dir;
    if(c == 'f' && delay>2) ndelay = delay/2; //闹钟间隔变小，移动速度变快
    if(c == 's') ndelay = delay * 2;	      //闹钟间隔变大，移动速度变慢
    if(ndelay>0)
      set_ticker(delay = ndelay);
  }
  endwin();	//关闭curses并重置tty
  return 0;
}

void move_msg(int signum)
{
  signal(SIGALRM,move_msg);
  move(row,col);
  addstr(BLANK);
  col += dir;
  move(row,col);
  addstr(MESSAGE);
  refresh();

  //处理临界区 now handle borders
  if(dir == -1 && col<=0)
    dir = 1;
  else if (dir == 1&& col+strlen(MESSAGE)>=COLS)
    dir = -1;
}

