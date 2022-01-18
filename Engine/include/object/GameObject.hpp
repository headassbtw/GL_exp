#pragma once
#include <string>
#include <vector>
#include "Transform.hpp"
#include "object/Mesh.hpp"
namespace Engine{
    namespace Enums{
        enum GameObjectFollowTarget{
            None,
            Camera,
            InverseCamera
        };
    }
    
    struct GameObject{
        std::string name;
        Transform transform;
        Mesh mesh;
        std::vector<GameObject> children;
        Enums::GameObjectFollowTarget follow;
        GameObject();
        ~GameObject();
    };
}