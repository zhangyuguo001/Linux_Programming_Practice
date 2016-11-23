/* cp1.c
 * 版本1，将缓存中的内容写入文件中
 * 时间：2016年7月11日20:53:09
 */
 #include <stdlib.h>
 #include <stdio.h>
 #include <unistd.h>
 #include <fcntl.h>
 #define BUFFERSIZE 4096
 #define COPYMODE 0644
 
 void oops(char *,char *);
 main (int ac,char *av[])
  {
  	int in_fd,out_fd,n_chars;
  	char buf[BUFFERSIZE];
  	
  	if(ac!=3)		//检查args(暂时不太明白....)
  		fprintf(stderr,"usage: %s source destination\n",*av);
  		exit(1);
  	
  	if((in_fd = open(av[1],O_RDONLY)) == -1)			//打开文件
  		opps("Cannot open ",av[1]);  	
  	if((out_fd = creat(av[2],COPYMODE)) == -1)			//新建文件
  		opps("Cannot creat ",av[2]);
  		
  		while((n_chars = read(in_fd,buf,BUFFERSIZE))>0)		//从打开的文件读取数据
  			if(write(out_fd,buf,n_chars) != n_chars)	//将读取数据写入新建文件中
  				oops("Write error to ",av[2]);
  				
  		if(n_chars == -1) 
  			oops("Read error from",av[1]);
  		if(close(in_fd) == -1 || close(out_fd) == -1)		//关闭两个文件
  			oops("Error closing files",""); 	
  		
}
  	void oops(char *s1,char *s2)//报错函数
  	{
  		fprintf(stderr,"Error: %s ",s1);			//文件输出方式的写法
  		perror(s2);
  		exit(1);
  		}
