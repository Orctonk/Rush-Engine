#include "Rushpch.h"
#include "Profiler.h"

#include "Rush/core/Time.h"

namespace Rush {

std::ofstream Profiler::m_ProfFile;

void Profiler::Init(){
    m_ProfFile.open("ProfilerData.json");

    m_ProfFile << "[{}"; 
}

void Profiler::Shutdown(){
    m_ProfFile << "]";

    m_ProfFile.close();
}

void Profiler::SubmitSample(ProfilerSample &sample){
    m_ProfFile << ",";
    m_ProfFile << std::endl;
    m_ProfFile << "{\"name\":\""    << sample.scope << "\",";
    m_ProfFile << "\"beg\":"        << sample.start     << ",";
    m_ProfFile << "\"dur\":"        << (sample.end - sample.start) << "}";
}

Sampler::Sampler(const char *scope){
    m_Sample.scope = scope;
    m_Sample.start = Time::ProgramTimeMicros();
}

Sampler::~Sampler(){
    m_Sample.end = Time::ProgramTimeMicros();
    Profiler::SubmitSample(m_Sample);
}

}
