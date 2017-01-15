/* lclnt1.c
   License server client version 1
   link with lclnt_funcs1.o dgram.o
 */
#include <stdio.h>
#include <stdlib.h>

int main(int ac, char *av[])
{
	setup();
	if(get_ticket() != 0)
		exit(0);
	do_regular_work();

	release_ticket();
	shut_down();
	return 0;
}

do_regular_work()
//the main work if the application goes here
{
	printf("SuperSleep version 1.0 Running _Licensed Software\n");
	sleep(10);
}
