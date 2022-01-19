#include <squirrel.h>
#include <scripting/bindings/rendercamera.hpp>
#include <rendering/Camera.hpp>
using namespace Engine;

void Engine::Scripting::Bindings::RenderCamera::Bind(HSQUIRRELVM sq_vm){
    Sqrat::Class<Engine::Render::Camera> aClass(sq_vm, "RenderCamera");
    aClass.Var("FOV",&Engine::Render::Camera::FOV);
    aClass.Var("NearClip",&Engine::Render::Camera::NearClip);
    aClass.Var("FarClip",&Engine::Render::Camera::FarClip);
    aClass.Var("Position",&Engine::Render::Camera::Position);
    aClass.Var("Rotation",&Engine::Render::Camera::Rotation);
    Sqrat::RootTable(sq_vm).Bind("RenderCamera", aClass);
}