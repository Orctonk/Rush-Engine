#ifndef __TAGCOMPONENT_H__
#define __TAGCOMPONENT_H__

#include <string>

#include <cereal/archives/json.hpp>
#include <cereal/types/string.hpp>
#include <entt/core/attribute.h>

struct ENTT_API TagComponent {
    std::string tag;

    template <typename Archive>
    void serialize(Archive &archive) {
        archive(
            cereal::make_nvp("name", tag));
    }
};

#endif // __TAGCOMPONENT_H__
