#ifndef __PARTICLECOMPONENT_H__
#define __PARTICLECOMPONENT_H__

#include "Rush/graphics/particles/ParticleSystem.h"

struct RUSH_API ParticleComponent {
    Rush::ParticleSystem particleSystem;
    Rush::ParticleProperties emissionProperties;
    float emissionRate      {0.0f};
    float timeSinceEmission {0.0f};
};

#endif // __PARTICLECOMPONENT_H__
