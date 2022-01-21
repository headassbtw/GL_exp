#include "sqrat/sqratTable.h"
#include "ui/elements.hpp"
#include <squirrel.h>
#include <scripting/bindings/hud.hpp>
#include <ui/hud.hpp>
using namespace Engine;

void bound(){
    printf("yep bound\n");
}


void bindsprite(HSQUIRRELVM sq_vm){
    Sqrat::Class<Sprite> spriteClass(sq_vm,"Sprite");
    spriteClass.Var("vert_idx", &Sprite::vert_idx);
    spriteClass.Var("vert_count", &Sprite::vert_count);
    spriteClass.Var("TextureID", &Sprite::TextureID);
    spriteClass.Var("Position", &Sprite::Position);
    spriteClass.Var("Size", &Sprite::Size);
    Sqrat::RootTable(sq_vm).Bind("Sprite", spriteClass);
}

void bindtext(HSQUIRRELVM sq_vm){
    Sqrat::Class<Text> textClass(sq_vm,"Text");
    textClass.Var("vert_idx", &Text::vert_idx);
    textClass.Var("vert_count", &Text::vert_count);
    textClass.Var("TextureID", &Text::TextureID);
    textClass.Var("text", &Text::text);
    textClass.Var("Position", &Text::Position);
    textClass.Var("Size", &Text::Size);
    Sqrat::RootTable(sq_vm).Bind("Text", textClass);
}

void Engine::Scripting::Bindings::HUD::Bind(HSQUIRRELVM sq_vm){
    bindsprite(sq_vm);
    bindtext(sq_vm);
    Sqrat::Table aTable(sq_vm);
    aTable.Func("Clear", &Engine::UI::HUD::Clear);
    aTable.Func("Render", &UI::HUD::Render);
    aTable.Func("CreateSprite", &UI::HUD::CreateSprite);
    aTable.Func("CreateText", &UI::HUD::CreateText);
    //aTable.Func("DestroySprite", &UI::HUD::DestroySprite);
    //aTable.Func("DestroyText", &UI::HUD::DestroyText);
    aTable.Func("Count", &UI::HUD::CountElements);
    aTable.Func("bound", &bound);
    Sqrat::RootTable(sq_vm).Bind("HUD", aTable);
}