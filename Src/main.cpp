#include "VideoManager.h"


int main()
{
    VideoManager *mgr = new VideoManager;
    mgr->Init();
    videoBuffer *buf = mgr->GetFrameData();
    return 0;
}
