print("Hello from squirrel!"); 
local lit_shader = "content/shaders/frag_lit.glsl";
local unlit_shader = "content/shaders/frag_unlit.glsl";

local skybox = engine_spawn(1,"content/models/skybox.obj","content/textures/skybox.dds",unlit_shader);
local axis = engine_spawn(1,"content/models/axis.obj","content/textures/color.dds",unlit_shader);

local j = engine_spawn(0,"content/models/scene.obj","content/textures/uvtemplate.dds",lit_shader);
local o1 = engine_spawn(0,"content/models/cube.obj","",lit_shader);
local o2 = engine_spawn(0,"content/models/cube.obj","",lit_shader);
local o3 = engine_spawn(0,"content/models/cube.obj","",lit_shader);
local o4 = engine_spawn(0,"content/models/cube.obj","",lit_shader);
engine_move(j,0,0,0);
engine_rotate(j,0,0,0);

engine_move(o1,2,0,2);
engine_rotate(o1,0,180,0);

engine_move(o2,2,0,-2);
engine_rotate(o2,0,90,0);

engine_move(o3,-2,0,2);
engine_rotate(o3,0,-90,0);

engine_move(o4,-2,0,-2);
engine_rotate(o4,0,0,0);

