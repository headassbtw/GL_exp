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
    
    class GameObject{
        public:
        std::string name;
        Transform transform;
        Mesh mesh;
        std::vector<GameObject> children;
        int layer;
        Enums::GameObjectFollowTarget follow;
        GameObject();
        ~GameObject();
    };
}