#include <squirrel.h>
#include <scripting/bindings/gameobject.hpp>
#include <object/GameObject.hpp>
using namespace Engine;

void Engine::Scripting::Bindings::GameObject::Bind(HSQUIRRELVM sq_vm){
    Sqrat::Class<Engine::GameObject> aClass(sq_vm, "GameObject");
        aClass.Var("name", &Engine::GameObject::name);
        aClass.Var("transform", &Engine::GameObject::transform);
        aClass.Var("mesh", &Engine::GameObject::mesh);
        aClass.Var("children", &Engine::GameObject::children);
        aClass.Var("layer", &Engine::GameObject::layer);
        aClass.Var("follow", &Engine::GameObject::follow);
        Sqrat::RootTable(sq_vm).Bind("GameObject", aClass);
}