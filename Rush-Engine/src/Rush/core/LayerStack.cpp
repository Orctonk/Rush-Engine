#include "LayerStack.h"

namespace Rush {

LayerStack::LayerStack() {
    m_LayerInsertIndex = 0;
 }

LayerStack::~LayerStack() { 
    for(auto l : m_Layers)
        delete l;
}

void LayerStack::PushLayer(Layer *layer){
    m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
    m_LayerInsertIndex++;
}

void LayerStack::PopLayer(Layer *layer){
    auto it = std::find(m_Layers.begin(),m_Layers.end(),layer);
    if(it != m_Layers.end()){
        m_Layers.erase(it);
        m_LayerInsertIndex--;
    }
}

void LayerStack::PushOverlay(Layer *layer){
    m_Layers.push_back(layer);
}

void LayerStack::PopOverlay(Layer *layer){
    auto it = std::find(m_Layers.begin(),m_Layers.end(),layer);
    if(it != end()){
        m_Layers.erase(it);
    }
}

}