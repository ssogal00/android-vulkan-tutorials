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
#include "Matrix.h"
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

    CreateTestResources();

    return true;
}

void OpenGLEngine::Tick(float Seconds)
{
    mElapsedSec += Seconds;
    Render();
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

void OpenGLEngine::Terminate()
{

}

void OpenGLEngine::Render()
{
    glViewport(0,0, mWidth, mHeight);

    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(mProgram);

    const float ratio = mWidth / (float) mHeight;

    mRotateAngle = mElapsedSec * 0.01;

    Matrix Mvp = Matrix::Ortho(-ratio, ratio, -1,1,1,-1);
    Mvp = RotateZ(Mvp, TO_RADIAN(mRotateAngle));

    const GLint mvp_location = glGetUniformLocation(mProgram, "MVP");

    glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) &Mvp.mRows[0]);
    glBindVertexArray(mVertexArray);
    glDrawArrays(GL_TRIANGLES, 0, 3);

}

static const char* vertex_shader_text =
        "#version 100\n"
        "precision mediump float;\n"
        "uniform mat4 MVP;\n"
        "attribute vec3 vCol;\n"
        "attribute vec2 vPos;\n"
        "varying vec3 color;\n"
        "void main()\n"
        "{\n"
        "    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
        "    color = vCol;\n"
        "}\n";


static const char* fragment_shader_text =
        "#version 100\n"
        "precision mediump float;\n"
        "varying vec3 color;\n"
        "void main()\n"
        "{\n"
        "    gl_FragColor = vec4(color, 1.0);\n"
        "}\n";

void OpenGLEngine::CreateTestResources()
{
    glGenBuffers(1, &mVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

    mVertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(mVertexShader, 1, &vertex_shader_text, nullptr);
    glCompileShader(mVertexShader);

    mFragmentShader= glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(mFragmentShader, 1, &fragment_shader_text, nullptr);
    glCompileShader(mFragmentShader);

    mProgram = glCreateProgram();
    glAttachShader(mProgram, mVertexShader);
    glAttachShader(mProgram, mFragmentShader);
    glLinkProgram(mProgram);

    const GLint vpos_location = glGetAttribLocation(mProgram, "vPos");
    const GLint vcol_location = glGetAttribLocation(mProgram, "vCol");


    glGenVertexArrays(1, &mVertexArray);
    glBindVertexArray(mVertexArray);

    glEnableVertexAttribArray(vpos_location);
    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex), (void*) offsetof(Vertex, Position));
    glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex), (void*) offsetof(Vertex, Color));

}