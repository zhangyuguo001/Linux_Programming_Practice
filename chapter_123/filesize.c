/*filesize.c 
 *输出文件大小
 *此实例程序输出/etc/passwd文件的大小
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int main ()
{
  struct stat infobuf;		//建立文件信息缓存区域，struct stat是存放文件属性的一个结构
  if(stat("/etc/passwd", &infobuf) == -1)
    perror("etc/passwd");
  else
    printf("The size of /etc/passwd is %d\n",infobuf.st_size );
}
  
