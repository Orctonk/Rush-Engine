#ifndef __SCRIPTCOMPONENT_H__
#define __SCRIPTCOMPONENT_H__

#include <string>

#include <cereal/archives/json.hpp>
#include <cereal/types/string.hpp>
#include <entt/core/attribute.h>

struct ENTT_API ScriptComponent {
    std::string scriptFile;

    template<typename Archive>
    void serialize(Archive &archive) {
        archive(
            cereal::make_nvp("script", scriptFile)
        );
    }
};

#endif // __SCRIPTCOMPONENT_H__
