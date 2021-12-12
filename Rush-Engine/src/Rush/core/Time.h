#ifndef __TIME_H__
#define __TIME_H__

#include "Core.h"

#include <chrono>
#include <stdint.h>

namespace Rush {

class RUSH_API Time {
private:
    static std::chrono::steady_clock::time_point s_LastUpdate;
    static std::chrono::steady_clock::time_point s_Startup;

    static double s_DeltaTime;
    static double s_FixedDeltaTime;
    static double s_TimeScale;

public:
    Time() = delete;
    ~Time() = delete;

    static void Init();
    static void Shutdown();
    static void Update();
    static double Delta() { return s_DeltaTime; }
    static double FixedDelta() { return s_FixedDeltaTime; }
    static double TimeScale() { return s_TimeScale; }
    static uint64_t ProgramTimeMillis();
    static uint64_t ProgramTimeMicros();
    static void SetTimeScale(double timescale) { s_TimeScale = timescale; }
    static void SetFixedDelta(double fixeddelta) { s_FixedDeltaTime = fixeddelta; }
};

} // namespace Rush

#endif // __TIME_H__
