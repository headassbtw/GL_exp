#pragma once
#include <string>
#include <vector>
#include "Transform.hpp"
#include "object/Mesh.hpp"
namespace Engine{
    struct GameObject{
        std::string name;
        Transform transform;
        Mesh mesh;
        std::vector<GameObject> children;
        GameObject();
        ~GameObject();
    };
}