#include <glm/fwd.hpp>
#include <squirrel.h>
#include <scripting/bindings/vector.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
using namespace Engine;

void vec3(HSQUIRRELVM sq_vm){
    Sqrat::Class<glm::vec3> aClass(sq_vm, "Vector3");
    aClass.Var("x", &glm::vec3::x);
    aClass.Var("y", &glm::vec3::y);
    aClass.Var("z", &glm::vec3::z);
    Sqrat::RootTable(sq_vm).Bind("Vector3", aClass);
}
void vec2(HSQUIRRELVM sq_vm){
    Sqrat::Class<glm::vec2> aClass(sq_vm, "Vector2");
    aClass.Var("x", &glm::vec2::x);
    aClass.Var("y", &glm::vec2::y);
    Sqrat::RootTable(sq_vm).Bind("Vector2", aClass);
}

void Engine::Scripting::Bindings::Vector::Bind(HSQUIRRELVM sq_vm){
    vec3(sq_vm);
    vec2(sq_vm);
}