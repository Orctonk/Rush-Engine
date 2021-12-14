#ifndef __BINDINGBASE_H__
#define __BINDINGBASE_H__
#include <string>
#include <vector>

namespace Rush {
namespace Bindings {

typedef std::vector<std::string> FileList;

class BindingBase {
public:
    virtual FileList GetFilenames() = 0;
    virtual void BindMethods() = 0;
    virtual void Init() { }
};

} // namespace Bindings
} // namespace Rush
#endif // __BINDINGBASE_H__
