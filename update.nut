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

}
catch(exception){
    error(exception);
}