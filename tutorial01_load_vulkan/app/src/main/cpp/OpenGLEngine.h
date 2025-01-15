
#pragma once

#include "RenderingEngine.h"
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <GLES3/gl32.h>
#include "Vector.h"

struct Vertex
{
    Vector2 Position;
    Vector3 Color;
};

static const Vertex Vertices[3]
{
    { { -0.6f, -0.4f }, { 1.f, 0.f, 0.f } },
    { {  0.6f, -0.4f }, { 0.f, 1.f, 0.f } },
    { {   0.f,  0.6f }, { 0.f, 0.f, 1.f } }
};

class OpenGLEngine : public RenderingEngine
{
public:
    bool Initialize(struct android_app* app ) override;
    void Clear() override;
    void SwapBuffer() override;
    void Tick(float Seconds) override;
    void OnPause() override;
    void OnResume(class android_app* app) override;
    void Terminate() override;

private:

    void CreateTestResources();
    void Render();
    EGLDisplay mDisplay = EGL_NO_DISPLAY;
    EGLSurface  mSurface = EGL_NO_SURFACE;
    EGLContext mEGLContext = EGL_NO_CONTEXT;
    int mWidth=0;
    int mHeight=0;
    float mClearColorRed=0;
    float mElapsedSec=0;
    EGLConfig* mConfigList;
    int mSelectedConfigIndex = -1;

    //
    GLuint mVertexBuffer;
    GLuint mVertexShader;
    GLuint mFragmentShader;
    GLuint mProgram;
    GLuint mVertexArray;
    float mRotateAngle=0;
};

