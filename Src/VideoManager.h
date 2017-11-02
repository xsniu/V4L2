#ifndef _VIDEO_H
#define _VIDEO_H

struct videoBuffer
{
    void *     data;
    size_t     length;
};

class VideoManager
{
    int _Fd {0};
    videoBuffer * _Buffers;
    int _BuffersNum;

public:
    VideoManager() {}
    ~VideoManager() {}
    void Init();
    videoBuffer* GetFrameData();
private:

    int OpenDevice();
    int CloseDevice();
    int QueryCapability();
    int SetVideoParm();
    int InitMMap();
    void GetInfo();
    void StartCapturing();
    
};

#endif