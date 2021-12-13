#include "Input.binding.h"
#include "Rush/core/Input.h"

namespace Rush {
namespace Bindings {

void Input::BindMethods() {
    mono_add_internal_call("Rush.Input::KeyDown", Rush::Input::KeyDown);
    mono_add_internal_call("Rush.Input::MousePressed", Rush::Input::MousePressed);
}

} // namespace Bindings
} // namespace Rush