#include "Time.binding.h"
#include "Rush/core/Time.h"

namespace Rush {
namespace Bindings {

void Time::BindMethods() {
    mono_add_internal_call("Rush.Time::Delta", (const void *)Rush::Time::Delta);
    mono_add_internal_call("Rush.Time::FixedDelta", (const void *)Rush::Time::FixedDelta);
    mono_add_internal_call("Rush.Time::TimeScale", (const void *)Rush::Time::TimeScale);
    mono_add_internal_call("Rush.Time::ProgramTimeMillis", (const void *)Rush::Time::ProgramTimeMillis);
    mono_add_internal_call("Rush.Time::ProgramTimeMicros", (const void *)Rush::Time::ProgramTimeMicros);
    mono_add_internal_call("Rush.Time::SetTimeScale", (const void *)Rush::Time::SetTimeScale);
    mono_add_internal_call("Rush.Time::SetFixedDelta", (const void *)Rush::Time::SetFixedDelta);
}

} // namespace Bindings
} // namespace Rush