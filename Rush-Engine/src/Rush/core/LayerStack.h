#ifndef __LAYERSTACK_H__
#define __LAYERSTACK_H__

#include "Core.h"
#include "Layer.h"

#include <vector>

namespace Rush {

class RUSH_API LayerStack {
public:
    using iterator = typename std::vector<Layer *>::const_iterator;
    using const_iterator = typename std::vector<Layer *>::const_iterator;

    iterator begin() { return m_Layers.begin(); }
    const_iterator begin() const { return m_Layers.begin(); }

    iterator end() { return m_Layers.end(); }
    const_iterator end() const { return m_Layers.end(); }

    LayerStack();
    ~LayerStack();

    void PushLayer(Layer *layer);
    void PopLayer(Layer *layer);
    void PushOverlay(Layer *layer);
    void PopOverlay(Layer *layer);

private:
    int m_LayerInsertIndex;
    std::vector<Layer *> m_Layers;
};

} // namespace Rush

#endif // __LAYERSTACK_H__
