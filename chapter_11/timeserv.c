/*
 * timeserv.c   a socket - based time of day server
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>
#include <strings.h>

#define PORTNUM 13000  //our time service phone number
#define HOSTLEN 256
#define oops(msg)  {perror(msg);exit(1);}

int main(int ac, char *av[])
{


  return 0;
}

