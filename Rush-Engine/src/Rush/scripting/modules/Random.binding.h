#ifndef __RANDOM_BINDING_H__
#define __RANDOM_BINDING_H__

#include "../BindingBase.h"
#include "../ScriptingBackend.h"

namespace Rush {
namespace Bindings {

class Random : public BindingBase {
public:
    virtual FileList GetFilenames() override { return { ScriptingBackend::BindingsBaseDir + "modules/Random.cs" }; };
    virtual void BindMethods() override;
};

} // namespace Bindings
} // namespace Rush

#endif // __RANDOM_BINDING_H__
