#include <scripting/bindings/vec3.hpp>
#include <scripting/bindings/transform.hpp>
#include <scripting/bindings/rendercamera.hpp>
#include <scripting/bindings/gameobject.hpp>
#include <scripting/bindings/game.hpp>
#include <scripting/bindings/hud.hpp>
#include "sqext.hpp"
#include "sqrat/sqratClass.h"
#include "sqrat/sqratTable.h"
#include "sqrat/sqratUtil.h"
#include <rendering/Shader.hpp>
#include <content/textures.hpp>
#include <Game.hpp>
#include <scripting/squirrel.hpp>
#include <cstdarg>
#include <cstddef>
#include <cstdio>
#include <scripting/squirrel.hpp>
#include <object/GameObject.hpp>
#include <squirrel.h>
#include <sqstdio.h>
#include <sqrat.h>

Engine::Game* help;
HSQUIRRELVM sq_vm;

void printfunc(HSQUIRRELVM v, const SQChar *s,...) 
        {
            va_list listargs;
            va_start(listargs, s);

            const char* ass = va_arg(listargs, const char*);
                printf("%s\n", ass); 
                va_end(listargs);
        } 

void errfunc(HSQUIRRELVM v, const SQChar *s,...) 
        {
            va_list listargs;
            va_start(listargs, s);

            const char* ass = va_arg(listargs, const char*);
                std::string err = std::string("[ERROR] ");
                err += ass;
                printf("%s\n", err.c_str()); 
                help->Errors.push_front(err);
                va_end(listargs);
        } 

SQInteger register_global_func(HSQUIRRELVM v,SQFUNCTION f,const char *fname)
 {
        sq_pushroottable(v);
        sq_pushstring(v,fname,-1);
        sq_newclosure(v,f,0);
        sq_createslot(v,-3);
        sq_pop(v,1);
        return 0;
 }

class EngineSpawn{
        public:
        Engine::GameObject Spawn(int layer, int camfollow, const char* objfile,const char* texfile,const char* shaderfile){
                help->Camera.NearClip = 0.5f;

                auto object = Engine::GameObject();
                auto shader = Engine::Render::Shaders::GetShaders( "content/shaders/vert.glsl", shaderfile );
                object.mesh = Engine::Mesh(objfile);
                object.follow = (Engine::Enums::GameObjectFollowTarget)camfollow;
                object.layer = layer;
                //object.mesh.FlagForUpdate();
                object.mesh.Shader = shader;
                object.mesh.Texture = Engine::Filesystem::Textures::LoadDDS(texfile);

                return object;
        }
        int Register(Engine::GameObject obj){
                int idx = help->Objects.size();
                help->Objects.push_back(obj);
                return idx;
        }
};

void Engine::Scripting::RunScript(const char* cmd)
{
        int aaa = sizeof(cmd) / sizeof(char*);
        sq_compilebuffer(sq_vm,cmd,(int)aaa*sizeof(SQChar),"compile",1);
        sq_pushroottable(sq_vm);
        sq_call(sq_vm,1,1,0);
}

Engine::Game hi(){
        return *help;
}
int texturecount(){
        return Engine::Game::LoadedTextures;
}
int shadercount(){
        return Engine::Game::LoadedShaders;
}
std::string updatescript(){
        return help->UpdateScript;
}
int renderms(){return help->RenderThreadMs;}
int inputms(){return help->InputThreadMs;}
int scriptms(){return help->ScriptThreadMs;}
Engine::Render::Camera cam(){return help->Camera;}

void Engine::Scripting::Run(Engine::Game& game){

    help = &game;
    sq_vm = sq_open(8192);
    sq_setprintfunc(sq_vm, printfunc, errfunc);
    sq_seterrorhandler(sq_vm);
    Sqrat::DefaultVM::Set(sq_vm);
    


        sq_pushroottable(sq_vm);

        Sqrat::Table aTable(sq_vm);
        
        aTable.Func("GetGame", &hi);
        aTable.Func("UpdateScript", &updatescript);
        aTable.Func("LoadedTextures", &texturecount);
        aTable.Func("LoadedShaders", &shadercount);
        aTable.Func("RenderThreadMs", &renderms);
        aTable.Func("InputThreadMs", &inputms);
        aTable.Func("ScriptThreadMs", &scriptms);
        aTable.Func("Camera", &cam);
        
        Sqrat::RootTable(sq_vm).Bind("Constants", aTable);

        Sqrat::Class<EngineSpawn> eClass(sq_vm, "EngineSpawn");

        eClass.Func("Spawn", &EngineSpawn::Spawn);
        eClass.Func("Register", &EngineSpawn::Register);

        Sqrat::RootTable(sq_vm).Bind("EngineSpawn", eClass);

        Engine::Scripting::Bindings::Vector3::Bind(sq_vm);
        Engine::Scripting::Bindings::Transform::Bind(sq_vm);
        Engine::Scripting::Bindings::RenderCamera::Bind(sq_vm);
        Engine::Scripting::Bindings::GameObject::Bind(sq_vm);
        Engine::Scripting::Bindings::Game::Bind(sq_vm);
        Engine::Scripting::Bindings::HUD::Bind(sq_vm);
    

        sqstd_dofile(sq_vm, "script.nut",true,true);
    
}
void Engine::Scripting::Run(const char* script){
        help->UpdateScript = script;
        sqstd_dofile(sq_vm, script, true, true);
        sq_pop(sq_vm, 1);
}

void Engine::Scripting::Terminate(){
    sq_pop(sq_vm,1);
    sq_close(sq_vm);
}