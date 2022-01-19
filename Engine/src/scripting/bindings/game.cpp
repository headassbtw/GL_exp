#include <squirrel.h>
#include <scripting/bindings/game.hpp>
#include <Game.hpp>
using namespace Engine;

void Engine::Scripting::Bindings::Game::Bind(HSQUIRRELVM sq_vm){
    Sqrat::Class<Engine::Game> aClass(sq_vm, "Game");
    aClass.Var("Width",&Engine::Game::Width);
    aClass.Var("Height",&Engine::Game::Height);
    aClass.Var("InputThreadMs",&Engine::Game::InputThreadMs);
    aClass.Var("Errors",&Engine::Game::Errors);
    aClass.Var("RenderThreadMs",&Engine::Game::RenderThreadMs);
    aClass.Var("ScriptThreadMs",&Engine::Game::ScriptThreadMs);
    aClass.Var("RenderSkybox",&Engine::Game::RenderSkybox);
    aClass.Var("RenderAxisHelper",&Engine::Game::RenderAxisHelper);
    aClass.Var("Camera",&Engine::Game::Camera);
        Sqrat::RootTable(sq_vm).Bind("Game", aClass);
}