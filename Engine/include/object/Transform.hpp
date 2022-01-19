#pragma once
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
namespace Engine{
    class Transform{
        public:
        glm::vec3 Position;
        glm::vec3 Rotation;
        glm::vec3 Scale;
        Transform();
        ~Transform();
    };
}