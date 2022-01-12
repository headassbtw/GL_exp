#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>
namespace Engine{
    namespace Filesystem{
        bool OBJ(const char* path, std::vector < glm::vec3 > & out_vertices,
        std::vector < glm::vec2 > & out_uvs,
        std::vector < glm::vec3 > & out_normals);
        std::string Text(const char* path);

    }
}