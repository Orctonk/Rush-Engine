#ifndef __IMGUILAYER_H__
#define __IMGUILAYER_H__

#include "Rush/core/Core.h"
#include "Rush/core/Layer.h"

namespace Rush {

class RUSH_API ImguiLayer : public Layer {
public:
    ImguiLayer();
    ~ImguiLayer();

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnUpdate() override;
    
    virtual void OnEvent(Event &e) override;
};

}

#endif // __IMGUILAYER_H__
