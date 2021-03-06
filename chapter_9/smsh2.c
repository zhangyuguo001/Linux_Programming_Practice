/* smsh2.c -- small-shell version 2
 * small shell that wupports command line parsing
 * and if..then..else.fi logic(by calling process())
 * note: 
 *    Version smsh2:gcc smsh2.c splitline.c execute.c process.c controlflow.c
 *    Version smsh3(unrealized):gcc smsh2.c spliteline.c execute.c process2.c controlflow.c builtin.c varlib.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include "smsh.h"

#define DFL_PROMPT ">"

int main()
{
  char *cmdline, *prompt, **arglist;
  int result;
  void setup();
  
  prompt = DFL_PROMPT;
  setup();

  while((cmdline = next_cmd(prompt, stdin))!= NULL)
  {
    if((arglist = splitline(cmdline)) != NULL)
    {
      result = process(arglist);
      freelist(arglist);
    }
    free(cmdline);
  }
  return 0;
}

void setup()
{
  //initialize shell ,return nothing call fatal() if trouble
  signal(SIGINT,SIG_IGN);
  signal(SIGQUIT,SIG_IGN);
}

void fatal(char *s1,char *s2, int n)
{
  fprintf(stderr,"Error: %s,%s\n",s1,s2);
  exit(n);
}
