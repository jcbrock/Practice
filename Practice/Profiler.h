#pragma once
#include <chrono>
// TODO - could templatize this if I wanted something other than floats
/*class Profiler
{
public:
    Profiler(int bytesInAPage);
    ~Profiler() = default;

    void AddDataPoint(float v);
    void TickFrame();

private:
    int index = 0;
    int runningAvgSize = 10;
    unsigned char mData[4906]; //uh, minus the other member vars?
};*/

class Profiler
{
public:
    Profiler();
    ~Profiler();

    //void AddDataPoint(float v);
    void StartFrame();
    void EndFrame();
    float GetRunningAvg() const { return mRunningAverage;  }
    float GetRunningAvg2() const { return mRunningAverage2; }

private:
    float mRunningAverage = 0.f;
    float mRunningAverage2 = 0.f;
    int mIndex = 0;
    std::chrono::high_resolution_clock::time_point mFrameStartTime;
    // would std::chrono::system_clock::duration be better?
    long long mFrameTimes[100] = { 0 };
    long long mFrameTimes2[100] = { 0 };
};