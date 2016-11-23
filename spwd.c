/* swpwd.c 
 * pwd命令的简单版本
 * 使用readdir()来获取信息
 * chdir函数会改变当前进程的工作目录，进程有一个存放当前目录i-节点号的变量
 * 目录改变，变量更新为新目录的i-节点号
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>


void printpathto(ino_t );
void inum_to_name(ino_t , char *, int );
ino_t get_inode(char *);
int main()
{
  printpathto(get_inode("."));
  putchar('\n');
  return 0;
}

void printpathto(ino_t this_inode)
//利用递归结构实现的路径打印
{
  ino_t my_inode;
  char its_name[BUFSIZ];
  if(get_inode("..") != this_inode)
  {
    chdir("..");
    inum_to_name(this_inode,its_name,BUFSIZ);
    my_inode = get_inode(".");
    printpathto(my_inode);
    printf("/%s", its_name);
  }
}

void inum_to_name(ino_t inode_to_find, char *namebuf, int buflen)
//根据i-节点获取文件名，并存储于字符串namebuf中
{
  DIR *dir_ptr;		//DIR这个变量在哪定义的？
  struct dirent *direntp;
  dir_ptr = opendir(".");
  if(dir_ptr == NULL)
  {
    perror(".");
    exit(1);
  }
  while((direntp = readdir(dir_ptr)) != NULL)
    if(direntp->d_ino == inode_to_find)
    {
      strncpy(namebuf, direntp->d_name, buflen);
      namebuf[buflen-1] = '\0';	//确保字符串的结尾是\0
      closedir(dir_ptr);
      return;
    }
  fprintf(stderr,"error looking for inum /%d\n", inode_to_find);
  exit(1);
}

ino_t get_inode(char *fname )
//向stat函数传递文件名，获取其i-节点
{
  struct stat info;
  if(stat(fname, &info) == -1)
  {
    fprintf(stderr, " Cannot stat");
    perror(fname);
    exit(1);
  }
  return info.st_ino;
}


