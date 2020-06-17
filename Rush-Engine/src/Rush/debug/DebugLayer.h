#ifndef __DEBUGLAYER_H__
#define __DEBUGLAYER_H__
#include "Rush/core/Layer.h"

namespace Rush {

class DebugLayer : public Layer {
    float m_FrameTimes[60];
    int m_FrameTimeOffset;
public:
    DebugLayer();
    ~DebugLayer();

    void OnAttach() override;
    void OnDetach() override;
    void OnUpdate() override;
    void OnEvent(Event &e) override;
    void OnImguiRender() override;
};

}
#endif // __DEBUGLAYER_H__
