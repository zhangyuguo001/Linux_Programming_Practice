/* forkdemo1.c
 * show how fork create two process ,distinguishable
 * by the different return values from fork()
 * 一次调用两次返回
 */

#include <stdio.h>
#include <stdlib.h>
int main()
{
  int ret_from_fork,mypid;
  mypid = getpid();
  printf("Before: my pid is %d\n", mypid);

  ret_from_fork = fork();

  sleep(1);
  printf("After: my pid is %d,fork() said %d\n",getpid(),ret_from_fork);
  return 0;
}


