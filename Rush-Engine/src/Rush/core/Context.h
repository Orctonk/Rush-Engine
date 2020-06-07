#ifndef __CONTEXT_H__
#define __CONTEXT_H__

namespace Rush {

class Context {
public:
    Context() {};
    virtual ~Context() {};

    virtual void Init() = 0;
    virtual void MakeCurrent() = 0;
    virtual void SwapBuffer() = 0;
};

}

#endif // __CONTEXT_H__
