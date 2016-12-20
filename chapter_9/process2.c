/* prcess.c
 * command processing layer
 *
 * the process(char **arglist) function is called by the main loop
 * It sits in front of the execute() function.This layer handles
 * two main classes of processing:
 * a)built - in functions(e.g.exit(),set,=,read,...)
 * b)control structures(e.g. if. while,for)
 */

#include <stdio.h>
#include <stdlib.h>
#include "smsh.h"

int is_control_comand(char *);
int do_control_comand(char **);
int ok_to_execute();

int process(char **args)
/*
 * purpose: process use command
 * returns: result of processing command
 * details: if a built - in then call approprite function ,if not
 * 	    execute()
 * errors: arise from sbroutines ,handled there
 */
{
  int rv = 0;
  if(args[0] == NULL)
    rv = 0;
  else if(is_control_command(args[0]))
    rv = do_control_command(args);
  else if(ok_to_execute())
    if(!builtin_command(args,&rv))
      rv = execute(args);
  return rv;
}

