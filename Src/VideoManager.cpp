#include "VideoManager.h"

#include <fcntl.h>
#include <malloc.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <sys/mman.h>
#include <linux/videodev2.h>
#include <iostream>
// #include <libv4l2.h>

#define CLEAR(x) memset(&(x), 0, sizeof(x))
#define IMG_WIDTH 1280
#define IMG_HEIGHT 720



int VideoManager::OpenDevice()
{
    _Fd = open("/dev/video0", O_RDWR | O_NONBLOCK, 0);
    if(_Fd < 0)
    {
        return -1;
    }
    return 0;
}


int VideoManager::CloseDevice()
{
    return close(_Fd);
}


int VideoManager::QueryCapability()
{
    struct v4l2_capability cap;
    int ret = ioctl(_Fd, VIDIOC_QUERYCAP, &cap);
    if(ret == -1)
    {
        std::cout << "Query capability failed!" << std::endl;
        return -1;
    }

    if(!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE))
    {
        std::cout << "Device is not video capture device." << std::endl;
        return -1;
    }

    if(!(cap.capabilities & V4L2_CAP_STREAMING))
    {
        std::cout << "Device does not support streming I/O" << std::endl;
        return -1;
    }
    return 0;
}

int VideoManager::SetVideoParm()
{
    struct v4l2_format fmt;
    CLEAR(fmt);
    fmt.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	fmt.fmt.pix.width       = IMG_WIDTH;
	fmt.fmt.pix.height      = IMG_HEIGHT;
	fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
    fmt.fmt.pix.field       = V4L2_FIELD_INTERLACED;
    
    if(-1 == ioctl(_Fd, VIDIOC_S_FMT, &fmt))
    {
        std::cout << "VIDIOC_S_FMT error" << std::endl;
        return -1;
    }

    return 0;
}


int VideoManager::InitMMap()
{
    struct v4l2_requestbuffers req;
    CLEAR(req);

    req.count = 5;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;

    if(-1 == iotcl(_Fd, VIDIOC_REQBUFS, &req))
    {
        std::cout << "Init memory mapping failed." << std::endl;
        return -1;
    }
    return 0;
}
