
#pragma once

class RenderingEngine
{
public:
    virtual bool Initialize(struct android_app* app )=0;
    virtual void Clear()=0;
    virtual void SwapBuffer()=0;
    virtual void Tick(float Seconds)=0;
    virtual void Terminate() =0;
    virtual void OnPause()=0;
    virtual void OnResume(class android_app* app)=0;
};

