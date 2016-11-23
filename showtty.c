/* 显示驱动程序属性
 * 未写完
 * 2016年10月26日19:15:22
 */
#include <stdlib.h>
#include <stdio.h>
#include <termios.h>

main()
{
  struct termios ttyinfo;
  if(tcgetattr(0,&ttyinfo) == -1 )
  {
    perror("cannot get params about stdin");
    exit(1);
  }
  showbaud(cfgetospeed(&ttyinfo));
  
}
