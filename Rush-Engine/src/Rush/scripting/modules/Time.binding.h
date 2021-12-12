#ifndef __TIME_BINDING_H__
#define __TIME_BINDING_H__

#include "../BindingBase.h"
#include "../ScriptingBackend.h"
#include <stdint.h>

namespace Rush {
namespace Bindings {

class Time : public BindingBase {
public:
    virtual std::string GetFilename() override { return ScriptingBackend::BindingsBaseDir + "modules/Time.cs"; };
    virtual void BindMethods() override;
};

} // namespace Bindings
} // namespace Rush

#endif // __TIME_BINDING_H__
