/**
 * Created by yangshengjiepro on 15/5/11.
 */

var PFuns = function () {
    this.flag = 0;
}

//执行某个特效
PFuns.runEFFAttack_SP = function (sp , Url , name , num, speed , loop , delay , zorder , scale , tag , cp) {
    var sprite = sp;
    var sp_eff = new cc.Sprite();

    var animation = new cc.Animation();
    for (var i = 1; i <= num; i++) {
        var toi="";
        if(i<10)
        {
            toi = "0"+i;
        }
        else
        {
            toi = i;
        }
        var frameName = Url  + "/" + name  + toi + ".png";
        Mlog.c("frameName>>"+frameName);
        var frame =GameFrameCache.getCache(frameName);
        if(frame!=null) {
            Mlog.c("frameName>ok>>"+frameName);
            animation.addSpriteFrame(frame);
        }
    }
    var usetime =1 / parseInt(speed);
    animation.setDelayPerUnit(usetime);
    animation.setRestoreOriginalFrame(true);
    animation.setLoops(loop);
    var action = cc.animate(animation);
    var actdelay2 = cc.DelayTime.create(delay);

    if(delay>0)
    {
        sp_eff.runAction(cc.Sequence(actdelay2,action,cc.callFunc(PFuns.effremove, sp_eff)));
    }
    else
    {
        sp_eff.runAction(cc.Sequence(action,cc.callFunc(PFuns.effremove, sp_eff)));
    }


    if(cp!=null)
    {
        sp_eff.setPosition(cc.p(cp.x,cp.y));
    }
    else
    {
        sp_eff.setPosition(cc.p(sprite.getContentSize().width/2,sprite.getContentSize().height/2));
    }

    sp_eff.setScale(scale);
    sprite.addChild(sp_eff,zorder,tag);

};

//移除
PFuns.effremove = function () {
    this.removeFromParent();
};