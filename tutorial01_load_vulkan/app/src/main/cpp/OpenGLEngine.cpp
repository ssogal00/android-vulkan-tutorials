//
// Created by haminlove on 2024-12-23.
//

#include "OpenGLEngine.h"

#include <GLES3/gl3.h>
#include <GLES3/gl32.h>
#include <EGL/eglext.h>
#include <android/log.h>
#include <game-activity/native_app_glue/android_native_app_glue.h>
#include <cmath>
#define LOG_TAG "[MyEGL]"

const  int EGLMinRedBits		= 8;
const  int EGLMinGreenBits		= 8;
const  int EGLMinBlueBits		= 8;
const  int EGLMinAlphaBits		= 0;
const  int EGLMinDepthBits		= 16;
const  int EGLMinStencilBits	= 8;
const  int EGLMinSampleBuffers	= 1;
const  int EGLMinSampleSamples	= 0;

const EGLint Attributes[] = {
        EGL_RED_SIZE,       EGLMinRedBits,
        EGL_GREEN_SIZE,     EGLMinGreenBits,
        EGL_BLUE_SIZE,      EGLMinBlueBits,
        EGL_ALPHA_SIZE,     EGLMinAlphaBits,
        EGL_DEPTH_SIZE,     EGLMinDepthBits,
        EGL_STENCIL_SIZE,   EGLMinStencilBits,
        EGL_SAMPLE_BUFFERS, EGLMinSampleBuffers,
        EGL_SAMPLES,        EGLMinSampleSamples,
        EGL_RENDERABLE_TYPE,  EGL_OPENGL_ES3_BIT,
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT | EGL_PBUFFER_BIT,
        EGL_CONFIG_CAVEAT,  EGL_NONE,
        EGL_NONE
};

struct ConfigInfo
{
    int RedBits = 0;
    int GreenBits = 0;
    int BlueBits = 0;
    int AlphaBits =0;
    int StencilBits=0;
    int SampleBuffers=0;
    int Samples=0;
    int DepthBits = 0;
};


bool OpenGLEngine::Initialize(android_app* app)
{
    mDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if(mDisplay == EGL_NO_DISPLAY)
    {
        __android_log_print(ANDROID_LOG_ERROR, "[Vulkan]","EGL_NO_DISPLAY");
        return false;
    }

    __android_log_print(ANDROID_LOG_INFO, "[Vulkan]", "EGL Get Display Success");

    if(!eglInitialize(mDisplay, nullptr, nullptr))
    {
        __android_log_print(ANDROID_LOG_ERROR, "[Vulkan]","eglInitialize failed");
        return false;
    }

    __android_log_print(ANDROID_LOG_INFO, "[Vulkan]", "EGL Inintialize Success");

    EGLint NumConfigs = 0;
    if(!eglGetConfigs(mDisplay, nullptr, 0, &NumConfigs))
    {
        __android_log_print(ANDROID_LOG_ERROR, "[Vulkan]","eglGetConfigs failed");
        return false;
    }

    __android_log_print(ANDROID_LOG_INFO, "[Vulkan]", "EGL NumConfigs : %d" , NumConfigs);

    if(!eglChooseConfig(mDisplay, Attributes, nullptr, 0, &NumConfigs))
    {
        __android_log_print(ANDROID_LOG_ERROR, "[Vulkan]","eglChooseConfig failed");
        return false;
    }

    mConfigList = new EGLConfig[NumConfigs];

    if(!eglChooseConfig(mDisplay, Attributes, mConfigList, NumConfigs, &NumConfigs) )
    {
        __android_log_print(ANDROID_LOG_ERROR, "[Vulkan]","eglChooseConfig with attribute failed");
        return false;
    }

    if(NumConfigs <= 0)
    {
        __android_log_print(ANDROID_LOG_ERROR, "[Vulkan]","Num Config is zero !!! ");
        return false;
    }

    int SelectedConfigIndex = -1;

    ConfigInfo SelectedConfigInfo{};

    for(int i = 0; i < NumConfigs; ++i)
    {
        int ResultValue;
        int R,G,B,A,D,S,SB, Samples;
        eglGetConfigAttrib(mDisplay, mConfigList[i], EGL_RED_SIZE, &R);
        eglGetConfigAttrib(mDisplay, mConfigList[i], EGL_GREEN_SIZE, &G);
        eglGetConfigAttrib(mDisplay, mConfigList[i], EGL_BLUE_SIZE, &B);
        eglGetConfigAttrib(mDisplay, mConfigList[i], EGL_ALPHA_SIZE, &A);
        eglGetConfigAttrib(mDisplay, mConfigList[i], EGL_DEPTH_SIZE, &D);
        eglGetConfigAttrib(mDisplay, mConfigList[i], EGL_STENCIL_SIZE, &S);
        eglGetConfigAttrib(mDisplay, mConfigList[i], EGL_SAMPLE_BUFFERS, &SB);
        eglGetConfigAttrib(mDisplay, mConfigList[i], EGL_SAMPLES, &Samples);

        __android_log_print(ANDROID_LOG_INFO, "[Vulkan]","Red : %d, Green : %d, Blue : %d, Alpha  : %d, Depth : %d, Stencil : %d, Sample Buffer : %d, Samples : %d",
                            R,G,B,A,D,S,SB, Samples);

        if(SelectedConfigIndex == -1 )
        {
            SelectedConfigIndex = i;
            SelectedConfigInfo.RedBits = R;
            SelectedConfigInfo.GreenBits = G;
            SelectedConfigInfo.BlueBits = B;
            SelectedConfigInfo.AlphaBits= A;
            SelectedConfigInfo.StencilBits = S;
            SelectedConfigInfo.DepthBits = D;
            SelectedConfigInfo.SampleBuffers = SB;
            SelectedConfigInfo.Samples = Samples;
        }
        else
        {
            if( SelectedConfigInfo.RedBits <= R &&
                SelectedConfigInfo.BlueBits <= B &&
                SelectedConfigInfo.GreenBits <= G &&
                SelectedConfigInfo.AlphaBits <= A &&
                SelectedConfigInfo.DepthBits <= D &&
                SelectedConfigInfo.StencilBits <= S &&
                SelectedConfigInfo.Samples <= Samples)
            {
                SelectedConfigIndex = i;
                SelectedConfigInfo.RedBits= R;
                SelectedConfigInfo.BlueBits= B;
                SelectedConfigInfo.GreenBits= G;
                SelectedConfigInfo.AlphaBits= A;
                SelectedConfigInfo.DepthBits= D;
                SelectedConfigInfo.StencilBits= S;
                SelectedConfigInfo.Samples= Samples;
            }
        }
    }

    mSelectedConfigIndex = SelectedConfigIndex;
    __android_log_print(ANDROID_LOG_INFO, "[Vulkan]","eglChooseConfig Selected Index : %d", SelectedConfigIndex);


    __android_log_print(ANDROID_LOG_INFO, "[Vulkan]","Red : %d, Green : %d, Blue : %d, Alpha  : %d, Depth : %d, Stencil : %d, Sample Buffer : %d, Samples : %d",
                        SelectedConfigInfo.RedBits, SelectedConfigInfo.GreenBits, SelectedConfigInfo.BlueBits, SelectedConfigInfo.AlphaBits,
                        SelectedConfigInfo.DepthBits, SelectedConfigInfo.StencilBits, SelectedConfigInfo.SampleBuffers, SelectedConfigInfo.Samples);


    mSurface =  eglCreateWindowSurface(mDisplay, mConfigList[SelectedConfigIndex], app->window, nullptr);

    if(mSurface == EGL_NO_SURFACE)
    {
        __android_log_print(ANDROID_LOG_ERROR, "[Vulkan]","eglCreateWindowSurface Failed!!");
        return false;
    }

    eglQuerySurface(mDisplay, mSurface, EGL_WIDTH, &mWidth);
    eglQuerySurface(mDisplay, mSurface, EGL_HEIGHT, &mHeight);

    __android_log_print(ANDROID_LOG_INFO, "[Vulkan]","eglCreateWindowSurface Success !!");
    __android_log_print(ANDROID_LOG_INFO, "[Vulkan]","Width : %d, Height : %d !!", mWidth, mHeight);

    int ContextAttributes[5]
            {
                    EGL_CONTEXT_MAJOR_VERSION_KHR,3,
                    EGL_CONTEXT_MINOR_VERSION_KHR,1,
                    EGL_NONE,
            };

    mEGLContext = eglCreateContext(mDisplay, mConfigList[SelectedConfigIndex], EGL_NO_CONTEXT, ContextAttributes);

    if(mEGLContext == EGL_NO_CONTEXT)
    {
        __android_log_print(ANDROID_LOG_ERROR, "[Vulkan]","eglCreateContext Failed!!");
        return false;
    }
    __android_log_print(ANDROID_LOG_INFO, "[Vulkan]","EGLContext Create Success!!!");

    if( eglMakeCurrent(mDisplay, mSurface, mSurface, mEGLContext) != EGL_TRUE)
    {
        __android_log_print(ANDROID_LOG_ERROR, "[Vulkan]","eglMakeCurrent Failed!!");

        return false;
    }

    glViewport(0,0,mWidth, mHeight);

    glClearColor(1,0,0,1);

    mInitialized=true;

    return true;
}

void OpenGLEngine::Tick(float Seconds)
{
    mElapsedSec += Seconds;
}

void OpenGLEngine::Clear()
{
    const float PI = 3.14159265358979323846f;

    mClearColorRed = (std::cosf(2 * PI * mElapsedSec) + 1.0f) * 0.5f;

    glClearColor(mClearColorRed, 0, 0, 1);

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}

void OpenGLEngine::SwapBuffer()
{
    eglSwapBuffers(mDisplay, mSurface);
}

void OpenGLEngine::OnPause()
{
    if(mDisplay != EGL_NO_DISPLAY)
    {
        eglMakeCurrent(mDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    }
    if(mSurface != EGL_NO_SURFACE)
    {
        eglDestroySurface(mDisplay, mSurface);
        mSurface = EGL_NO_SURFACE;
    }
}

void OpenGLEngine::OnResume(android_app* app)
{
    if(mDisplay == EGL_NO_DISPLAY)
    {
        return;
    }

    mSurface = eglCreateWindowSurface(mDisplay, mConfigList[mSelectedConfigIndex], app->window, nullptr);

    eglMakeCurrent(mDisplay,mSurface,mSurface, mEGLContext);
}

void OpenGLEngine::Terminate() {

}