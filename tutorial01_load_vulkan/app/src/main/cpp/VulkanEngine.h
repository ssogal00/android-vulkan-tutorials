
#pragma once


#include <vulkan_wrapper.h>
#include "RenderingEngine.h"

class VulkanEngine : public RenderingEngine
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

// Global variables
    VkInstance mVulkanInstance;
    VkPhysicalDevice mTutorialGpu;
    VkDevice mDevice;
    VkSurfaceKHR mSurface;
    bool mInitialized=false;
};

