#ifndef __LAYER_H__
#define __LAYER_H__

#include "Rush/events/Event.h"

namespace Rush {
    
class Layer {
public:
    Layer();
    virtual ~Layer();

    virtual void OnAttach() = 0;
    virtual void OnDetach() = 0;
    virtual void OnUpdate() = 0;
    virtual void OnEvent(Event &e);
};

} // namespace Rush

#endif // __LAYER_H__