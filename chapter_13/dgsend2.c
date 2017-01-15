/* dgsend.c -datagram sender
 * 		usage: desend hostname portnum "message"
 * 		action: sends message to hostname:portbnum
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#define oops(m,x) {perror(m);exit(x);}

int make_dgram_client_socket();
int make_internet_address(char *,int ,struct sockaddr_in *);

int main(int ac,char *av[])
{
	int sock;					//use this sock to send
	char *msg;					//send this mesage
	struct sockaddr_in saddr; 	//put sender's address here
	socklen_t saddrlen;	

	if(ac != 4){
		fprintf(stderr, "usage: dgsend host port 'message'\n");
		exit(1);
	}
	msg = av[3];

	//get a datagram socket
	if((sock = make_dgram_client_socket()) == -1)
		oops("cannot make socket",2);

	//combine hostname and portnumber of destination into an address
	make_internet_address(av[1],atoi(av[2]),&saddr);

	//send a string through the socket to that address
	if(sendto(sock,msg,strlen(msg),0,&saddr,sizeof(saddr)) == -1)	
		oops("sendto failed", 3);

	//receive mesage from server
	saddrlen = sizeof(saddr);
	if(recv_return_message(sock,&saddr,&saddrlen) == -1)	
		oops("receive from server failed", 4);
	return 0;
}

void recv_return_message(int sock, struct sockaddr_in *addrp,socklen_t *len )
{
	char msg[BUFSIZ+BUFSIZ];
	//saddrlen = sizeof(addrp);
	while((msglen = recvfrom(sock,msg,BUFSIZ,0,addrp,len))>0)
	{
		msg[msglen] = '\0';
		printf("dgrecv: got a message: %s\n",msg);
	}	
}