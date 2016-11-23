/* ls1.c 
 * 列出目录下的内容
 */
#include<stdio.h>
#include<sys/types.h>
#include<dirent.h>
#include<stdlib.h>

void do_ls(char *);

main (int ac, char * av[])
{
  if(ac == 1)
    do_ls(".");
  else
    while(--ac){
    printf("%s:\n",* ++av);
    do_ls(*av);
    }
}

void do_ls(char dirname[])
//列出dirname目录下的文件
{
  DIR * dir_ptr;		//目录
  struct dirent * direntp;	//每个目录的入口
  if((dir_ptr = opendir(dirname)) == NULL)
    fprintf(stderr,"ls1: cannot open %s\n",dirname);
  else
  {
    while ((direntp = readdir(dir_ptr)) != NULL)
      printf("%s\n",direntp->d_name);
    closedir(dir_ptr);
  }
}
