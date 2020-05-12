#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <error.h>
#include <string.h>
#include <fcntl.h>

#define BUFSIZE   (1024)

#define FIFO1    "fifo1"
#define FIFO2    "fifo2"

int main(int argc , char *argv[])
{
   int fd0, fd1;

   char readBuf[BUFSIZE];
   char writeBuf[BUFSIZE];
   int readcnt,writecnt;
    int maxfd;
    int ret;
   struct timeval timeval;
   fd_set set0, set1;

   ret = mkfifo(FIFO1, 0777);
   if(ret < 0)
         perror("cannot create fifo1\n");

   ret = mkfifo(FIFO2, 0777);
   if(ret < 0)
         perror("cannot create fifo2\n");

    if( (fd0 = open(FIFO1, O_RDWR, 0) ) < 0) 
         perror("cannot open fifo1\n");
    if( (fd1 = open(FIFO2, O_RDWR, 0) ) < 0) 
         perror("cannot open fifo2\n");


    FD_ZERO(&set0); FD_ZERO(&set1);

    FD_SET(fd0 , &set0);FD_SET(fd1, &set1);

    timeval.tv_sec = 2; timeval.tv_usec = 0;

   memcpy(writeBuf, "hello\n", 6);
   printf("write = %s",writeBuf);

   writecnt = write(fd0, writeBuf, 6);
   if(writecnt < 0)
       perror("write error\n");

    maxfd = fd0>fd1?fd0:fd1;
    if( select(maxfd +1, &set1, &set0, NULL, &timeval) < 0)
        perror("select error\n");
      
while(1)
{

    if(FD_ISSET(fd0, &set0) < 0)
      printf("fd0 is not ready\n");
   else
	{    
           readcnt = read(fd0, readBuf, 10);
           if(readcnt > 0){
              readBuf[readcnt] = '\0';
		printf("read is=%s\n",readBuf);}
           else
             printf("read proc...\n");

	}

   if(FD_ISSET(fd1, &set1) < 0)
      printf("fd1 is not ready\n");   
    else
	{     
           writecnt = write(fd1, readBuf, 10);
           if(writecnt > 0){
              readBuf[writecnt] = '\0';
		printf("write is=%s\n",readBuf);}
           else
             printf("write proc...\n");
	}
   //sleep(1);
}


   return 0;

}
