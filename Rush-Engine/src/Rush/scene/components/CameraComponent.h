#ifndef __CAMERACOMPONENT_H__
#define __CAMERACOMPONENT_H__

#include "../Camera.h"
#include "Rush/core/Core.h"
#include "Rush/graphics/Cubemap.h"
#include "Rush/resources/AssetManager.h"

#include <glm/glm.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/array.hpp>
#include <cereal/types/string.hpp>
#include <entt/core/attribute.h>

struct ENTT_API CameraComponent {
    bool main;
    glm::vec4 clearColor;
    Rush::Shared<Rush::Cubemap> skybox;
    Rush::Camera camera;

    CameraComponent() 
        : main(false), clearColor(0.0f,0.0f,0.0f,1.0f), camera(1024.0f/720.0f,45.0f) { }

    CameraComponent(bool main, Rush::Camera cam) 
        : main(main), clearColor(0.0f,0.0f,0.0f,1.0f), camera(cam) { }

    template<typename Archive>
    void save(Archive &archive) const{
        std::string sbp = skybox == nullptr ? "none" : skybox->GetDebugName();
        archive(
            cereal::make_nvp("main", main),
            cereal::make_nvp("clear", std::array{
                cereal::make_nvp("r",clearColor.r),
                cereal::make_nvp("g",clearColor.g),
                cereal::make_nvp("b",clearColor.b),
                cereal::make_nvp("a",clearColor.a)
            }),
            cereal::make_nvp("type", std::string(camera.IsPerspective() ? "perspective" : "orthogonal") ),
            cereal::make_nvp("frustum", std::array{
                cereal::make_nvp("near", camera.GetNear()),
                cereal::make_nvp("far", camera.GetFar()),
                cereal::make_nvp("fov", camera.GetFOV())
            }),
            cereal::make_nvp("skybox", sbp)
        );
    }

    template<typename Archive>
    void load(Archive &archive){
        std::string skyboxBuf, type;
        float fov, far, near;

        archive(
            cereal::make_nvp("main", main),
            cereal::make_nvp("clear", std::array{
                cereal::make_nvp("r",clearColor.r),
                cereal::make_nvp("g",clearColor.g),
                cereal::make_nvp("b",clearColor.b),
                cereal::make_nvp("a",clearColor.a)
            }),
            cereal::make_nvp("type", type),
            cereal::make_nvp("frustum", std::array{
                cereal::make_nvp("near", near),
                cereal::make_nvp("far", far),
                cereal::make_nvp("fov", fov)
            }),
            cereal::make_nvp("skybox", skyboxBuf)
        );
        float aspect = 1024.0f/720.0f;  // TODO: serialize aspect ratio
        if(type == "perspective")
            camera.SetPerspective(aspect,fov,near,far);
        else
            camera.SetOrthographic(-0.5f*aspect,0.5f*aspect,0.5f*(1.0f/aspect),-0.5f*(1.0f/aspect),near,far);

        if(skyboxBuf != "none")
            skybox = Rush::AssetManager::GetCubemap(skyboxBuf);
    }
};
#endif // __CAMERACOMPONENT_H__
