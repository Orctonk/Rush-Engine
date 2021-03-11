#include <Rush.h>
#include <Rush/core/EntryPoint.h>
#include <iostream>
#include <string>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>

using namespace Rush;

float vertices[] = {
        // positions          // normals          // Tangents       // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, 0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, 0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, 0.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, 0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, 0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, 0.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, 0.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, 0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, 0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, 0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, 0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, 0.0f,  1.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, 0.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, 0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, 0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, 0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, 0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, 0.0f,  1.0f,  1.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, 0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, 0.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, 0.0f,  1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, 0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, 0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, 0.0f,  0.0f,  1.0f,
												  
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, 0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, 0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, 0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, 0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, 0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, 0.0f,  0.0f,  1.0f,
												 
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f, 0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f, 0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f, 0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f, 0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f, 0.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f, 0.0f,  0.0f,  1.0f,
};
unsigned int indices[] = {  // note that we start from 0!
	0, 1, 2, 3, 4, 5,		// Back face
	6, 7, 8, 9, 10, 11,		// Front face
	12, 13, 14, 15, 16, 17, // Left face
	18, 19, 20, 21, 22, 23, // Right face
	24, 25, 26, 27, 28, 29, // Bottom face
	30, 31, 32, 33, 34, 35  // Top face
}; 

class Sandbox : public Application{
private: 
	Scene m_Scene;
	Entity m_Box;
	Entity m_Camera;
	Entity m_Light;

	float a,b;

public:
	Sandbox(){}
	~Sandbox(){}
	void Init() override {
		Logger::Info(	"Rush Version: " 
					+ 	std::to_string(RUSH_VERSION_MAJOR) 
					+ 	"." 
					+ 	std::to_string(RUSH_VERSION_MINOR));	

		m_Camera = m_Scene.NewEntity("Camera");
		m_Camera.GetComponent<TransformComponent>().Translate({ 0.0f,1.0f,3.0f });
		m_Camera.GetComponent<TransformComponent>().Rotate(glm::toQuat(glm::eulerAngleXYZ(90.0f,0.0f,0.0f )));
		Camera c((float)GetWindow()->GetWidth() / GetWindow()->GetHeight(), 90.0f);
		m_Camera.AddComponent<CameraComponent>(true, c);

		m_Box = m_Scene.NewEntity("Test Entity");
		SubMesh sm;
		sm.vertices = VertexArray::Create();
		Shared<VertexBuffer> vb = VertexBuffer::Create(vertices,sizeof(vertices));
		vb->SetInterleavedLayout({ShaderData::FLOAT3,ShaderData::FLOAT3,ShaderData::FLOAT3,ShaderData::FLOAT2});
		Shared<IndexBuffer> ib = IndexBuffer::Create(indices,36);
		sm.vertices->AddVertexBuffer(vb);
		sm.vertices->SetIndexBuffer(ib);
		sm.meshName = "Box";
		sm.material = CreateShared<Material>();
		sm.material->materialShader = Shader::Create("res/shaders/materialShader.glsl");
		sm.material->diffuseTexture = Texture::Create("res/container.jpg");
		Shared<RootMesh> rm = CreateShared<RootMesh>();
		rm->name = "box";
		rm->submeshes = { sm };
		m_Box.AddComponent<MeshRendererComponent>(rm);

		m_Light = m_Scene.NewEntity("Light");
		LightComponent &l = m_Light.AddComponent<LightComponent>();
		l.ambient = glm::vec3(0.1f);
		l.diffuse = glm::vec3(1.0f);
		l.specular = glm::vec3(1.0f);

		l.constant = 1.0f;
		l.linear = .09f;
		l.quadratic = .032f;

		PushOverlay(new ImguiLayer());
	}

	void Exit() override {
		
	}

	void Update() override{
		float time = Time::ProgramTimeMillis() / 1000.0f;
		auto& t = m_Camera.GetComponent<TransformComponent>();
		t.SetTranslation({ sin(time) * 3.0f,0.0f,cos(time) * 3.0f });
		t.SetRotation(glm::toQuat(glm::eulerAngleY(time)));
		m_Scene.Render();
	}
};

Application* Rush::CreateApplication(){
	return new Sandbox();
}