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
		Rush::Logger::Info(	"Rush Version: " 
					+ 	std::to_string(RUSH_VERSION_MAJOR) 
					+ 	"." 
					+ 	std::to_string(RUSH_VERSION_MINOR));
		PushOverlay(new Rush::ImguiLayer());
		PushLayer(new SandboxLayer());
		GetWindow()->SetMaximized(false);
		GetWindow()->Resize(720, 640);
		GetWindow()->Move(1930, 30);
	}

	void Exit() override {
		
	}

    void Update() override {

    }
};

Rush::Application* Rush::CreateApplication(){
	return new Sandbox();
}