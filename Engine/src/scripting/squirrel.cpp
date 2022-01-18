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

SQInteger register_global_func(HSQUIRRELVM v,SQFUNCTION f,const char *fname)
 {
        sq_pushroottable(v);
        sq_pushstring(v,fname,-1);
        sq_newclosure(v,f,0);
        sq_createslot(v,-3);
        sq_pop(v,1);
        return 0;
 }

SQInteger move(HSQUIRRELVM vm){
    SQInteger nargs = sq_gettop(vm); //number of arguments
    SQInteger obj;
    SQFloat x;
    SQFloat y;
    SQFloat z;
    sq_getinteger(vm, 2, &obj);
    sq_getfloat(vm, 3, &x);
    sq_getfloat(vm, 4, &y);
    sq_getfloat(vm, 5, &z);


    help->Objects[obj].transform.Position = glm::vec3(x,y,z);
    //help->Objects[obj].mesh.ApplyTransform(help->Objects[obj].transform);
    //help->Objects[obj].mesh.FlagForUpdate();



    sq_pushinteger(vm,nargs);
    return 1;
}

SQInteger rotate(HSQUIRRELVM vm){
    SQInteger nargs = sq_gettop(vm); //number of arguments
    SQInteger obj;
    SQFloat x;
    SQFloat y;
    SQFloat z;
    sq_getinteger(vm, 2, &obj);
    sq_getfloat(vm, 3, &x);
    sq_getfloat(vm, 4, &y);
    sq_getfloat(vm, 5, &z);


    help->Objects[obj].transform.Rotation = glm::vec3(x,y,z);
    //help->Objects[obj].mesh.ApplyTransform(help->Objects[obj].transform);
    //help->Objects[obj].mesh.FlagForUpdate();

    sq_pushinteger(vm,nargs);
    return 1;
}

SQInteger scale(HSQUIRRELVM vm){
    SQInteger nargs = sq_gettop(vm); //number of arguments
    SQInteger obj;
    SQFloat x;
    SQFloat y;
    SQFloat z;
    sq_getinteger(vm, 2, &obj);
    sq_getfloat(vm, 3, &x);
    sq_getfloat(vm, 4, &y);
    sq_getfloat(vm, 5, &z);


    help->Objects[obj].transform.Scale = glm::vec3(x,y,z);
    help->Objects[obj].mesh.ApplyTransform(help->Objects[obj].transform);
    help->Objects[obj].mesh.FlagForUpdate();

    sq_pushinteger(vm,nargs);
    return 1;
}


SQInteger setskybox(HSQUIRRELVM vm){
    SQInteger nargs = sq_gettop(vm); //number of arguments
    SQBool set;
    sq_getbool(vm, 2, &set);
    help->RenderSkybox = set;
    sq_pushinteger(vm,nargs);
    return 1;
}

SQInteger spawn(HSQUIRRELVM vm){
    SQInteger nargs = sq_gettop(vm); //number of arguments
    SQInteger follow;
    SQInteger layer;
    const SQChar* objout;
    const SQChar* texout;
    const SQChar* shdout;
    sq_getinteger(vm, 2, &layer);
    sq_getinteger(vm, 3, &follow);
    sq_getstring(vm, 4, &objout);
    sq_getstring(vm, 5, &texout);
    sq_getstring(vm, 6, &shdout);

    help->Camera.NearClip = 0.5f;

    auto object = Engine::GameObject();
    auto shader = Engine::Render::Shaders::GetShaders( "content/shaders/vert.glsl", shdout );
    object.mesh = Engine::Mesh(objout);
    object.follow = (Engine::Enums::GameObjectFollowTarget)follow;
    object.layer = layer;
    //object.mesh.FlagForUpdate();
    object.mesh.Shader = shader;
    object.mesh.Texture = Engine::Filesystem::Textures::LoadDDS(texout);


    int idx = help->Objects.size();
    help->Objects.push_back(object);
    //help->ProcessMeshes();
    sq_pushinteger(vm,idx);
    return 1;
}


SQInteger func(HSQUIRRELVM vm){
    SQInteger nargs = sq_gettop(vm); //number of arguments
        for(SQInteger n=1;n<=nargs;n++)
        {
            
                printf("arg %d is ",n);
                switch(sq_gettype(vm,n))
                {
                case OT_NULL:
                        printf("null");
                        break;
                case OT_INTEGER:
                        printf("integer");
                        break;
                case OT_FLOAT:
                        printf("float");
                        break;
                case OT_STRING:
                        printf("string");
                        const SQChar* ass;
                        sq_getstring(vm, n, &ass);
                        printf("%s", ass);
                        break;
                case OT_TABLE:
                        printf("table");
                        break;
                case OT_ARRAY:
                        printf("array");
                        break;
                case OT_USERDATA:
                        printf("userdata");
                        break;
                case OT_CLOSURE:
                        printf("closure(function)");
                        break;
                case OT_NATIVECLOSURE:
                        printf("native closure(C function)");
                        break;
                case OT_GENERATOR:
                        printf("generator");
                        break;
                case OT_USERPOINTER:
                        printf("userpointer");
                        break;
                default:
                        return sq_throwerror(vm,"invalid param"); //throws an exception
                }
        printf("\n");
        }


        sq_pushinteger(vm,nargs);
        return 1;
}

 void Engine::Scripting::RunScript(const char* cmd)
 {
    int aaa = sizeof(cmd) / sizeof(char*);
    sq_compilebuffer(sq_vm,cmd,(int)aaa*sizeof(SQChar),"compile",1);
    sq_pushroottable(sq_vm);
    sq_call(sq_vm,1,1,0);
 }

void Engine::Scripting::Run(Engine::Game& game){

    help = &game;
    sq_vm = sq_open(1024);
    
    sq_setprintfunc(sq_vm, printfunc, NULL);
    sq_seterrorhandler(sq_vm);
    


    sq_pushroottable(sq_vm);

    register_global_func(sq_vm, ::func, "test");
    register_global_func(sq_vm, ::spawn, "engine_spawn");
    register_global_func(sq_vm, ::setskybox, "cl_showsky");
    register_global_func(sq_vm, ::move, "engine_move");
    register_global_func(sq_vm, ::rotate, "engine_rotate");
    register_global_func(sq_vm, ::scale, "engine_scale");

    
    sqstd_dofile(sq_vm, "script.nut",true,true); 
    
}

void Engine::Scripting::Terminate(){
    sq_pop(sq_vm,1);
    sq_close(sq_vm);
}