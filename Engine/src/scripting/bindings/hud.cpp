#include "sqrat/sqratTable.h"
#include <squirrel.h>
#include <scripting/bindings/hud.hpp>
#include <ui/hud.hpp>
using namespace Engine;

void bound(){
    printf("yep bound\n");
}

void Engine::Scripting::Bindings::HUD::Bind(HSQUIRRELVM sq_vm){
    Sqrat::Table aTable(sq_vm);
    aTable.Func("Clear", &Engine::UI::HUD::Clear);
    aTable.Func("Render", &UI::HUD::Render);
    aTable.Func("PushSprite", &UI::HUD::PushSprite);
    aTable.Func("PushText", &UI::HUD::PushText);
    aTable.Func("Count", &UI::HUD::CountElements);
    aTable.Func("bound", &bound);
    Sqrat::RootTable(sq_vm).Bind("HUD", aTable);
}