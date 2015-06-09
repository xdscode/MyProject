/**
 * Created by yangshengjiepro on 15/4/20.
 */

var LOADINGBARPRONUM = 1;

var LOADINGBAR_TAG = 99912;

var MainLayer = BaseLayer.extend({
    ctor:function(){
        this._oktouch=true;//开启可透点击
        this._showbg=true;//开启背景
        this._showbgAcion=false;//主界面不需要弹出效果
        this._super();
        //var mainbg =cc.Sprite(res.MainBG);
        //mainbg.attr({
        //    x:this.getContentSize().width/2,
        //    y:this.getContentSize().height/2,
        //    scale:1,
        //    ratation:0
        //});
        //mainbg.setAnchorPoint(cc.p(0.5,0.5));
        //this.addChild(mainbg);

        this.setGameFrameCache(resPlist.BUFFSKILL);

        var UIroot = this.setUIFile_File(resJSON.Main_JSON);
        UIroot.setAnchorPoint(cc.p(0.5,0.5));
        UIroot.setPosition(cc.p(this.getContentSize().width/2,this.getContentSize().height/2));
        this.addChild(UIroot,1);

        var lv_100 = ccui.helper.seekWidgetByName(UIroot,"lv_100");
        lv_100.setString("等级");

        var lv_200 = ccui.helper.seekWidgetByName(UIroot,"lv_200");
        lv_200.setString("lv:300");

        var bt_muisc = ccui.helper.seekWidgetByName(UIroot,"bt_music");
        bt_muisc.setTag(1);
        bt_muisc.addTouchEventListener(this.touchEvent_MainButton,bt_muisc);

        //PFuns.runEFFAttack_SP(this,"skill_dianjin","000",13,6,3,1,1000,1,100,null);
        //初始化进度条
        this.initloadingbar(this);
        //进行异步加载，绑定更新进度条的方法setloadingbar
        GameFrameCache.setAllCache(this,this.setloadingbar);

    },


    //进度条的加载
    initloadingbar:function(sp_loading){
        //加载Loading条
        var sp_loadingtiao = new cc.Sprite(res.UILOADINGBD_png);
        sp_loadingtiao.attr({
            x: sp_loading.getContentSize().width/2,
            y: sp_loading.getContentSize().height/2-180,
            scale: 1,
            rotation: 0
        });
        sp_loading.addChild(sp_loadingtiao,1);
        var loadingBar = new ccui.LoadingBar(res.UILOADINGBAR_png);
        loadingBar.x = sp_loading.getContentSize().width/2;
        loadingBar.y = sp_loading.getContentSize().height/2-180;
        sp_loading.addChild(loadingBar,2,LOADINGBAR_TAG);
        loadingBar.setPercent(0);
    },

    //资源loadding buffer进度回调
    setloadingbar:function(){

        var pernum = parseInt(LOADINGBARPRONUM/LOADINGBARPROALLNUM *100);
        Mlog.c("pernum >>" +pernum);
        LOADINGBARPRONUM++;
        var loadingBar = this.getChildByTag(LOADINGBAR_TAG);
        if(loadingBar!=null)
        {
            loadingBar.setPercent(pernum);
        }
        //进度条加载完毕进行跳转
        if(pernum==100)
        {
            //加载完毕
            Mlog.c("异步加载资源完毕");
            PFuns.runEFFAttack_SP(this , "skill_dianjin" , "000" , 13, 6 , 3 ,1 , 11111 , 1 ,1000 , null);
        }
    },

    touchEvent_MainButton:function(sender,type){
        switch (type){

            case ccui.Widget.TOUCH_ENDED:
                var tagnum = sender.getTag();
                Mlog.c("touchEvent_MainButton tagnum >"+tagnum);

                break;
            default :
                break;
        }
    }


});


var MainScene = cc.Scene.extend({
    onEnter:function(){
        this._super();
        var layer = new MainLayer();
        this.addChild(layer);
    }
});