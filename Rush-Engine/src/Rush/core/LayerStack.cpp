#include "LayerStack.h"

namespace Rush {

LayerStack::LayerStack() { }

LayerStack::~LayerStack() { 
    for(auto l : m_Layers)
        delete l;
}

void LayerStack::PushLayer(Layer *layer){
    m_LayersEnd = m_Layers.emplace(m_LayersEnd, layer);
}

void LayerStack::PopLayer(Layer *layer){
    auto it = std::find(begin(),m_LayersEnd,layer);
    if(it != m_LayersEnd){
        m_Layers.erase(it);
        m_LayersEnd--;
    }
}

void LayerStack::PushOverlay(Layer *layer){
    m_Layers.push_back(layer);
}

void LayerStack::PopOverlay(Layer *layer){
    auto it = std::find(m_LayersEnd,end(),layer);
    if(it != end()){
        m_Layers.erase(it);
    }
}

}