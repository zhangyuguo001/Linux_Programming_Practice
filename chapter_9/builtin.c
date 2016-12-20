/*
 * builtin.c
 * contains the switch and the function for builtin commands
 */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "smsh.h"
#include "varlib.h"

int assign(char *);
int okname(char *);

int builtin_command(char **args, int *resultp)
/*
 * purpose: run a builtin command
 * return: 1 if args[0] is builtin ,0 if not
 * details: test arg[0] against all known built - ins .Call functions
 */
{
  int rv = 0;
  if(strcmp(args[0],"set") == 0)
  {
    VLlist();
    *resultp = 0;
    rv =1;
  }
  else if(strchr(args[0], '=') != NULL)
  {
    *resultp = assign(args[0]);
    if(*resultp != -1)
      rv = 1;
  }
  else if(strcmp(args[0],"export") == 0)
  {
    if(args[1] != NULL && okname(args[1]))
      *resultp = VLexport(args[1]);
    else 
      *resultp = 1;
  }
  return rv;
}

int assign (char * str)
/* 
 * purpose: execute name = val AND ensure that name is legal
 * return: -1 for ilegal lval, or result of Vlstore
 * warning: modefies the string ,but retores it to normal
 */
{
  char *cp;
  int rv;
  cp = strchr(str, '=');
  *cp = '\0';
  rv = (okname(str)?VLstore(str,cp+1): -1);
  return rv;  
}

int okname(char *str)
/*
 * purpose: determines if a string is a legal variable name
 * return: 0 for no,1 for yes
 */
{
  char *cp;
  for(cp = str; *cp;cp== )
  {
    if((isdigit(*cp) && cp ==str)||!(isalnum(*cp)||*cp =='_'))
      return 0;
  }
  return (cp != str);	//no empty strings, either
}
