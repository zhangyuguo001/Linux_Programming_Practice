/* ls2.c
 * 输出当前目录下的列表信息
 * 用到stat结构，pwd.h和grp.h中的用户结构和组结构
 * 	目录是文件的列表，更确切的说，是记录的序列，每条记录对应一个文件或子目录。
 * 	通过readdir来读取目录的记录，readdir返回一个指向目录的当前记录的指针，记
 * 	录的类型是struct dirent,结构定义在、usr/include/dirent.h
 *
 * 	磁盘上的文件有很多属性，如文件大小，文件所有者的ID等。如果需要得到文件属
 * 	性，进程可以定义一个结构struct stat,然后调用stat,告诉内核把文件属性放到这
 * 	个结构中
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>

void do_ls(char[]);
void dostat(char *);
void show_file_info(char *, struct stat *);
void mode_to_letters(int , char[]);
char * uid_to_name(uid_t);
char * gid_to_name(gid_t);

main (int ac, char *av[])
{
  if(ac == 1)
    do_ls(".");
  else
    while(--ac)
    {
      printf("%s:\n",*++av);
      do_ls(*av);
    }
}

void do_ls(char dirname[])
//输出目录dirname下的文件信息
{
  DIR *dir_ptr;			//目录
  struct dirent *direntp;	//每条记录的引用
  if((dir_ptr = opendir(dirname)) == NULL)
    fprintf(stderr,"ls1: cannot open %s\n", dirname);
  else 
  {
    while((direntp = readdir(dir_ptr)) != NULL)
      dostat(direntp->d_name);
    closedir(dir_ptr);
  }
}

void dostat(char *filename)
{
  struct stat info;
  if(stat(filename, &info) == -1)
    perror(filename);
  else
    show_file_info(filename, &info);
}

void show_file_info(char *filename, struct stat *info_p)
//打印filename的相关信息，该信息存储在struct stat结构中
{
  char *uid_to_name(), *ctime(), *gid_to_name(), *filemode();
  void mode_to_letters();
  char modestr[11];

  mode_to_letters(info_p->st_mode, modestr);

  printf("%s", modestr);
  printf("%4d ", (int)info_p->st_nlink);
  printf("%-8s ", uid_to_name(info_p->st_uid));
  printf("%-8s", gid_to_name(info_p->st_gid));
  printf("%8ld ", (long)info_p->st_size);
  printf("%.12s", 4 + ctime(&info_p->st_mtime));
  printf(" %s\n", filename);
}

void mode_to_letters(int mode, char str[])
{
  strcpy(str, "----------");
  if(S_ISDIR(mode)) str[0] = 'd';
  if(S_ISCHR(mode)) str[0] = 'c';
  if(S_ISBLK(mode)) str[0] = 'b';

  if(mode & S_IRUSR) str[1] = 'r';
  if(mode & S_IWUSR) str[2] = 'w';
  if(mode & S_IXUSR) str[3] = 'x';
  if(mode & S_IRGRP) str[4] = 'r';
  if(mode & S_IWGRP) str[5] = 'w';
  if(mode & S_IXGRP) str[6] = 'x';
  if(mode & S_IROTH) str[7] = 'r';
  if(mode & S_IWOTH) str[8] = 'w';
  if(mode & S_IXOTH) str[9] = 'x';
}

#include <pwd.h>
char * uid_to_name(uid_t uid)
{
  struct passwd * getpwuid(), *pw_ptr;
  static char numstr[10];

  if((pw_ptr = getpwuid(uid)) == NULL)
  {
    sprintf(numstr, "%d", uid);
    return numstr;
  }else
    return pw_ptr->pw_name;
}

#include <grp.h>
char * gid_to_name(gid_t gid)
{
  struct group * getgrgid(), *grp_ptr;
  static char numstr[10];
  if((grp_ptr =getgrgid(gid)) == NULL)
  {
    sprintf(numstr, "%d", gid);
    return numstr;
   }else
   return grp_ptr->gr_name;
}





















