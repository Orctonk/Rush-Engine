#ifndef __GLM_BINDING_H__
#define __GLM_BINDING_H__

#include "../BindingBase.h"
#include "../ScriptingBackend.h"

namespace Rush {
namespace Bindings {

class GLM : public BindingBase {
private:
    const std::string GLMBindingDir = RUSH_GLM_BINDINGS_DIR;

public:
    virtual FileList GetFilenames() override;
    virtual void BindMethods() override;
};

} // namespace Bindings
} // namespace Rush

#endif // __GLM_BINDING_H__
