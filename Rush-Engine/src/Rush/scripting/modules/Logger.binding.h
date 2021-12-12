#ifndef __LOGGER_BINDINGS_H__
#define __LOGGER_BINDINGS_H__

#include "../BindingBase.h"
#include "../ScriptingBackend.h"

namespace Rush {
namespace Bindings {

class Logger : public BindingBase {
    static void Log(int level, MonoString *message);

public:
    virtual std::string GetFilename() override { return ScriptingBackend::BindingsBaseDir + "modules/Logger.cs"; }
    virtual void BindMethods() override;
};

} // namespace Bindings
} // namespace Rush

#endif // __LOGGER_BINDINGS_H__
