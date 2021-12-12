#ifndef __SCRIPTCOMPONENT_H__
#define __SCRIPTCOMPONENT_H__

#include <string>

#include <cereal/archives/json.hpp>
#include <cereal/types/string.hpp>
#include <entt/core/attribute.h>

struct ENTT_API ScriptComponent {
    std::string scriptFile;
    std::string className;

    template <typename Archive>
    void serialize(Archive &archive) {
        archive(
            cereal::make_nvp("script", scriptFile),
            cereal::make_nvp("class", className));
    }
};

#endif // __SCRIPTCOMPONENT_H__
