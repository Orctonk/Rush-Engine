#ifndef __SANDBOX_LAYER_H__
#define __SANDBOX_LAYER_H__

#include <Rush.h>
#include "CameraController.h"

class SandboxLayer : public Rush::Layer {
private:
	Rush::Scene m_Scene;
	Rush::Entity m_Camera;
	CameraController m_CC;

	float m_AccumulatedTime;
	bool m_Step;

	bool OnKeyDown(Rush::KeyboardPressEvent& e);
public:
	SandboxLayer(){}
	~SandboxLayer() {}

	virtual void OnAttach() override;
	virtual void OnDetach() override {}
	virtual void OnUpdate() override;
	virtual void OnImguiRender() override {}
	virtual void OnEvent(Rush::Event& e) override;
};

#endif // __SANDBOX_LAYER_H__