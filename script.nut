/*
EngineSpawn.Spawn() takes a few parameters:
layer, any integer, higher is on top
camera follow mode, 0 is normal, 1 is only rotation, like a skybox, 2 is inverse look (axis aligned but 3rd person)
path to the obj file
path to the texture
path to the shader

after creating you must use EngineSpawn.Register() to add it to the vertex buffer and render queue

Constants.GetGame() returns the Engine::Game instance, but does not live update because idk squirrel moment
it will quickly fuck up your day if you repeatedly call it



*/



run <- 0;
local lit_shader = "content/shaders/frag_lit.glsl";
local unlit_shader = "content/shaders/frag_unlit.glsl";
spawn <- EngineSpawn();
game <- Constants.GetGame();

//local axis = spawn.Spawn(10,2,"content/models/axis.obj","content/textures/color.dds",unlit_shader);
//spawn.Register(axis);
local skybox = spawn.Spawn(-1,1,"content/models/skybox.obj","content/textures/skybox.dds","content/shaders/frag_skybox.glsl");
spawn.Register(skybox);


local obj = spawn.Spawn(0,0,"content/models/scene.obj","content/textures/uvtemplate.dds",lit_shader);
obj.layer = 0;
obj.transform.Position.y = -0.25;


spawn.Register(obj);
local o1 = spawn.Spawn(0,0,"content/models/cube.obj","",lit_shader);
local o2 = spawn.Spawn(0,0,"content/models/cube.obj","",lit_shader);
local o3 = spawn.Spawn(0,0,"content/models/cube.obj","",lit_shader);
local o4 = spawn.Spawn(0,0,"content/models/cube.obj","",lit_shader);

local fuck = spawn.Spawn(0,0,"content/models/fuck.obj","",unlit_shader);
spawn.Register(fuck);

o1.transform.Position.x = 2;
o2.transform.Position.x = 2;
o3.transform.Position.x = -2;
o4.transform.Position.x = -2;

o1.transform.Position.z = -2;
o2.transform.Position.z = 2;
o3.transform.Position.z = 2;
o4.transform.Position.z = -2;

o1.transform.Position.y = -2;
o2.transform.Position.y = -2;
o3.transform.Position.y = -2;
o4.transform.Position.y = -2;


spawn.Register(o1);
spawn.Register(o2);
spawn.Register(o3);
spawn.Register(o4);

//HUD.CreateText("Testing",0,0,50);
//Utils.Run("ui.nut");
HUD.CreateSprite("content/textures/grunt.dds",0,0,2000,2000);
