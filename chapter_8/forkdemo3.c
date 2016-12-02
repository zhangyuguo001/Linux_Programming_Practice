/* forkdemo3.c
 * shows how the return value from fork()
 * allows as process to detemine whether
 * it is a child or process
 */
#include <stdio.h>
#include <stdlib.h>

int main()
{
  int fork_rv;
  printf("Before: my pid i s%d\n",getpid());
  fork_rv = fork();
  if(fork_rv == -1)
    perror("fork");
  else if(fork_rv == 0)
    printf("child pid is %d\n", getpid());
  else
    printf("parent pid is %d\n", getpid());
  return 0;
}
