/* fileinfo.c
 * 利用stat()函数来获取文件的相关属性信息
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>



int main(int ac, char *av[])
{
  struct stat info;		//文件缓冲区
  if(ac>1)
    if(stat(av[1], &info) != -1)
    {
      show_stat_info(av[1], &info);
      return 0;
    }
    else
      perror(av[1]);
  return 1;
}

show_stat_info(char *fname, struct stat *buf)
//显示文件缓冲区的信息
{
  printf("   mode : %o\n", buf->st_mode);
  printf("  links : %d\n", buf->st_nlink);
  printf("   user : %d\n", buf->st_uid);
  printf("  group : %d\n", buf->st_gid);
  printf("   size : %d\n", buf->st_size);
  printf("modtime : %d\n", buf->st_mtime);
  printf("   name : %s\n", fname);
}
