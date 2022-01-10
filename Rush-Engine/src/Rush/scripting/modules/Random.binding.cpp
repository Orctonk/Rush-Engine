#include "Random.binding.h"
#include "Rush/core/Random.h"

namespace Rush {
namespace Bindings {

void Random::BindMethods() {
    mono_add_internal_call("Rush.Random::SetSeed", (const void *)Rush::Random::SetSeed);
    mono_add_internal_call("Rush.Random::GetInt()", (const void *)static_cast<int (*)()>(Rush::Random::GetInt));
    mono_add_internal_call("Rush.Random::GetInt(int)", (const void *)static_cast<int (*)(int)>(Rush::Random::GetInt));
    mono_add_internal_call("Rush.Random::GetInt(int,int)", (const void *)static_cast<int (*)(int, int)>(Rush::Random::GetInt));
    mono_add_internal_call("Rush.Random::GetFloat()", (const void *)static_cast<float (*)()>(Rush::Random::GetFloat));
    mono_add_internal_call("Rush.Random::GetFloat(float)", (const void *)static_cast<float (*)(float)>(Rush::Random::GetFloat));
    mono_add_internal_call("Rush.Random::GetFloat(float,float)", (const void *)static_cast<float (*)(float, float)>(Rush::Random::GetFloat));
}

} // namespace Bindings
} // namespace Rush