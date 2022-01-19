#include <glm/fwd.hpp>
#include <squirrel.h>
#include <scripting/bindings/vec3.hpp>
#include <glm/vec3.hpp>
using namespace Engine;

void Engine::Scripting::Bindings::Vector3::Bind(HSQUIRRELVM sq_vm){
    Sqrat::Class<glm::vec3> aClass(sq_vm, "glm::vec3");
        aClass.Var("x", &glm::vec3::x);
        aClass.Var("y", &glm::vec3::y);
        aClass.Var("z", &glm::vec3::z);
        Sqrat::RootTable(sq_vm).Bind("glm::vec3", aClass);
}