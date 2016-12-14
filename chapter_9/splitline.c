/* 
 * splitline.c - command reading and parsing function for smsh
 * char *next_cmd(char *prompt, FILE *fp) - get next command
 * char ** splitline(char *str); - parse a string
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "smsh.h"

char * next_cmd(char *prompt, FILE *fp)
/*
 * purpose: read next command line from fp
 * returns: dynamically allocated string holding command line
 * errors: NULL at EOF (not really an error)
 *    	   calls fatal from emalloc()
 * notes: allocates space in BUFSIZ chunks
 */
{
  char *buf;		//the buffer
  int bufspace = 0;	//total size
  int pos = 0;		//current position
  int c ;		//input char
  
  printf("%s", prompt);
  while((c = getc(fp))!= EOF)
  {
    //need space?
    if(pos+1 >= bufspace)
    {
      if(bufspace == 0)
        buf = emalloc(BUFSIZ);
      else //or expand
        buf = erealloc(buf,bufspace+BUFSIZ);
      bufspace += BUFSIZ;
    }
    //end of command?
    if(c == '\n')
      break;
    //no, add to buffer
    buf[pos++] = c;
  }
  if(c == EOF && pos == 0)
    return NULL;
  buf[pos] = '\0';
  return buf;
}

/* splitline (parse line into an array of strings) */
#define is_delim(x)((x) == ' '||(x) == '\t')

char ** splitline(char *line)
/*
 * purpose: split a line into array of white - space separated tokens
 * returns: a NULL - terminated array of ponters to copies of the 
 * 	    tokens or NULL if line if no tokens on the line
 * action: traverse the array ,locate string ,make copies
 * note: strtok() could work, but we may want to add quotes later
 */
{
  char *newstr();
  char **args;
  int spots = 0;	//spots(位置) in stable
  int bufspace = 0;	//bytes in stable
  int argnum = 0;	//slots(插槽) used
  char *cp = line;	//pos in string
  char *start;
  int len;
  
  if(line == NULL)	//handle special case
    return NULL;

  args = emalloc(BUFSIZ);	//initialize array
  bufspace = BUFSIZ;
  spots = BUFSIZ/sizeof(char *);

  while(*cp != '\0')
  {
    while(is_delim(*cp))	//skip leading spaces
      cp++;
    if(*cp == '\0')		//quit at end -o-string
      break;

    //make sure the array has room (+1 for NULL)(思考：NULL是为了向execvp()函数传array时，确保最后元素为NULL)
    if(argnum+1 >= spots)
    {
      args = erealloc(args,bufspace+BUFSIZ);
      bufspace += BUFSIZ;
      spots += (BUFSIZ/sizeof(char*));
    }

    //make start, then find end of word
    start = cp;
    len = 1;
    while(*++cp != '\0' && ! (is_delim(*cp)))
      len++;
    args[argnum++] = newstr(start, len);
  }
  args[argnum] = NULL;
  return args;
}

/*
 * purpose: constructor for strings
 * returns：a string ,never NULL
 */
char *newstr(char *s, int l)
{
 char * rv = emalloc(l+1);
 rv[1] = '\0';
 /* 
  * char *strncpy(char *dest, const char *src, int n)，
  * 把src所指向的字符串中以src地址开始的前n个字节复制到dest所指的数组中，并返回dest
  */
 strncpy(rv,s,l);	
 return rv;
}

/*
 * purpose: free the list returned by splitline
 * returns: nothing
 * action:free all strings in list and then free the list
 */
void freelist(char **list)
{
  char **cp = list;
  while(*cp)
    free(*cp++);
  free(list);
}

void * emalloc(size_t n)
//size_t是标准C库中定义的，应为unsigned int，在64位系统中为 long unsigned int
{
  void *rv;
  if((rv = malloc(n)) == NULL)
    fatal("out of memory","",1);
  return rv;
}

void *erealloc(void *p, size_t n)
{
  void * rv;
  if((rv= realloc(p,n)) == NULL)
    fatal("realloc() failed","",1);
  return rv;
}
