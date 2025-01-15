// Copyright 2022 Google Inc. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <android/log.h>
#include <cassert>
#include <vector>
#include <vulkan_wrapper.h>
#include <game-activity/native_app_glue/android_native_app_glue.h>

#include "Vector.h"
#include "Matrix.h"
#include "OpenGLEngine.h"
#include "VulkanEngine.h"
#include "LogUtil.h"
#include <chrono>

RenderingEngine* GRenderingEngine = nullptr;

const bool bUseVulkan=false;

// Functions interacting with Android native activity
void android_main(struct android_app* state);

void HandleAppCmd(android_app* app, int32_t cmd);

// typical Android NativeActivity entry function
void android_main(struct android_app* app)
{
    Vector4::Vector4Test();
    Matrix::MatrixTest();

    app->onAppCmd = HandleAppCmd;

    int events;
    android_poll_source* source;

    auto prevTime = std::chrono::high_resolution_clock::now();

    do
    {
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - prevTime);

        prevTime = std::chrono::high_resolution_clock::now();

        if(GRenderingEngine)
        {
            GRenderingEngine->Tick(elapsed.count() / 1000.f);
            //GRenderingEngine->Clear();
            GRenderingEngine->SwapBuffer();
        }

        //if(GRenderingEngine)
        {
            //if (ALooper_pollAll(initialized_ ? 1 : 0, nullptr, &events, (void**)&source) >= 0)
            if (ALooper_pollAll(1, nullptr, &events, (void**)&source) >= 0)
            {
                if (source != NULL)
                {
                    source->process(app, source);
                }
            }
        }
    } while (app->destroyRequested == 0);
}


void HandleAppCmd(android_app* app, int32_t cmd)
{
    switch (cmd)
    {
        case APP_CMD_INIT_WINDOW:
            if(bUseVulkan)
            {
                GRenderingEngine = new VulkanEngine{};
                GRenderingEngine->Initialize(app);
            }
            else
            {
                GRenderingEngine = new OpenGLEngine{};
                GRenderingEngine->Initialize(app);
            }
            break;
        case APP_CMD_TERM_WINDOW:
            // The window is being hidden or closed, clean it up.
            if(GRenderingEngine)
            {
                GRenderingEngine->Terminate();
            }
            break;
        default:
            LOGI("event not handled: %d", cmd);
    }
}