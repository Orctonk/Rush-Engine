#include <Rush.h>
#include <iostream>
#include <string>

using namespace Rush;

class Sandbox : public Application{
private: 

	bool WindowCloseHandler(Events::WindowCloseEvent e){
		m_Running = false;
		return true;
	}

	bool KeyPressHandler(Events::KeyboardPressEvent e){
		if(e.keycode == RUSH_KEY_ESCAPE){
			m_Running = false;
			return true;
		}
		return false;
	}

public:
	Sandbox(){}
	~Sandbox(){}
	void Init() override {
		Logger::Info(	"Rush Version: " 
					+ 	std::to_string(RUSH_VERSION_MAJOR) 
					+ 	"." 
					+ 	std::to_string(RUSH_VERSION_MINOR));	

		Events::EventManager::GetInstance()
			.RegisterHandler<Events::WindowCloseEvent>(
				std::bind(
					&Sandbox::WindowCloseHandler, this,std::placeholders::_1));
					
		Events::EventManager::GetInstance()
			.RegisterHandler<Events::KeyboardPressEvent>(
				std::bind(
					&Sandbox::KeyPressHandler, this,std::placeholders::_1));
	}

	void Exit() override {
		
	}

	void Update() override{
		m_Window->Update();
	}
};

Application* Rush::CreateApplication(){
	return new Sandbox();
}