/* pshl.c
 * prompting(提示) shell version 1
 * Prompts for the command and its arguments
 * Build the argument vector for the call to execvp
 * Uses execvp(),and never returns
 */
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

#define MAXARGS 20 //cmdline args
#define ARGLEN 100 //token length

int main()
{
  char *arglist[MAXARGS+1];	//an arry of ptrs
  int numargs;			//index into array
  char argbuf[ARGLEN];		//read stuff here
  char *makestring();		//malloc etc
  numargs = 0;
  while(numargs < MAXARGS)
  {
    printf("Arg[%d]", numargs);
    if(fgets(argbuf, ARGLEN, stdin) && *argbuf != '\n')
      arglist[numargs++] = makestring(argbuf);
    else
    {
      if(numargs>0)
        arglist[numargs] = NULL;
	execute(arglist);
	numargs = 0;
    }
  }
  return 0;
}

int execute(char *arglist[])
//use execvp to do it 
{
  execvp(arglist[0], arglist);
  perror("execvp failed");
  exit(1);
}

char* makestring(char *buf)
//trim off(修减) newline and create storage for the string
{
  char *cp;
  buf[strlen(buf)-1] = '\0';
  cp = (char*) malloc(strlen(buf)+1);
  if(cp == NULL)
  {
    fprintf(stderr ,"no memory\n");
    exit(1);
  }
  strcpy(cp, buf);	//copy chars
  return cp;		//return ptr
}

