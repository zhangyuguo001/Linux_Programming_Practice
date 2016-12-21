/* pipedemo.c
 * demonstrates: how to create and use a pipe 
 * effect：create a pipe ,write into writng 
 * end,then runs around and reads from reading 
 * end.A little weried ,but demonstrate the idea
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
int main()
{
  int len,i,apipe[2];
  char buf[BUFSIZ];
  char buf2[BUFSIZ];

  //get a pipe
  if(pipe(apipe) == -1)
  {
    perror("could not make pipe.");
    exit(1);
  }
  printf("Got a pipe! It is file description:{%d %d}\n",apipe[0],apipe[1]);

  //read from stdin, write into pipe ,read from pipe ,print
  while(fgets(buf,BUFSIZ,stdin))
  {
    len  = strlen(buf);
    if(write(apipe[1],buf,len)!=len)
    {
      perror("writing to pipe");
      break;
    }
    for(i =0;i<len;i++)
      buf[i] = 'X';
    printf("buf content is: %s\n",buf);

    len  = read(apipe[0],buf2,BUFSIZ);
    if(len == -1)
    {
      perror("reading from pipe");
      break;
    }
    printf("buf2 content is: %s\n",buf2);
    if(write(1,buf2,len)!= len)
    {
      perror("writing to stdout");
      break;
    }
  }
  return 0;
}
