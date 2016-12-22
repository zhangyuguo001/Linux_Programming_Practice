/* pipe.c
 * demonstrate how to create a pipeline from one process to another
 * take two args,each s command ,and connects
 * usage: pipe command1 | command2
 * Limitations: commands to not take arguments
 * uses execlp()since known number of args
 * Note: exchange child and parent and watch fun
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define oops(m,x) {perror(m);exit(x);}
int main(int ac,char **av)
{
  int thepipe[2],newfd,pid;
  if(ac != 3)
  {
    fprintf(stderr,"usage: pipe cmd1 cmd2\n");
    exit(1);
  }

  if(pipe(thepipe) == -1)
    oops("cannot get a pipe",1);
  if ((pid = fork()) == -1)
    oops("cannot fork",2);

  //parent will read from pipe
  if(pid>0)
  {
    close(thepipe[1]);	//parent doesn't write to pipe
    if(dup2(thepipe[0],0) == -1)
      oops("could not redirect stdin",3);
    close(thepipe[0]);	//stdin is dupped,close pipe
    execlp(av[2],av[2],NULL);
    oops(av[2],4);
  }else{

  //child execs av[1] and writes into pipe 
  close(thepipe[0]);	//child doesn't read from pipe
  if(dup2(thepipe[1],1) == -1)
    oops("could not redirect stdout",4);
  close(thepipe[1]);	//stdout is duped,close pipe
  execlp(av[1],av[1],NULL);
  oops(av[1],5);
 }
  return 0;
}
