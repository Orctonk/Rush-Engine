#ifndef __PROFILERRESULTVIEW_H__
#define __PROFILERRESULTVIEW_H__

#include <Rush.h>

#include <vector>
#include <string>
#include <unordered_map>
#include <stdint.h>

struct Sample {
    uint32_t id;
    uint64_t start;
    uint64_t dur;
};

class ProfilerResultView {
private:
    bool m_Focused;
    std::string m_CurFile;
    float m_FirstDisplayedTime;
    float m_DisplayedDuration;
    std::vector<Sample> m_Samples;
    std::unordered_map<uint32_t, std::string> m_SampleNameMap;

    void LoadProfileResult(std::string path);
    void InsertSample(Sample &s);

    bool ScrollHandle(Rush::MouseScrollEvent &e);
    bool KeyPressHandle(Rush::KeyboardPressEvent &e);
    bool KeyRepeatHandle(Rush::KeyboardRepeatEvent &e);

public:
    bool enabled;

    ProfilerResultView();
    ~ProfilerResultView();

    void OnImguiRender();
    void OnEvent(Rush::Event &e);
};

#endif // __PROFILERESULTVIEW_H__
