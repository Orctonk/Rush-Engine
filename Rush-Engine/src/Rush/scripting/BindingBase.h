#ifndef __BINDINGBASE_H__
#define __BINDINGBASE_H__
#include <string>

namespace Rush {
namespace Bindings {

class BindingBase {
public:
    virtual std::string GetFilename() = 0;
    virtual void BindMethods() = 0;
    virtual void Init() {};
};

}
}
#endif // __BINDINGBASE_H__
