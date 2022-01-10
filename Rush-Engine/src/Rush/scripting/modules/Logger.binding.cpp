#include "Logger.binding.h"

#include "Rush/core/Logger.h"

namespace Rush {
namespace Bindings {

void Logger::Log(int level, MonoString *message) {
    char *msg = mono_string_to_utf8(message);
    switch (level) {
    case 0:
        Rush::Logger::Error(msg);
    case 1:
        Rush::Logger::Warning(msg);
    case 2:
        Rush::Logger::Info(msg);
    case 3:
        Rush::Logger::Trace(msg);
    }
}

void Logger::BindMethods() {
    mono_add_internal_call("Rush.Logger::Log", (const void *)Log);
}

} // namespace Bindings
} // namespace Rush