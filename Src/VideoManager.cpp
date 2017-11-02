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

struct v4l2_buffer curBuf;

int VideoManager::OpenDevice()
{
    _Fd = open("/dev/video0", O_RDWR | O_NONBLOCK, 0);
    if(_Fd < 0)
    {
        std::cout << "Open device failed" << std::endl;
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

    _Buffers = (videoBuffer*) calloc(req.count, sizeof(*_Buffers));
    if(!_Buffers)
    {
        std::cout << "Out of memory" << std::endl;
        return -1;
    }

    for(_BuffersNum = 0; _BuffersNum < req.count; ++_BuffersNum)
    {
        struct v4l2_buffer buf;
        CLEAR(&buf, 0, sizeof(buf));

        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = _BuffersNum;
        if(-1 == ioctl(_Fd, VIDIOC_QUERYBUF, &buf))
        {
            std::cout << "VIDIOC_QUERYBUF error";
            return -1;
        }

        _Buffers[_BuffersNum].length = buf.length;
        _Buffers[_BuffersNum].start = 
            mmap (NULL /* start anywhere */,
                buf.length,
                PROT_READ | PROT_WRITE /* required */,
                MAP_SHARED /* recommended */,
                _Fd, buf.m.offset);
        
        if (MAP_FAILED == _Buffers[_BuffersNum].start)
        {
            std::cout << "mmap failed" << std::endl;
            return -1;
        }
    }
    return 0;
}


void VideoManager::GetInfo()
{
    struct v4l2_input input;
	int index;

	if (-1 == ioctl (_Fd, VIDIOC_G_INPUT, &index)) {
		perror ("VIDIOC_G_INPUT");
		exit (EXIT_FAILURE);
	}

    CLEAR(input);
	input.index = index;

	if (-1 == ioctl (_Fd, VIDIOC_ENUMINPUT, &input)) {
		perror ("VIDIOC_ENUMINPUT");
		exit (EXIT_FAILURE);
	}

	printf ("Current input: %s\n", input.name);
}

void VideoManager::StartCapturing()
{
	unsigned int i;
	enum v4l2_buf_type type;


	for (i = 0; i < _BuffersNum; ++i) {
		struct v4l2_buffer buf;

		memset(&buf, 0, sizeof(buf));

		buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory      = V4L2_MEMORY_MMAP;
		buf.index       = i;

		if (-1 == ioctl (_Fd, VIDIOC_QBUF, &buf)) {
			perror ("VIDIOC_QBUF");
			exit(EXIT_FAILURE);
		}
	}

	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	if (-1 == ioctl (_Fd, VIDIOC_STREAMON, &type)) {
		perror ("VIDIOC_STREAMON");
		exit(EXIT_FAILURE);
	}
}

videoBuffer* VideoManager::GetFrameData()
{
	unsigned int i;
    CLEAR(curBuf)
	curBuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	curBuf.memory = V4L2_MEMORY_MMAP;

	if (-1 == ioctl (_Fd, VIDIOC_DQBUF, &curBuf)) {
		switch (errno) {
		case EAGAIN:
			return -1;

		case EIO:
			/* Could ignore EIO, see spec. */

			/* fall through */

		default:
			perror ("VIDIOC_DQBUF");
			exit(EXIT_FAILURE);
		}
	}

    assert (curBuf.index < _BuffersNum);
    
    videoBuffer* ret = new videoBuffer;
    ret.start = new unsigned char [_Buffers[buf.index].length];
    ret.length = _Buffers[buf.index].length;
    memcpy(ret.start, _Buffers[buf.index].start, ret.length);


	// printf ("%d %d: ", buf.index, buf.bytesused);

	if (-1 == ioctl (_Fd, VIDIOC_QBUF, &curBuf)) {
		perror ("VIDIOC_QBUF");
		exit(EXIT_FAILURE);
	}

	return ret;
}

void VideoManager::Init()
{
    if(-1 == OpenDevice())
    {
        return;
    }

    if (-1 == QueryCapability())
    {
        return;
    }

    if(-1 == SetVideoParm())
    {
        return;
    }

    if(-1 == InitMMap())
    {
        return;
    }

    GetInfo();
    StartCapturing();
}

