#include "Time.h"

#include <chrono>

namespace Rush {

using namespace std::chrono;

steady_clock::time_point Time::s_LastUpdate;

double Time::s_DeltaTime = 0;
double Time::s_FixedDeltaTime;
double Time::s_TimeScale;

void Time::Init() {
    s_TimeScale = 1.0f;
    s_LastUpdate = steady_clock::now();
    s_DeltaTime = 0.0f;
    s_FixedDeltaTime = 0.02f;
    s_TimeScale = 1.0f;
}

void Time::Shutdown() { }

void Time::Update() {
    steady_clock::time_point now = steady_clock::now();
    s_DeltaTime = duration_cast<duration<double>>(now-s_LastUpdate).count() * s_TimeScale;
    s_LastUpdate = now;
}
    
} // namespace Rush
