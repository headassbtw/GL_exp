#pragma once
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
namespace Engine{
    struct Transform{
        glm::vec3 Position;
        glm::vec3 Rotation;
        glm::vec3 Scale;
        Transform();
        ~Transform();
    };
}