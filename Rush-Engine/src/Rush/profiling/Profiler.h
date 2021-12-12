#ifndef __PROFILER_H__
#define __PROFILER_H__

#include <ostream>
#include <stdint.h>
#include <string.h>

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

namespace Rush {

struct ProfilerSample {
    const char *scope;
    uint64_t start;
    uint64_t end;
};

class Profiler {
private:
    static std::ofstream m_ProfFile;

public:
    static void Init();
    static void Shutdown();
    static void SubmitSample(ProfilerSample &sample);
};

class Sampler {
private:
    ProfilerSample m_Sample;

public:
    Sampler(const char *scope);
    ~Sampler();
};

//#define RUSH_PROFILE
#ifdef RUSH_PROFILE
    #define __PROFILE(scope) Rush::Sampler __##scope_sample__(scope)
#else
    #define __PROFILE(scope)
#endif

#define RUSH_FUNC_SIG             __PRETTY_FUNCTION__
#define RUSH_PROFILE_FUNCTION()   __PROFILE(RUSH_FUNC_SIG)
#define RUSH_PROFILE_SCOPE(scope) __PROFILE(scope)

} // namespace Rush
#endif // __PROFILER_H__
