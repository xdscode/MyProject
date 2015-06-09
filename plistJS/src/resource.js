var res = {
    HelloWorld_png : "res/HelloWorld.png",
    CloseNormal_png : "res/CloseNormal.png",
    CloseSelected_png : "res/CloseSelected.png",

    MainBG:"res/map.jpg",
    YaoGanBG:"res/yaoganbd.png",
    YaoGan_Dian:"res/yaogan_dian.png",

    UILOADINGBAR_png : "res/loadingbar.png",
    UILOADINGBD_png : "res/loadingbarbd.png"
};

var resPlist ={
    BUFFSKILL:"res/Buffskill.plist"
};

var resJSON ={
    Main_JSON:"res/MainScene.json",
};

var g_resources = [];
for (var i in res) {
    g_resources.push(res[i]);
}

jsb.fileUtils.addSearchPath("res/ui/");
jsb.fileUtils.addSearchPath("res/eff/");