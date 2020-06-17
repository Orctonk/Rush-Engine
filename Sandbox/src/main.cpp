#include <Rush.h>
#include <iostream>
#include <string>

#include <glm/gtc/type_ptr.hpp>

using namespace Rush;

float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
};
unsigned int indices[] = {  // note that we start from 0!
	0, 1, 2, 3, 4, 5,    // Back face
	6, 7, 8, 9, 10, 11,    // Left face
	12, 13, 14, 15, 16, 17,    // Back face
	18, 19, 20, 21, 22, 23,    // Top face
	24, 25, 26, 27, 28, 29,    // Bottom face
	30, 31, 32, 33, 34, 35    // Front face
}; 

class Sandbox : public Application{
private: 
	Unique<VertexArray> m_VA;
	Camera m_Cam;
	Shared<Shader> m_Shader;
	Shared<Texture> m_Texture;
	PointLight m_Light;

	float a,b;

public:
	Sandbox() : m_Cam(ProjectionMode::PERSPECTIVE, 1024.f /768.0f) {}
	~Sandbox(){}
	void Init() override {
		Logger::Info(	"Rush Version: " 
					+ 	std::to_string(RUSH_VERSION_MAJOR) 
					+ 	"." 
					+ 	std::to_string(RUSH_VERSION_MINOR));	

		m_Cam.SetPosition(glm::vec3(0.0f,1.0f,3.0f));
		m_Cam.SetRotation(90.0f,20.0f,0.0f);

		m_Shader = Shader::Create("res/test.glsl");

		m_VA = VertexArray::Create();
		Shared<VertexBuffer> vb = VertexBuffer::Create(vertices,sizeof(vertices));
		vb->SetInterleavedLayout({ShaderData::FLOAT3,ShaderData::FLOAT3,ShaderData::FLOAT2});
		Shared<IndexBuffer> ib = IndexBuffer::Create(indices,36);
		m_VA->AddVertexBuffer(vb);
		m_VA->SetIndexBuffer(ib);

		m_Texture = Texture::Create("res/container.jpg");

		m_Light.position = glm::vec3(1.0f,1.0f,1.0f);

		m_Light.ambient = glm::vec3(0.1f);
		m_Light.diffuse = glm::vec3(1.0f);
		m_Light.specular = glm::vec3(1.0f);

		m_Light.constant = 1.0f;
		m_Light.linear = .09f;
		m_Light.quadratic = .032f;

		bool active = true;
		m_Shader->SetUniform("u_PLights[0].activated",ShaderData::BOOL, &active);
		m_Shader->SetUniform("u_PLights[0].position",ShaderData::FLOAT3, glm::value_ptr(m_Light.position));
		m_Shader->SetUniform("u_PLights[0].constant",ShaderData::FLOAT, &m_Light.constant);
		m_Shader->SetUniform("u_PLights[0].linear",ShaderData::FLOAT, &m_Light.linear);
		m_Shader->SetUniform("u_PLights[0].quadratic",ShaderData::FLOAT, &m_Light.quadratic);
		m_Shader->SetUniform("u_PLights[0].ambient",ShaderData::FLOAT3, glm::value_ptr(m_Light.ambient));
		m_Shader->SetUniform("u_PLights[0].diffuse",ShaderData::FLOAT3, glm::value_ptr(m_Light.diffuse));
		m_Shader->SetUniform("u_PLights[0].specular",ShaderData::FLOAT3, glm::value_ptr(m_Light.specular));

		PushOverlay(new ImguiLayer());
	}

	void Exit() override {
		
	}

	void Update() override{
		a++;
		m_Cam.SetRotation(a,20.0f,0);
		m_Cam.SetPosition(glm::vec3(3*glm::cos(glm::radians(a)),1.0f,3*glm::sin(glm::radians(a))));
		Renderer::BeginScene(m_Cam);
		Renderer::Submit(m_Shader,m_VA,glm::mat4(1.0f));
		Renderer::EndScene();
		
	}
};

Application* Rush::CreateApplication(){
	return new Sandbox();
}