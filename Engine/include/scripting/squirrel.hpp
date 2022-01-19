#pragma once
#include <Game.hpp>
namespace Engine{
    namespace Scripting{
        void RunScript(const char* cmd);
        void Run(Engine::Game& game);
        void GiveGame(Engine::Game gam);
        void Run(const char* script);
        void Terminate();
    }
}