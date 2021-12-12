#include "Time.binding.h"
#include "Rush/core/Time.h"

namespace Rush {
namespace Bindings {

void Time::BindMethods() {
    mono_add_internal_call("Rush.Time::Delta", Rush::Time::Delta);
    mono_add_internal_call("Rush.Time::FixedDelta", Rush::Time::FixedDelta);
    mono_add_internal_call("Rush.Time::TimeScale", Rush::Time::TimeScale);
    mono_add_internal_call("Rush.Time::ProgramTimeMillis", Rush::Time::ProgramTimeMillis);
    mono_add_internal_call("Rush.Time::ProgramTimeMicros", Rush::Time::ProgramTimeMicros);
    mono_add_internal_call("Rush.Time::SetTimeScale", Rush::Time::SetTimeScale);
    mono_add_internal_call("Rush.Time::SetFixedDelta", Rush::Time::SetFixedDelta);
}

} // namespace Bindings
} // namespace Rush