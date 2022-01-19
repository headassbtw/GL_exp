#include <squirrel.h>
#include <scripting/bindings/transform.hpp>
#include <object/Transform.hpp>
using namespace Engine;

void Engine::Scripting::Bindings::Transform::Bind(HSQUIRRELVM sq_vm){
    Sqrat::Class<Engine::Transform> aClass(sq_vm, "Transform");
        aClass.Var("Position", &Engine::Transform::Position);
        aClass.Var("Rotation", &Engine::Transform::Rotation);
        aClass.Var("Scale", &Engine::Transform::Scale);
        Sqrat::RootTable(sq_vm).Bind("Transform", aClass);
}