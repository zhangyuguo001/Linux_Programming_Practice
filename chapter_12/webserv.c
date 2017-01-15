/* webserv.c - a minimal web server(version 0.2)
 * 		usage :ws portnumber
 * ferture: supports the GET command only
 *			runs in the current directory
 * 			forks a new child to handle each request
 *			has MAJOR security holes,for demo purposes only
 *			has many other weakness,but is a good start
 * build: gcc webserv.c socklib.c -o webserv
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

int main(int ac,char *av[])
{
	int sock,fd;
	FILE *fpin;
	char request[BUFSIZ];
	if(ac == 1)
	{
		fprintf(stderr,"usage: webserv portnumber\n");
		exit(1);
	}
	sock = make_server_socket(atoi(av[1]));  
	if(sock == -1) exit(2);

	//main loop here 
	while(1)
	{
		//take a call anfd buffer it
		fd = accept(sock,NULL,NULL);
		fpin = fdopen(fd,"r");

		//read request
		fgets(request,BUFSIZ,fpin);
		printf("got a call: request = %s",request);
		read_till_crnl(fpin);

		//do what_client asks
		process_rq(request,fd);

		fclose(fpin);
	}
	return 0;
}

//read_till_crnl(FILE *) skip over all request info until a CRNL is seen
read_till_crnl(FILE *fp)
{
	char buf[BUFSIZ];
	while(fgets(buf,BUFSIZ,fp) != NULL && strcmp(buf,"\r\n") != 0);
}

/* process_rq(char *,int fd)
 * do what the request asks for and write reply to fd
 * handles request in a new prcess
 * rq is HTTP command: GET /foo/bar.html HTTP/1.0
 */
process_rq(char *rq, int fd)
{
	char cmd[BUFSIZ],arg[BUFSIZ];
	//create a new process and return if not the child
	if(fork() != 0)
		return;
	strcpy(arg,"./");		//precede args with
	if(sscanf(rq,"%s %s",cmd,arg+2) != 2)
		return;
	if(strcmp(cmd,"GET") != 0)
		cannot_do(fd);
	else if(not_exist(arg))
		do_404(arg,fd);
	else if(isadir(arg))
		do_ls(arg,fd);
	else if(ends_in_cgi(arg))
		do_exec(arg,fd);
	else
		do_cat(arg,fd);
}

header(FILE *fp,char * content_type)
//the reply header thing
{
	fprintf(fp, "HTTP/1.0 200 OK\r\n");
	if(content_type)
		fprintf(fp, "content-type: %s\r\n",content_type);
}

cannot_do(int fd)
{
	FILE *fp = fdopen(fd,"w");
	fprintf(fp,"HTTP/1.0 501 Not Implemented\r\n");
	fprintf(fp, "content-type: text/plain\r\n" );
	fprintf(fp,"\r\n");

	fprintf(fp,"That command is not yet implemented\r\n");
	fclose(fp);
}

do_404(char *item ,int fd)
{
	FILE * fp = fdopen(fd,"w");
	fprintf(fp,"HTTP/1.0 404 Not Found\r\n");
	fprintf(fp, "content-type: text/plain\r\n" );
	fprintf(fp,"\r\n");

	fprintf(fp,"The item you request: %s\r\nis not found\r\n",item);
	fclose(fp);
}

isadir(char *f)
{
	struct stat info ;		//the struct stat store the dir information
	return (stat(f,&info) != -1 && S_ISDIR(info.st_mode));
}

not_exist(char *f)
{
	struct stat info;
	return (stat(f,&info) == -1);
}

do_ls(char *dir, int fd)
{
	FILE *fp;
	fp = fdopen(fd,"w");
	header(fp,"text/plain");
	fprintf(fp,"\r\n");
	fflush(fp);		//clear the buffer, force to print

	dup2(fd,1);
	dup2(fd,2);
	close(fd);

	execlp("ls","ls","-l",dir,NULL);
	perror(dir);
	exit(1);
}

char * file_type(char *f)
{
	char *cp;
	if((cp = strrchr(f,'.')) != NULL)
		return cp+1;
	return "";
}

ends_in_cgi(char *f)
{
	return (strcmp(file_type(f),"cgi") == 0);
}

do_exec(char * prog,int fd)
{
	FILE *fp;
	fp = fdopen(fd,"w");
	header(fp,NULL);
	fflush(fp);
	dup2(fd,1);
	dup2(fd,2);
	close(fd);
	execl(prog,prog,NULL);
	perror(prog);
}

do_cat(char *f,int fd)
{
	char *extension = file_type(f);
	char *content = "text/plain";
	FILE *fpsock,*fpfile;
	int c;

	if(strcmp(extension,"html") == 0)
		content = "text/html";
	else if(strcmp(extension,"gif") == 0)
		content = "text/gif";
	else if(strcmp(extension,"jpg") == 0)
		content = "text/jpg";
	else if(strcmp(extension,"jpeg") == 0)
		content = "text/jpeg";
	fpsock = fdopen(fd,"w");
	fpfile = fopen(f,"r");
	if(fpsock != NULL && fpfile != NULL)
	{
		header(fpsock,content);
		fprintf(fpsock, "\r\n");
		while((c = getc(fpfile)) != EOF)
			putc(c,fpsock);
		fclose(fpfile);
		fclose(fpsock);
	}
	exit(0);
}
