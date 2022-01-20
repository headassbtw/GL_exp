try{
    local gm = Constants.GetGame();
    HUD.Clear();
    HUD.PushText("Alpha Build",1200,50,30);

    local rentime = "Render Thread: ";
    rentime += gm.RenderThreadMs;
    rentime += "ms";
    HUD.PushText(rentime,0,880,20);

    local scrtime = "Scripting Thread: ";
    scrtime += gm.ScriptThreadMs;
    scrtime += "ms";
    HUD.PushText(scrtime,0,860,20);
    
    local inputtime = "Input Thread: ";
    inputtime += gm.InputThreadMs;
    inputtime += "ms";
    HUD.PushText(inputtime,0,840,20);
    

    local tex = "";
    tex += Constants.LoadedTextures();
    tex += " Loaded Textures";
    HUD.PushText(tex,0,800,20);

    local shader = "";
    shader += Constants.LoadedShaders();
    shader += " Loaded Shaders";
    HUD.PushText(shader,0,780,20);

    local nut = "Current update Script: ";
    nut += Constants.UpdateScript();
    HUD.PushText(nut,0,740,20);

    local Pos = "Pos: ";
    Pos += "(";
    Pos += gm.Camera.Position.x;
    Pos += ",";
    Pos += gm.Camera.Position.y;
    Pos += ",";
    Pos += gm.Camera.Position.z;
    Pos += ")";
    HUD.PushText(Pos,0,700,20);

    local Rot = "Rot: ";
    Rot += "(";
    Rot += gm.Camera.Rotation.x;
    Rot += ",";
    Rot += gm.Camera.Rotation.y;
    Rot += ",";
    Rot += gm.Camera.Rotation.z;
    Rot += ")";
    HUD.PushText(Rot,0,680,20);

    HUD.PushSprite("content/textures/uvtemplate.dds",0,0,200,200);
    HUD.PushSprite("content/textures/checker.dds",20,20,300,300);


}
catch(exception){
    error(exception);
}