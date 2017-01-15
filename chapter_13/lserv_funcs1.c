/* lsrv_funcs1.c
   function for thee license server
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>
#include <sys/errno.h>

#define SERVER_PORTNUM 2020  		//our server's port number
#define MSGLEN 128					//size of our datagrams
#define TICKET_AVAIL 0				//Slot is available for use
#define MAXUSERS 3					//only 3 user for us
#define oops(x) {perror(x); exit(-1);}

//important variables
int ticket_array[MAXUSERS];			//our ticket array
int sd = -1;						//our socket
int num_tickets_out = 0;			//number of tickets outstanding

char *do_hello();
char *do_goodbye();

//setup() - initialize license server
setup()
{
	sd = make_dgram_server_socket(SERVER_PORTNUM);
	if(sd == -1)
		oops("make socket");
	free_all_tickets();
	return sd;
}

free_all_tickets()
{
	int i;
	for(i=0;i<MAXUSERS;i++)
		ticket_array[i] = TICKET_AVAIL;
}

//shut_down() - close down license server
shut_down()
{
	close(sd);
}

/* handle_request(request,clientaddr,addrlen)
   baranch on code in request
 */
handle_request(char * req,struct sockaddr_in *client,socklen_t addlen)
{
	char *response;
	int ret;

	//act and compose a response
	if(strncmp(req,"HELLO",4) == 0)
		response = do_hello(req);
	else if(strncmp(req,"GBYE",4) == 0)
		response = do_goodbye(req);
	else
		response = "FALL invalid request";

	//send the response to the client
	narrate("SAID:", response,client);
	ret = sendto(sd,response,strlen(response),0,client,addlen);
	if(ret == -1)
		perror("SERVER sendto failed");	
}

/* do_hello
   give out a ticket if any are available
   IN msg_p   message received from client
   Results: ptr to response
   note: return in static buffer verwrittrn/overwritten by each call
 */
 
 char *do_hello(char * msg_p)
 {
 	int x;
 	static char replybuf[MSGLEN];

 	if(num_tickets_out >= MAXUSERS)
 		return("FALL no ticket available");

 	//else find a free ticket and give it to client

 	for(x=0;x<MAXUSERS && ticket_array[x] != TICKET_AVAIL;x++);

 	//A sanity check - should nerver happen
 	if(x == MAXUSERS){
 		narrate("datagrams corrupt","",NULL);
 		return("FALL datagrams corrupt");
 	}

 	//Found a free ticket. Recond "name" of user(pid) in array
 	//generate ticket of from: pid.slot
 	ticket_array[x] = atoi(msg_p + 5);		//get pid in msg
 	sprintf(replybuf,"TICK %d.%d", ticket_array[x],x);
 	num_tickets_out++;
 	return(replybuf);
 }  

 /* do_goodbye
    take back ticket client is returning
    IN msg_p       message received from client
    results: ptr to response
    note:return is in static buffer over writen by each each call
  */

 char *do_goodbye(char *msg_p)
 {
 	int pid,slot;		//components of ticket

 	/* the user's giving us back a ticket.first we need to get
 	   the tikcet out of the message,which looks like:

 	   GBYE pid.slot
 	 */
 	if((sscanf((msg_p + 5), "%d. %d",&pid,&slot) != 2)||
 		(ticket_array[slot] != pid)){
 		narrate("Bogus ticket", msg_p+5,NULL);
 		return("FALL invalid ticket");
 	}

 	//the ticket is valid. release it
 	ticket_array[slot] = TICKET_AVAIL;
 	num_tickets_out--;

 	//Return response
 	return("THNX See ya!");
 }

 narrate(char *msg1,char *msg2,struct sockaddr_in *clientp)
 {
 	fprintf(stderr, "\t\tSERVER: %s %s",msg1, msg2);
 	if(clientp)
 		fprintf(stderr, "(%s: %d)",inet_ntoa(clientp->sin_addr),
 		ntohs(clientp->sin_port));
 	putc('\n',stderr);
 }


