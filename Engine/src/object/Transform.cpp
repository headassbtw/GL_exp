#include "object/GameObject.hpp"
#include <glm/fwd.hpp>
#include <object/Transform.hpp>

Engine::Transform::Transform(){
    Position = glm::vec3(0);
    Rotation = glm::vec3(0);
    Scale = glm::vec3(1);
}
Engine::Transform::~Transform(){
    
}