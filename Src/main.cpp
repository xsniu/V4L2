#include "VideoManager.h"

#include <fcntl.h>              /* low-level i/o */
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
using namespace cv;


int main()
{
#if 0
    CvCapture * capture = cvCreateCameraCapture(-1);
    IplImage *frame = NULL;
    cvNamedWindow("video", 1);
    while(1)
    {
        frame = cvQueryFrame(capture);
        if(!frame)
        {
              break;
        }
        cvShowImage("video", frame);
        char c = cvWaitKey(33);
        
        if(c == 27)
        {
            break;
        }
        
    }
    cvReleaseCapture(&capture);
    cvDestroyWindow("video");    
#endif

    VideoCapture cap; 
    cap.open(0);
    if (!cap.isOpened())// check if we succeeded
    {
    return -1;
    }
    namedWindow("Video", 1);
    while (1)
    {
    Mat frame;
    cap >> frame; // get a new frame from camera
    imshow("Video", frame);
    if (waitKey(1) == 27) break;
    }
    // Release the camera or video file
    cap.release();
    return 0;
}
