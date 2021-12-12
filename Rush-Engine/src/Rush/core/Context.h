#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "Core.h"

namespace Rush {

class RUSH_API Context {
public:
    Context(){};
    virtual ~Context(){};

    virtual void Init() = 0;
    virtual void MakeCurrent() = 0;
    virtual void SwapBuffer() = 0;
};

} // namespace Rush

#endif // __CONTEXT_H__
