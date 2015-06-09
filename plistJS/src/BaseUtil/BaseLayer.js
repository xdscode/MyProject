/**
 * Created by yangshengjiepro on 15/5/5.
 */
var BaseLayer=cc.Layer.extend({
    _bgFrame:null,
    _oktouch:false,
    _showbg:false,
    _showbgAcion:false,
    ctor:function(){
        this._super();
        //渲染一个背景层，默认为黑色的半透明的
        if(this._showbg==true)
        {
            //背景
            var bgFrame = cc.LayerColor(cc.color(0,0,0,200));
            this.addChild(bgFrame,0);
            this._bgFrame=bgFrame;
            this.setAnchorPoint(cc.p(0.5,0.5));
            //设置当前层里面所有节点的描点也和该层相同
            this.ignoreAnchorPointForPosition(false);
            this.setContentSize(winSize);
            this.setPosition(cc.p(winSize.width/2,winSize.height/2));
        }
        //开启底层不可点击触摸（层以下的UI都不可被点击）
        if(this._oktouch==true)
        {
            //点击时间
            cc.eventManager.addListener({
                event: cc.EventListener.TOUCH_ONE_BY_ONE,
                swallowTouches: true,
                onTouchBegan: function(){

                    return true;
                }
            }, this);
        }
        //开启打开窗体是带的特效
        if(this._showbgAcion==true)
        {
            var obj=this;
            obj.setScale(0.8);
            if(obj!=null){
                var sl=cc.EaseIn.create(cc.ScaleTo.create(0.15,1.1),2);
                var sl2=cc.ScaleTo.create(0.15,1);
                var seq=cc.Sequence(sl,sl2);
                obj.runAction(seq);
            }
        }
    },

    setUIFile_File:function(file){
        var json = ccs.load(file);
        return json.node;
    },
    setUIFile_JSON:function(file){
        return ccs.uiReader.widgetFromJsonFile(file);
    },

    setBgColor:function(color){
        this._bgFrame.setColor(color);
    },

    setGameFrameCache:function(plistfile){
        GameFrameCache.setCache(plistfile);
    },

    onEnter:function(){
        this._super();
    },
    onExit:function(){
        this._super();

    }
});