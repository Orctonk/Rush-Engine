#ifndef __INPUT_BINDING_H__
#define __INPUT_BINDING_H__

#include "../BindingBase.h"
#include "../ScriptingBackend.h"

namespace Rush {
namespace Bindings {

class Input : public BindingBase {
public:
    virtual FileList GetFilenames() override { return { ScriptingBackend::BindingsBaseDir + "modules/Input.cs" }; };
    virtual void BindMethods() override;
};

} // namespace Bindings
} // namespace Rush

#endif // __INPUT_BINDING_H__
