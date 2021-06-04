#include <Rush.h>
#include <Rush/core/EntryPoint.h>
#include <iostream>
#include <string>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>

#include "SandboxLayer.h"

class Sandbox : public Rush::Application {
public:
	Sandbox(){}
	~Sandbox(){}
	void Init() override {
		RUSH_LOG_INFO("Rush Version: {}.{}", RUSH_VERSION_MAJOR, RUSH_VERSION_MINOR);
		PushOverlay(new Rush::ImguiLayer());
		PushLayer(new SandboxLayer());
	}

	void Exit() override {
		
	}

    void Update() override {

    }
};

Rush::Application* Rush::CreateApplication(){
	return new Sandbox();
}