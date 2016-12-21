/* pipe.c
 * demonstrate how to create a pipeline from one process to another
 * take two args,each s command ,and connects
 * usage: pipe command1 | command2
 * Limitations: commands to not take arguments
 * uses execlp()since known number of args
 * Note: exchange child and parent and watch fun
 */
#include <stdio.h>
#include <stdlb.h>
#include <unistd.h>
#define oops(m.x) {perror(m);exit(x);}
int main()
{
  int thepipe[2],newfd,pid;
  if(ac != 3)
  {
    fprintf(stderr,"usage: pipe cmd1 cmd2\n");
    exit(1);
  }

  if(pipe(thepipe) == -1)
    oops("cannot get a pipe",1);

  return 0;
}
