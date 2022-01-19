/*
EngineSpawn.Spawn() takes a few parameters:
layer, any integer, higher is on top
camera follow mode, 0 is normal, 1 is only rotation, like a skybox, 2 is inverse look (axis aligned but 3rd person)
path to the obj file
path to the texture
path to the shader

after creating you must use EngineSpawn.Register() to add it to the vertex buffer and render queue
*/
run <- 0;
local lit_shader = "content/shaders/frag_lit.glsl";
local unlit_shader = "content/shaders/frag_unlit.glsl";
spawn <- EngineSpawn();
game <- Constants.GetGame();

local axis = spawn.Spawn(10,2,"content/models/axis.obj","content/textures/color.dds",unlit_shader);
local skybox = spawn.Spawn(-1,1,"content/models/skybox.obj","content/textures/skybox.dds","content/shaders/frag_skybox.glsl");
spawn.Register(axis);
spawn.Register(skybox);


local obj = spawn.Spawn(0,0,"content/models/scene.obj","content/textures/uvtemplate.dds",lit_shader);
obj.layer = 3;
obj.transform.Position.y = -0.25;


spawn.Register(obj);
local o1 = spawn.Spawn(0,0,"content/models/cube.obj","",lit_shader);
local o2 = spawn.Spawn(0,0,"content/models/cube.obj","",lit_shader);
local o3 = spawn.Spawn(0,0,"content/models/cube.obj","",lit_shader);
local o4 = spawn.Spawn(0,0,"content/models/cube.obj","",lit_shader);

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
