#ifndef __DEBUGLAYER_H__
#define __DEBUGLAYER_H__
#include "Rush/core/Layer.h"

#define DEBUG_FPS_SAMPLES 300

namespace Rush {

class RUSH_API DebugLayer : public Layer {
    float m_FrameTimes[DEBUG_FPS_SAMPLES];
    int m_FrameTimeOffset;
    static bool s_ShowWindow;
public:

    DebugLayer();
    ~DebugLayer();

    void OnAttach() override;
    void OnDetach() override;
    void OnUpdate() override;
    void OnEvent(Event &e) override;
    void OnImguiRender() override;

    static bool GetShown();
    static void SetShown(bool show);
};

}
#endif // __DEBUGLAYER_H__
