#include <Rush.h>
#include <iostream>
#include <string>

using namespace Rush;

float vertices[] = {
	 0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 	// back top right
	 0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,	// back bottom right
	-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,	// back bottom left
	-0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 	// back top left 
	 0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,	// front top right
	 0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,	// front bottom right
	-0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,	// front bottom left
	-0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f 	// front top left 
};
unsigned int indices[] = {  // note that we start from 0!
	0, 1, 3, 1, 2, 3,    // Back face
	3, 2, 7, 7, 2, 6,    // Left face
	4, 1, 0, 4, 5, 1,    // Back face
	3, 7, 0, 0, 7, 4,    // Top face
	6, 2, 5, 5, 2, 1,    // Bottom face
	7, 6, 5, 5, 4, 7    // Front face
}; 

class Sandbox : public Application{
private: 
	Unique<VertexArray> m_VA;
	Camera m_Cam;
	Shared<Shader> m_Shader;
	Shared<Texture> m_Texture;

	float a,b;

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
	Sandbox() : m_Cam(ProjectionMode::PERSPECTIVE) {}
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

		auto test = glm::vec4(0.0f,0.0f,0.0f,1.0f);
		m_Cam.SetPosition(glm::vec3(0.0f,1.0f,3.0f));
		m_Cam.SetRotation(90.0f,0.0f,0.0f);

		m_Shader = Shader::Create("res/test.glsl");

		m_VA = VertexArray::Create();
		Shared<VertexBuffer> vb = VertexBuffer::Create(vertices,sizeof(vertices));
		vb->SetInterleavedLayout({ShaderData::FLOAT3,ShaderData::FLOAT3,ShaderData::FLOAT2});
		Shared<IndexBuffer> ib = IndexBuffer::Create(indices,36);
		m_VA->AddVertexBuffer(vb);
		m_VA->SetIndexBuffer(ib);

		m_Texture = Texture::Create("res/container.jpg");
	}

	void Exit() override {
		
	}

	void Update() override{
		a++;
		m_Cam.SetRotation(a,20,0);
		m_Cam.SetPosition(glm::vec3(3*glm::cos(glm::radians(a)),1.0f,3*glm::sin(glm::radians(a))));
		Renderer::BeginScene(m_Cam);
		Renderer::Submit(m_Shader,m_VA,glm::mat4(1.0f));
		Renderer::EndScene();
		m_Window->Update();
	}
};

Application* Rush::CreateApplication(){
	return new Sandbox();
}