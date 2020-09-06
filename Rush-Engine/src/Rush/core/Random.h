#ifndef __RANDOM_H__
#define __RANDOM_H__

#include "Core.h"

#include <stdint.h>
#include <random>

namespace Rush {

class RUSH_API Random {
private:
    static std::default_random_engine s_RE;
    Random();
public:
    static void Init();
    static void SetSeed(uint32_t seed);

    static int GetInt();
    static int GetInt(int max);
    static int GetInt(int min, int max);

    static float GetFloat();
    static float GetFloat(float max);
    static float GetFloat(float min, float max);
};

}

#endif // __RANDOM_H__
