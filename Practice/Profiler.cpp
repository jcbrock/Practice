#include "Profiler.h"

Profiler::Profiler() = default;
Profiler::~Profiler() = default;

// void Profiler::AddDataPoint(float v)
// {
//     mFrameTimes
// }

void Profiler::StartFrame()
{
    mFrameStartTime = std::chrono::high_resolution_clock::now();
}

void Profiler::EndFrame()
{
    //std::chrono::system_clock::duration<float, std::chrono::milliseconds> duration;
    //duration = std::chrono::system_clock::now() - mFrameStartTime;
    auto t1 = std::chrono::high_resolution_clock::now() - mFrameStartTime;
    std::chrono::nanoseconds d = std::chrono::duration_cast<std::chrono::nanoseconds>(t1);
    std::chrono::milliseconds d2 = std::chrono::duration_cast<std::chrono::milliseconds>(t1);
    if (d.count() < 0)
    {
        int b = 0;
        b++;
    }
    mFrameTimes[mIndex] = d.count();
    mFrameTimes2[mIndex] = d2.count();
    //mFrameStartTime = std::chrono::system_clock::;
    ++mIndex;
    if (mIndex >= 100)
    {
        mIndex = 0;
    }
    long long runningTotal = 0;
    for (const auto& f : mFrameTimes)
    {
        runningTotal += f;
    }
    mRunningAverage = runningTotal / 100;

    long long runningTotal2 = 0;
    for (const auto& f : mFrameTimes2)
    {
        runningTotal2 += f;
    }
    mRunningAverage2 = runningTotal2 / 100;
}