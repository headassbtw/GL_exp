#include <object/Mesh.hpp>
#include <object/GameObject.hpp>

Engine::GameObject::GameObject(){
    layer = 0;
    transform = Engine::Transform();
}

Engine::GameObject::~GameObject(){
    
}