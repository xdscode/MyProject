/**
 * Created by yangshengjiepro on 15/5/11.
 */

var GameFrameCache = function(){
    this.flag = 0;
}

var LOADINGBARPROALLNUM = 0;

//异步加载
GameFrameCache.setAllCache = function (obj,objcallback) {

    //异步加载所有游戏资源
    var texCache = cc.textureCache;
    //遍历所有的资源
    var reslist = res;
    var allnum = 0;
    for (var key = 0 in reslist) {
        Mlog.c("reslist key"+key+"value:"+reslist[key]);
        allnum++;
    }

    LOADINGBARPROALLNUM = allnum;
    Mlog.c("LOADINGBARPROALLNUM>>",LOADINGBARPROALLNUM);

    var readnum = 0;
    for (var key = 0 in reslist) {
        //开始装载
        texCache.addImageAsync(reslist[key], objcallback, obj);
    }
};


//资源加载
GameFrameCache.setCache = function(plist){
    if(jsb.fileUtils.isFileExist(plist)==true)
    {
        cc.SpriteFrameCache.getInstance().addSpriteFrames(plist);
    }
    else
    {
        Mlog.c("No add File >>",plist);
    }
};

//获取Frame
GameFrameCache.getCache = function (name) {
    var frame;
    frame = cc.SpriteFrameCache.getInstance().getSpriteFrame(name);
    return frame;
};

//移除Plist
GameFrameCache.removeCache = function(plist){
    if (jsb.fileUtils.isFileExist(plist) == true) {
        cc.SpriteFrameCache.getInstance().removeSpriteFramesFromFile(plist);
    }
};