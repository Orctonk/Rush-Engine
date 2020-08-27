#include "Random.h"

namespace Rush {

std::default_random_engine Random::s_RE;

void Random::Init(){
    std::random_device r;
    SetSeed(r());
}

void Random::SetSeed(uint32_t seed){
    s_RE = std::default_random_engine(seed);
}

int Random::GetInt(){
    std::uniform_int_distribution<> dist(INT32_MIN,INT32_MAX);
    return dist(s_RE);
}

int Random::GetInt(int max){
    std::uniform_int_distribution<> dist(0,max);
    return dist(s_RE);
}

int Random::GetInt(int min, int max){
    std::uniform_int_distribution<> dist(min,max);
    return dist(s_RE);
}

float Random::GetFloat(){
    std::uniform_real_distribution<> dist(0.0f,1.0f);
    return dist(s_RE);
}

float Random::GetFloat(float max){
    std::uniform_real_distribution<> dist(0.0f,max);
    return dist(s_RE);
}

float Random::GetFloat(float min, float max){
    std::uniform_real_distribution<> dist(min,max);
    return dist(s_RE);
}

}