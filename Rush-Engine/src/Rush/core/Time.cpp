#include "Rushpch.h"
#include "Time.h"

namespace Rush {

using namespace std::chrono;

steady_clock::time_point Time::s_LastUpdate;
steady_clock::time_point Time::s_Startup;

double Time::s_DeltaTime = 0;
double Time::s_FixedDeltaTime;
double Time::s_TimeScale;

void Time::Init() {
    s_TimeScale = 1.0f;
    s_Startup = steady_clock::now();
    s_LastUpdate = s_Startup;
    s_DeltaTime = 0.0f;
    s_FixedDeltaTime = 0.02f;
    s_TimeScale = 1.0f;
}

void Time::Shutdown() { }

void Time::Update() {
    steady_clock::time_point now = steady_clock::now();
#ifdef RUSH_DEBUG
    if (duration_cast<duration<double>>(now - s_LastUpdate).count() > 1.0)
        s_LastUpdate = (now - duration_cast<steady_clock::duration>(duration<double>(1.0 / 60.0)));
#endif
    s_DeltaTime = duration_cast<duration<double>>(now-s_LastUpdate).count() * s_TimeScale;
    s_LastUpdate = now;
}

uint64_t Time::ProgramTimeMillis() {
    return duration_cast<duration<uint64_t,std::milli>>(steady_clock::now() - s_Startup).count();
}

uint64_t Time::ProgramTimeMicros() {
    return duration_cast<duration<uint64_t,std::micro>>(steady_clock::now() - s_Startup).count();
}
    
} // namespace Rush
