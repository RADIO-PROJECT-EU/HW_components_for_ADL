/* Capture video input from camera and display on screen */

#include "cv.h"
#include "highgui.h"
#include <stdio.h>
#include <ctype.h>
#include "radio_socket.h"


IplImage *image = 0;
IplImage *show  = 0;

#define WINDOWNAME "RADIO Camera Capture/Show Application"

#define CAPTURE_WEBCAM
//#define CAPTURE_BMP (11)


#define REDV   0
#define GREENV 1
#define BLUEV  2


typedef unsigned char byte;


#include "process.c"

int main(int argc, char** argv) {
  
  CvCapture *capture = 0;
 
  int x,y;
  unsigned char RGB[640][480][3];
  unsigned char Bac[640][480][3]; 
   
  capture = cvCaptureFromCAM(0);

  cvNamedWindow(WINDOWNAME, 1);
  
  
  unsigned char ip[4];
  ip[0]=192;
  ip[1]=168;
  ip[2]=0;
  ip[3]=30;
  udp_open(&ip);
  
  unsigned int iteration, index;
  
  iteration = 1;
  
  for(;;) {
  
    if (iteration>16383) iteration=0; else iteration++;
    index = (iteration%5)+1;

    #ifdef CAPTURE_WEBCAM
        image  = cvQueryFrame(capture);
    #endif

    #ifdef CAPTURE_BMP
    	char fnam[16];
	sprintf(fnam,"LIVE_%04d.bmp",(iteration%CAPTURE_BMP)+1);
	printf("Reading: %s\n",fnam);
        image  = cvLoadImage(fnam,CV_LOAD_IMAGE_COLOR);
    #endif
    
    int rows    = image->height;
    int columns = image->width;
    int step    = image->widthStep;
    int channels= image->nChannels;
    
    printf("Rows = %d   Columns = %d  channels = %d\n",rows,columns,channels);
    
    udp_send_string("Hello!\n");
    
    unsigned char *image_data = (unsigned char*)(image->imageData);
    
    for (y=0;y<rows;y++) {
        for (x=0;x<columns;x++) {
		RGB[x][y][REDV  ] = image_data[step*y+x*3+0];
		RGB[x][y][GREENV] = image_data[step*y+x*3+1];
		RGB[x][y][BLUEV ] = image_data[step*y+x*3+2];
		//printf("x=%d, y=%d, i=%d\n",x,y,step*y+x*3);
        }
    }
    
    process(RGB,Bac,index,3);
    
    
    for (y=0;y<rows;y++) {
      for (x=0;x<columns;x++) {
		image_data[step*y+x*3+0] = RGB[x][y][REDV  ];
		image_data[step*y+x*3+1] = RGB[x][y][GREENV];
		image_data[step*y+x*3+2] = RGB[x][y][BLUEV ];
      }
    }
    
    cvShowImage(WINDOWNAME, image);

    char c = cvWaitKey(30);
    if (c =='q') { break; }
  }

  cvReleaseCapture(&capture);
  cvDestroyWindow(WINDOWNAME);

  return 0;
}

