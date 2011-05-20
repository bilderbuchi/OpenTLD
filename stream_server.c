/* 
A really simple server in the internet domain using TCP.
This is supposed to run with one client only.
The server is hardcoded to run on port 5000.

This program is extremely simple, and it's not in any way bugfree.

Author: Rafael Oliveira <ludug3r0@gmail.com>
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     CvCapture *capture = NULL;
     IplImage *frame = NULL;
     IplImage *gray = NULL;

     int sockfd, newsockfd, portno;
     socklen_t clilen;
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     // if needed the port can be changed bellow
     portno = atoi("5000");
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");

     // Here you can choose if you wanna capture from CAM ou File
     capture = cvCaptureFromCAM(0);
     //capture = cvCaptureFromFile("/home/raso/Videos/BR.avi");


     /* My webcam suplies me with 640x480 frames, if yours is different,
        change bellow and don't forget to make changes in img/img_get.m */
     gray = cvCreateImage(cvSize(640,480), IPL_DEPTH_8U, 1);
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd, 
		 (struct sockaddr *) &cli_addr, 
		 &clilen);
     if (newsockfd < 0) error("ERROR on accept");
     while (1) {
	     frame = cvQueryFrame(capture);
             // In this case the frame is sent to the client in grayscale
             cvCvtColor(frame, gray, CV_BGR2GRAY);
	     n = write(newsockfd,gray->imageData,gray->imageSize);
	     if (n < 0) error("ERROR writing to socket");
	     
             // The next line shows what the cam sees, you can comment it.
             cvShowImage("Server", gray);
             // You can get out of the server by pressing 'q' 
	     if ((char)cvWaitKey(5) == 'q') break;
     }
     close(newsockfd);
     close(sockfd);
     return 0; 
}
