/* tinybc.c
 *           +-----------+              +------------+
 *   stdin  >0           >== pipetodc ==>            |
 *   	     |  tinybc   |              |    dc      |
 *   stdout <1           <==pipefromdc==<            |
 *           +-----------+              +------------+
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define oops(m,x) {perror(m);exit(x);}
int main()
{  
  int pid,todc[2],fromdc[2];

  //make two pipes
  if(pipe(todc) == -1 || pipe(fromdc) == -1)
    oops("pipe failed",1);

  //get a process for user interface
  if((pid = fork()) == -1)
    oops("cannot fork",2);
  
  if(pid == 0)			//child is dc
    be_dc(todc,fromdc);	
  else				//parent is ui
  {
    be_bc(todc,fromdc);  	//wait for child
    wait(NULL);
  }
return 0;
}

be_dc(int in[2], int out[2])
//set up stdin and stdout ,then execl dc
{
  //setup stdin from pipein
  if(dup2(in[0],0) == -1)
    oops("dc:cannot redirect stdin",3);
  close(in[0]);		//moved to fd 0
  close(in[1]);		//won'twrite here

  //setuo stdout to pipeout
  if(dup2(out[1],1) == -1)
    oops("dc: cannot redirect stdout",4);
  close(out[1]);	//moved to fd 1
  close(out[0]);	//won't read from here

  //execl dc with the - option
  execlp("dc","dc","-",NULL);
  oops("cannot run dc",5);
}

be_bc(int todc[2],int fromdc[2])
/* read from stdin and convert into RPN,send sown pipe
 * then read from other pipe and print to user
 * Uses fdopen() to convert a file descriptor to a strem
 */
{
  int num1,num2;
  char operation[BUFSIZ],message[BUFSIZ],*fgets();
  FILE *fpout,*fpin, *fdopen();
  
  //setup
  close(todc[0]);	//won't read from pipe to dc
  close(fromdc[1]);	//won't write to pipe from dc 
  
  fpout = fdopen(todc[1],"w");		//convert file descriptor
  fpin = fdopen(fromdc[0],"r");		//riptors to streams
  if(fpout == NULL || fpin == NULL)
    fatal("Error convering pipes to streams");
  
  //main loop
  while(printf("tinybc: "),fgets(message,BUFSIZ,stdin) != NULL)
  {
	  //parse input
	  if(sscanf(message,"%d %[-+ */^]%d",&num1,operation,&num2) != 3)
	  {
		  printf("systax error\n");
		  continue;
	  }
	  if(fprintf(fpout,"%d\n%d\n%c\np\n", num1,num2,*operation) == EOF)
		  fatal("Error writing");
	  fflush(fpout);	//clear fpont buffer
	  if(fgets(message,BUFSIZ,fpin) == NULL)
		  break;
	  printf("%d %c %d = %s", num1, *operation, num2, message);
  }
  fclose(fpout); 		//close pipe
  fclose(fpin);			//dc will see EOF  
}

fatal(char *mess[])
{
	fprintf(stderr,"Error: %s\n",mess);
	exit(1);
}
