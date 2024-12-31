//
// Created by haminlove on 2024-12-23.
//

#ifndef ASSEMBLYTEST_OPENGLENGINE_H
#define ASSEMBLYTEST_OPENGLENGINE_H

#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <GLES3/gl32.h>

class OpenGLEngine
{
public:
    bool Initialize(class android_app* app );
    void Clear();
    void SwapBuffer();
    void Tick(float Seconds);
    void OnPause();
    void OnResume(class android_app* app);
private:
    EGLDisplay mDisplay = EGL_NO_DISPLAY;
    EGLSurface  mSurface = EGL_NO_SURFACE;
    EGLContext mEGLContext = EGL_NO_CONTEXT;
    int mWidth=0;
    int mHeight=0;
    float mClearColorRed=0;
    float mElapsedSec=0;
    EGLConfig* mConfigList;
    int mSelectedConfigIndex = -1;
};


#endif //ASSEMBLYTEST_OPENGLENGINE_H
