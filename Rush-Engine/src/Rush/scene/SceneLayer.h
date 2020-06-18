#ifndef __SCENELAYER_H__
#define __SCENELAYER_H__

#include "Rush/core/Layer.h"

namespace Rush {

class SceneLayer : public Layer {
public: 
    SceneLayer();
    ~SceneLayer();

    void OnAttach() override;
    void OnDetach() override;
    void OnUpdate() override;
    void OnEvent(Event &e) override;
    void OnImguiRender() override;
};

}

#endif // __SCENELAYER_H__
