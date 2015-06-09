///**
// * Created by yangshengjiepro on 15/5/3.
// */
//
//var baselayer = cc.Layer.extend({
//    ctor:function(){
//        this._super();
//        cc.log("baselayer ctor read");
//    },
//
//    init:function(){
//        this._super();
//        cc.log("baselayer init read");
//    }
//});
//
//var myLayer = baselayer.extend({
//    ctor:function(){
//        this._super();
//        cc.log("myLayer ctor read");
//    },
//
//    init:function(){
//        cc.log("myLayer init read");
//    }
//});
//
//
//var MainScene = cc.Scene.extend({
//    onEnter:function(){
//        this._super();
//        var layer = new myLayer();
//        layer.init();
//        this.addChild(layer);
//    }
//});