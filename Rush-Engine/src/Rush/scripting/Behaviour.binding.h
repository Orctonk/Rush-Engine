#ifndef __BEHAVIOUR_BINDING_H__
#define __BEHAVIOUR_BINDING_H__

#include "components/Component.binding.h"

#include <mono/metadata/object.h>

namespace Rush {
namespace Bindings {

class Behaviour : public Component {
public:
    COMPONENT_BODY(Behaviour, ScriptingBackend::BindingsBaseDir + "Behaviour.cs")
};

} // namespace Bindings
} // namespace Rush

#endif // __BEHAVIOUR_BINDING_H__
