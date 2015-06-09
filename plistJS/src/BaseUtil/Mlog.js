/**
 * Created by yangshengjiepro on 15/5/5.
 */
/**
 * 自定义输出日志
 */
var OPENLOGFLAG = true;

var Mlog = function () {
    this.flag = 0;
}

//正常输出
Mlog.c = function(){
    var bakLog = cc._cocosplayerLog || cc.log || log;
    if(OPENLOGFLAG==true)
    {
        bakLog.call(this,"Mlog:" + cc.formatStr.apply(cc, arguments));
    }
};
//错误输出
Mlog.e = function(){
    var bakLog = cc._cocosplayerLog || cc.log || log;
    if(OPENLOGFLAG==true)
    {
        bakLog.call(this, "Mlog_ERROR:" + cc.formatStr.apply(cc, arguments));
    }
};