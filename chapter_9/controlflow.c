/* controlflow.c
 * "if" processing is done with two state variables
 * if_state and is_result
 */
#include <stdio.h>
#include <stdlib.h>
#include "smsh.h"

enum states {NEUTRAL,WANT_THEN,THEN_BLOCK};
enum reults {SUCCESS,FALL};

static int if_state = NEUTRAL;	//中立的
static int if_result = SUCCESS;
static int last_stat = 0;

int syn_err(char *);

int ok_to_execute()
/* 
 * purpose: determine the shell should execute a command
 * returns: 1 for yes, 0 for no
 * details: if in THEN_BLOCK and if_result was SUCCESS then yes
 * 	    if in THEN_BLOCK and if_result was FALL then no 
 * 	    if in WANT_THEN then syntax error (sh is different)
 */
{
  int rv = 1;	//default is positive
  if(if_state == WANT_THEN)
  {
    syn_err("then expected");
    rv = 0;
  }
  else if(if_state == THEN_BLOCK && if_result == SUCCESS)
    rv = 1;
  else if(if_state == THEN_BLOCK && if_result == FALL)
    rv = 0;
  return rv;  
}

int is_control_command(char *s)
/*
 * purpose: boolean to report if the command is a shell control command
 * returns: 0 or 1
 */
{
  return(strcmp(s,"if")==0 || strcmp(s,"then")==0 || strcmp(s,"fi")==0);
}

int do_control_command(char **args)
/*
 * purpose: boolean to report if the command is a shell control command
 * return: 0 or 1
 */
{
  char *cmd = args[0];
  int rv = -1;
  
  if(strcmp(cmd,"if")==0)
  {
    if(if_state != NEUTRAL)
      rv = syn_err("if unexpected");
    else{
      last_stat = process(args+1);	//递归的使用是为了if语句嵌套另外一个语句？案列运行后未能反映该功能，
      					//2016-12-20 14:57:01
      if_result = (last_stat == 0?SUCCESS:FALL);
      if_state = WANT_THEN;
      rv = 0;
    }
  }else if(strcmp(cmd,"then"==0)){
    if(if_state != WANT_THEN)
      rv = syn_err("then unexpected");
    else{
      if_state = THEN_BLOCK;
      rv = 0;
    }
  }else if(strcmp(cmd,"fi"==0)){
    if(if_state != THEN_BLOCK)
      rv = syn_err("fi unexpected");
    else{
      if_state = NEUTRAL;
      rv = 0;
    }
  }else 
    fatal("internal error processing:", cmd,2);
  return rv;
}

int syn_err(char *msg)
/* 
 * purpose: handles syntax errors in control structures
 * details: resets state to NEUTRAL
 * return: -1 in interactive mode.should call fatal in scripts
 */
{
  if_state = NEUTRAL;
  fprintf(stderr, "syntax error: %s\n",msg);
  return -1;
}
