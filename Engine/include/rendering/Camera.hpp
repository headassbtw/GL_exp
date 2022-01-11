#pragma once
#include <glm/glm.hpp>
namespace Engine{
    namespace Render{
        struct Camera{
            float FOV;
            float NearClip;
            float FarClip;
            glm::vec3 Position;
            glm::vec3 Rotation;
        };
    }
}