/* forkdemo2.c
 * show how child process pic up(捡起) at the return 
 * from fork() and execute any code they like,
 * even fork().Predict number of lines of output
 */

#include <stdlib.h>
#include <stdio.h>
int main()
{
  printf("my pid is %d\n",getpid());
  fork();
  fork();
  fork();
  printf("my pid is %d\n",getpid());
  return 0;
}
