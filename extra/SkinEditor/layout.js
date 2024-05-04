
//rssi
var RssiWidgets={};
const default_Rssi_prop={
    ble:false,
    color:"white",
    shadow:"#333333"
};

function Rssibar(ele,prop){
    this.ele=ele;
    this.prop=Object.assign({}, default_Rssi_prop, prop);
}
Rssibar.prototype.updateProp=function(prop){
    this.prop=Object.assign({}, this.prop, prop);
//    this.setRssi(this.rssi);
};
Rssibar.prototype.rmProp=function(prop){
    this.prop[prop]=default_Rssi_prop[prop];
//    this.setRssi(this.rssi);
};

Rssibar.prototype.setHidden=function(hidden){
    if(hidden)$(this.ele).hide();
    else $(this.ele).show();
};

Rssibar.prototype.setRssi=function(x){
    var t=this;
    t.rssi=x;
    // calculate number of "bar"
    var strength =t.ble? [-1000, -90, -80, -70, -67]:[-1000,-100,-80,-55];
    var bar = 4;
    for (; bar >= 0; bar--) {
        if (strength[bar] < x) break;
    }
    // update display
    var wu=$(t.ele).width()/7;
    var hu=$(t.ele).height()/4;
    var x =0;
    var y=$(t.ele).height() - hu;
    var height= hu;

    $(t.ele).find(".rssi-bar").each(function(i,div){
        $(div).css({"background-color":(i < bar) ? t.prop.color :t.prop.shadow,
        "left": x,
        "top": y,
        "height": height,
        "width": wu
        });
        x += 2 * wu;
        y -= hu;
        height +=hu;
    });
    $(t.ele).title = (x > 0) ? "?" : ""+x;
};

function getStringWidth(text, font) {
    // Create a temporary canvas element
    var canvas = document.createElement('canvas');
    var context = canvas.getContext('2d');

    // Set the font for the context
    context.font = font;

    // Measure the width of the text
    var me=context.measureText(text);
    var width = me.width;
//    var height = me.actualBoundingBoxAscent;
    // Clean up and return the width
//    document.body.removeChild(canvas);
    return width;
}
function getFontOfElement(element) {
    // Get the computed style of the element
    var computedStyle = $(element).css(['font-family', 'font-size', 'font-weight', 'font-style']);
    var font =  computedStyle['font-style'] + ' ' + computedStyle['font-weight'] + ' ' + computedStyle['font-size'] + ' ' + computedStyle['font-family'];

    return font;
}
function dimNumber(obj,val,total,type,calval){
    var ret;
    
    if(val == "" || val == "c"){
        obj.css(type,"");
        ret= calval;
    }else if ((typeof val === 'string') && val.charAt(val.length -1) == "%"){
        // percentage
        obj.css(type,val);
        ret=parseInt(total * parseFloat(val) / 100);
    }else {
        ret=parseInt(val);
        obj.css(type,ret + "px");
    }
    return ret;
}

function getDimNumber(oval){
    var val=oval.trim();
    if (val.charAt(val.length -1) == "%") return val;
    var ival = parseInt(val);
    if(isNaN(ival)) return "";
    return ival;
}

function position(obj,align,x,y,wv,hv){
    x=isNaN(x)? 0:x;
    y=isNaN(y)? 0:y;

    var pW =$("#lcd").width();
    var pH=$("#lcd").height();
    var w = dimNumber(obj,wv,pW,"width",getStringWidth(obj.text(),getFontOfElement(obj)));
    var h = dimNumber(obj,hv,pH,"height",$(obj).height());
    if(align == "tl"){
        // top left
        obj.css("top",y).css("left",x);
    }else if(align == "c"){
        // center
        obj.css("top",(pH - h)/2 + y).css("left",(pW - w)/2 + x);
    }else if(align == "bl"){
        // bottom left
        obj.css("top",pH - h + y).css("left", x);
    }else if(align == "lm"){
        // left middle
        obj.css("top",(pH - h)/2 + y).css("left",x);
    }else if(align == "tr"){
        // top right
        obj.css("top",y).css("left",pW - w + x);
    }else if(align == "br"){
        // bottom right
        obj.css("top",pH - h + y).css("left",pW - w + x);
    }else if(align == "rm"){
        //  right middle
        obj.css("top",(pH - h)/2 + y).css("left",pW - w + x);
    }
}

function calTextPosition(item){
    var tr = $('#input-table tr[target="' + item +'"]');
    var align = tr.find('select[name="align"]').val();
    var x = parseInt(tr.find('input[target="posx"]').val());
    var y = parseInt(tr.find('input[target="posy"]').val());
    var w= tr.find('input[target="width"]').val().trim();
    var h= tr.find('input[target="height"]').val().trim();
    position($("#" + item),align,x,y,w,h);
}

function changeFont(target,font){
    $(target).removeClass("font-s").removeClass("font-m").removeClass("font-l").removeClass("font-x");
    $(target).addClass(font);
}

function newStaticText(){
    var newtr=stxt_tr.clone(true);
    var newid ='statictext-' +(1+$("#input-table tr.static-text").length);
    $(newtr).attr('target',newid);

    $("#lcd").append($("<span id=\""+ newid +"\" class=\"lcd-text font-s static-text\"></span>"));
    $("#brewpi-start").before(newtr);
    moveable($("#"+newid));
    return newid;
}

function resetInput(){
    // clear input
    $('#lcd span.bpl').hide().each(function(i,ele){
        changeFont(this,"font-s");
    });
    $('#input-table input[target="show"]').prop('checked',false);
    $('#input-table select[target="align"]').val("tl");
    $('#input-table input[target="posx"]').val('');
    $('#input-table input[target="posy"]').val('');
    $('#input-table input[target="width"]').val('');
    $('#input-table input[target="height"]').val('');
    $('#input-table input[target="color"]').val('');
    $('#input-table select[target="font"]').val("default");
    // clear static texts
    var stn=$("#input-table tr.static-text").length;
    $("#input-table tr.static-text:gt(1)").remove();
    for(var i=2;i<=stn;i++){
        $("#statictext-" + i).remove();
    }

    // clear rectangles
    var rtn=$("#input-table tr.rectangles").length;
    $("#input-table tr.rectangles:gt(0)").remove();
    for(var i=2;i<=rtn;i++){
        $("#rectangle-" + i).remove();
    }
    $("#rectangle-1").width(0).height(0);
    $("#input-table tr.static-text").find('input[target="value"]').val('');
    $("#statictext-1").text('');
}
var satticTextStyles={};

function staticTextStyleInput(input){
    const map={decoration:"td",font:"tf",color:"tc"};
    if(typeof map[$(input).attr("target")] == "undefined") alert("invalid field");
    satticTextStyles[map[$(input).attr("target")]]=$(input).val();
    applyStaticTextStyle();
}

function dejsonStaticTextStyle(json){
    satticTextStyles=json;
    var tr=$('#input-table tr.ststyles');
    if(typeof json["td"] == "undefined") tr.find('select[target="decoration"]').val("n");
    else tr.find('select[target="decoration"]').val(json.td);

    if(typeof json["tc"] != "undefined") tr.find('input[target="color"]').val(json.tc);

    if(typeof json["tf"] != "undefined") tr.find('select[target="font"]').val(json.tf);
    else tr.find('select[target="font"]').val('default');
}

function applyStaticTextStyle(){
    $("#lcd span.static-text").each(function(i,ele){
        var tid = $(ele).attr("id");
        var tr = $('#input-table tr[target="'+ tid +'"]');
        // color
        if(typeof satticTextStyles["tc"] !="undefined" &&
            $(tr).find('input[target="color"]').val().trim()==""){
            // apply color
            $(ele).css("color","#"+satticTextStyles["tc"]);
        }
        // font
        if(typeof satticTextStyles["tf"] !="undefined" &&
            $(tr).find('select[target="font"]').val()=="default"){
            changeFont(ele,satticTextStyles["tf"]);
        }
        // decoration
        if(typeof satticTextStyles["td"] !="undefined"){
            $(ele).removeClass("decoration_u").removeClass("decoration_s");
            if (satticTextStyles["td"] != "n"){
                $(ele).addClass("decoration_" +satticTextStyles["td"]);
            }
        }
        calTextPosition(tid);
    });
}

function djsonTextStyle(tr,json){
    if(typeof json["a"] !="undefined") $(tr).find('select[target="align"]').val(json.a);

    if(typeof json["x"] !="undefined") $(tr).find('input[target="posx"]').val(json.x);    
    if(typeof json["y"] !="undefined") $(tr).find('input[target="posy"]').val(json.y);

    if(typeof json["w"] !="undefined" && json.w != "c") $(tr).find('input[target="width"]').val(json.w);
    if(typeof json["h"] !="undefined" && json.h != "c") $(tr).find('input[target="height"]').val(json.h);
    
    if(typeof json["tf"] !="undefined") $(tr).find('select[target="font"]').val(json.tf);
    
    if(typeof json["tc"] !="undefined") $(tr).find('input[target="color"]').val(json.tc);
    var tamap={c:"center",l:"left",r:"right"};
    if(typeof json["ta"] !="undefined") $(tr).find('select[target="text-align"]').val(tamap[json.ta]);

    $(tr).find('input[target="show"]').prop('checked',true);
}

function dejsonStaticTexts(json){
    // first upack all static texts
    $.each(json,function(i,text){
        var tid=(i==0)? "statictext-1":newStaticText();
        if(typeof text["tx"]=="undefined") alert("wrong Static Text Definition");
        $("#" + tid).text(text.tx);
        var tr=$('#input-table tr[target="'+ tid +'"]');
        tr.find('input[target="value"]').val(text.tx);
        djsonTextStyle(tr,text);
        i++;
    });    
}

function applyBplStyle(item,json){
    // color,  font, text alignment, then calPostion will handle others.
    if(typeof json['tc'] != "undefined") $("#" +item).css('color','#' + json.tc);
    var tamap={l:"left",r:"right",c:"center"};
    if(typeof json['ta'] !="undefined") $("#" +item).css('text-align',tamap[json.ta]);
    if(typeof json['tf'] !="undefined")  changeFont($("#" +item),"font-" + json.tf);
    $('#'+item).show();
    calTextPosition(item);
}

function calRectangle(item){
    var tr = $('#input-table tr[target="' + item +'"]');
    var align = tr.find('select[name="align"]').val();
    var x = parseInt(tr.find('input[target="posx"]').val());
    var y = parseInt(tr.find('input[target="posy"]').val());
    var wv= getDimNumber(tr.find('input[target="width"]').val());
    var hv= getDimNumber(tr.find('input[target="height"]').val());
    // there is no "content" value for rectangle. 
    // we need a least some integer.
    if(wv == "" || hv == ""){
        $('#' + item).hide();
        return;
    }
    $('#' + item).show();
    position($("#" + item),align,x,y,wv,hv);
}
function newRectangle(){
    var newtr=recblock_tr.clone(true);
    var newid ='rectangle-' +(1+$("#input-table tr.rectangles").length);
    $(newtr).attr('target',newid);

    $("#rectangle-1").after($("<div id=\""+ newid +"\" class=\"rectangle-blocks\"></div>"));
    $("#endofrectangle").before(newtr);
    moveable($("#"+newid));
    return newid;
}

function dejsonRectangles(blocks){
    $.each(blocks,function(i,json){
        if(i==0) tid = 'rectangle-1';
        else tid =newRectangle();
        var tr = $('#input-table tr.rectangles[target="' + tid +'"]');
        if(typeof json["x"] !="undefined") tr.find('input[target="posx"]').val(json.x);       
        if(typeof json["y"] !="undefined") tr.find('input[target="posy"]').val(json.y);    
        if(typeof json["w"] !="undefined") tr.find('input[target="width"]').val(json.w);
        if(typeof json["h"] !="undefined") tr.find('input[target="height"]').val(json.h);

        if(typeof json["fc"] !="undefined"){
            tr.find('input[target="fill"]').val(json.fc);
            $("#"+ tid).css("background-color","#" + json.fc);
        }
        if(typeof json["r"] !="undefined"){
            tr.find('input[target="corner"]').val(json.r);
            $("#"+ tid).css("border-radius", json.r + "px");
        }
        if(typeof json["bw"] !="undefined"){
            tr.find('input[target="border"]').val(json.bw);
            $("#"+ tid).css("border-width",json.bw + "px");
        }
        if(typeof json["bc"] !="undefined"){
            tr.find('input[target="color"]').val(json.bc);
            $("#"+ tid).css("border-color","#" + json.bc);
        }

        calRectangle(tid);
    });
}

function djsonWgtStyle(tr,json){
    if(typeof json["a"] !="undefined") $(tr).find('select[target="align"]').val(json.a);

    if(typeof json["x"] !="undefined") $(tr).find('input[target="posx"]').val(json.x);    
    if(typeof json["y"] !="undefined") $(tr).find('input[target="posy"]').val(json.y);

    if(typeof json["w"] !="undefined" && json.w != "c") $(tr).find('input[target="width"]').val(json.w);
    if(typeof json["h"] !="undefined" && json.h != "c") $(tr).find('input[target="height"]').val(json.h);

    if(typeof json["fc"] !="undefined") $(tr).find('input[target="color"]').val(json.fc);    
    if(typeof json["bg"] !="undefined") $(tr).find('input[target="bg-color"]').val(json.bc);    
    if(typeof json["sc"] !="undefined") $(tr).find('input[target="shadow-color"]').val(json.sc);    
    $(tr).find('input[target="show"]').prop('checked',true);
}

function rssiBarFromJson(target, json){
    var prop={};    
    if(typeof json["fc"] !="undefined") prop["color"] = json.fc;
    if(typeof json["sc"] !="undefined") prop["shadow-color"] = json.sc;

    var wgt = new Rssibar("#"+target,prop);
    RssiWidgets[target]=wgt;

    return wgt;
}

function processJson(json){
    $.each(json, function(key, value) {
        if(key =="bg"){
            //backgroud image
        }else if(key == "cbg"){

        }else if(key == "sbg"){
        }else if(key == "stxs"){
            // this must be process before stxt    
        }else if(key == "stxt"){
            // process style if any.
            if(typeof json["stxs"] !="undefined") dejsonStaticTextStyle(json["stxs"]);
            else dejsonStaticTextStyle({});

            dejsonStaticTexts(value);
            applyStaticTextStyle();
        }else if(key== "cbs"){
            dejsonRectangles(value);
        }else if(key== "click"){
        }else if(key== "c1" || key== "c2" || key== "c3"){
            
        }else{
            // assume bpl type
            var tr=$('#input-table tr.brewpi-item[target="'+ key +'"]');
            if(typeof value['rep'] != "undefined" && value.rep =="wgt"){

                var wgttarget = $(tr).attr("exclusive");
                var wtr=$('#input-table tr.brewpi-item[target="'+ wgttarget +'"]');
                djsonWgtStyle(wtr,value);
 
                $('#'+wgttarget).show();
                // create the widget
                var wgt=rssiBarFromJson(wgttarget,value);
                positionRssiBarWidget(wgttarget,wtr);
                if(typeof value["bc"] != "undefined") $("#" + wgttarget).css("background-color","#"+value.bc);
                wgt.setRssi($(wtr).find('input[target="value"]').val());
            }else{                
                if(tr.length==0) alert("unknown item:"+key);
                djsonTextStyle(tr,value);
                applyBplStyle(key,value);
            }
        }
    });
}

function moveObject(item,dx,dy){

    function offset(pos,off){
        var old =parseInt(pos.val());
        old = isNaN(old)? 0:old;
        pos.val(old + off);    
    }
    var target = $(item).attr("id");
    console.log(target + " offset:("+dx +","+dy+")");
    var tr=$('#input-table tr[target="'+ target +'"]');
    
    offset(tr.find('input[target="posx"]'),dx);

    offset(tr.find('input[target="posy"]'),dy);
}

function moveable(obj){
    obj.draggable({
        containment: "#lcd",
        stop: function (event, ui) {
          // Get the position relative to the parent
          var position = ui.position;
          var op=ui.originalPosition;
          //console.log("Stop position:", position," start:",window.start);
          moveObject(this,parseInt(position.left - op.left) ,parseInt(position.top - op.top ));
        }
      });
}
function getCommonProp(tr){
    var json={};

    var align = $(tr).find('select[name="align"]').val();
    if(align !="tl") json.a=align;
    var x = parseInt($(tr).find('input[target="posx"]').val());
    if(!isNaN(x) && x!=0 ) json.x = x;
    var y = parseInt($(tr).find('input[target="posy"]').val());
    if(!isNaN(y) && y!=0 ) json.y = y;

    var w= getDimNumber($(tr).find('input[target="width"]').val());
    if(w!="") json.w=w;
    var h= getDimNumber($(tr).find('input[target="height"]').val());
    if(h!="") json.h = h;
    return json;
}

function getTextJson(tr){
    var json=getCommonProp(tr);
    
    var tf=$(tr).find('select[target="font"]').val();
    if(tf && tf !="s") json.tf = tf;
    
    var tc= $(tr).find('input[target="color"]').val();
    if(tc != "") json.tc = tc;

    var tamap={center:"c",left:"l",right:"r"};
    var ta=$(tr).find('select[target="text-align"]').val();
    if(ta !="left") json.ta=tamap[ta];
    return json;
}

function getRssiJson(tr){
    var json=getCommonProp(tr);
    json.rep="wgt";
    var fc= $(tr).find('input[target="color"]').val();
    if(fc != "") json.fc = fc;
    var bc= $(tr).find('input[target="bg-color"]').val();
    if(bc != "") json.bg = bc;
    var sc= $(tr).find('input[target="shadow-color"]').val();
    if(sc != "") json.sc = sc;

    return json;
}

function generateJson(){
    var json={};
    var blocks=[];
    $("#input-table tr.rectangles").each(function(i,tr){
        var wv= getDimNumber($(tr).find('input[target="width"]').val());
        var hv= getDimNumber($(tr).find('input[target="height"]').val());
        if(wv!="" && hv!=""){
            var rectangle={w:wv,h:hv};
            var align = $(tr).find('select[name="align"]').val();
            if(align !="tl") rectangle.a=align;
            var x = parseInt($(tr).find('input[target="posx"]').val());
            if(!isNaN(x) && x!=0 ) rectangle.x = x;
            var y = parseInt($(tr).find('input[target="posy"]').val());
            if(!isNaN(y) && y!=0 ) rectangle.y = y;

            var fc=$(tr).find('input[target="fill"]').val().trim();
            if(fc != "") rectangle.fc = fc;
            var radius = parseInt($(tr).find('input[target="corner"]').val()); 
            if(!isNaN(radius) && radius !=0) rectangle.r = radius;
            var bw= parseInt($(tr).find('input[target="border"]').val());
            if(!isNaN(bw) && bw!=0) rectangle.bw = bw;

            var bc=$(tr).find('input[target="color"]').val().trim();
            if(bc !="")rectangle.bc = bc;
            blocks.push(rectangle);
        }
    });
    if (blocks.length > 0) json.cbs=blocks;
    var stxt=[];
    $("#input-table tr.static-text,tr.brewpi-item").each(function(i,tr){
        if($(tr).hasClass("static-text")){
            var text = $(tr).find('input[target="value"]').val().trim();
            if(text != ""){
                var statxt=getTextJson(tr);
                statxt.tx = text;
                stxt.push(statxt);
            }
        }else if($(tr).hasClass("brewpi-item")){
           if($(tr).find('input[target="show"]').prop('checked')){
                if($(tr).hasClass("rssi-widget")){
                    json[$(tr).attr('exclusive')] = getRssiJson(tr);
                }else json[$(tr).attr('target')] = getTextJson(tr);
           }
        }
    });

    if(stxt.length > 0){
        // text style
        var tr=$("#input-table tr.ststyles");
        var stxs={};
        var tf=$(tr).find('select[target="font"]').val();
        if(tf !="s") stxs.tf = tf;
        
        var tc= $(tr).find('input[target="color"]').val();
        if(tc != "") stxs.tc = tc;
        var td= $(tr).find('input[target="decoration"]').val();
        if(td !="n") stxs.td = td;
        if(Object.keys(stxs)>0) json.stxs=stxs;
        json.stxt = stxt;
    }
    //return JSON.stringify(json);

    return prettierJson(json);
}
function prettierJson(json){
    var out="{\n";
    var comma=false;
    $.each(json,function(key,val){

        if(comma) out +=",\n";
        else comma = true;
        
        if(Array.isArray(val)){
            out += '"' + key + '":[';
            $.each(val,function(idx,item){
                if(idx >0) out +=",\n";
                out += JSON.stringify(item);
            });

            out += "]";
        }else{
            out += '"' + key + '":' + JSON.stringify(val);
        }
    });

    out += "\n}";
    return out;
}

/************************* */

function imageRead(img){
    // remove image if any
    $(".lcd-background img").remove();
    var mImg=$("<img>");
    mImg.attr("src",img);
    $(".lcd-background").append(mImg);
}

function readImage(file) {
    // Check if the file is an image.
    if (file.type && !file.type.startsWith('image/')) {
      console.log('File is not an image.', file.type, file);
      return;
    }
  
    const reader = new FileReader();
    reader.addEventListener('load', (event) => {
      imageRead(event.target.result);
    });
    reader.readAsDataURL(file);
  }


var startUpload = function(files) {
    //console.log(files);
    $.each(files,function(i,file){
        readImage(file);
    });
}
function initFileDrop(){
$("#js-upload-form").on('submit', function(e) {
    var uploadFiles = document.getElementById('js-upload-files').files;
    e.preventDefault()

    startUpload(uploadFiles);
    return false;
});

$("#drop-zone")[0].ondrop=function(e) {
    e.preventDefault();
    this.className = 'upload-drop-zone';

    startUpload(e.dataTransfer.files)
    return false;
};

$("#drop-zone").on('dragover', function() {
    this.className = 'upload-drop-zone drop';
    return false;
});

$("#drop-zone").on('dragleave',function() {
    this.className = 'upload-drop-zone';
    return false;
});
}

/********************** */
// RSSI Widget
function positionRssiBarWidget(target,tr){
    var iw = parseInt($(tr).find('input[target="width"]').val());
    var ih = parseInt($(tr).find('input[target="height"]').val());
    $("#" + target).width(isNaN(iw)? 16:iw);
    $("#" + target).height(isNaN(ih)? 16:ih);

    var align = $(tr).find('select[target="align"]').val();
    var x =  $(tr).find('input[target="posx"]').val();
    var y = $(tr).find('input[target="posy"]').val();

    $("#" + target).css("top",'');
    $("#" + target).css("bottom",'');
    $("#" + target).css("left",'');
    $("#" + target).css("right",'');


    if(align == "c"){
        var w = $("#" + target).width();
        var h = $("#" + target).height();
        var pw = $("#" + target).parent().width();
        var ph = $("#" + target).parent().height();
        $("#" + target).css("top",y + (ph-h)/2);
        $("#" + target).css("left",x  + (pw-w)/2);
    }else if(align == "tl"){
        // top left
        $("#" + target).css("top",y);
        $("#" + target).css("left",x);
    }else if(align == "bl"){
        // bottom left
        $("#" + target).css("bottom",y);
        $("#" + target).css("left",x);

    }else if(align == "lm"){
        // left middle
        var h = $("#" + target).height();
        var ph = $("#" + target).parent().height();

        $("#" + target).css("top",y + (ph-h)/2);
        $("#" + target).css("left",x);

    }else if(align == "tr"){
        // top right
        $("#" + target).css("top",y);
        $("#" + target).css("right",x);

    }else if(align == "br"){
        // bottom right
        $("#" + target).css("bottom",y);
        $("#" + target).css("right",x);
    }else if(align == "rm"){
        // right middle
        var h = $("#" + target).height();
        var ph = $("#" + target).parent().height();
        $("#" + target).css("top",y + (ph-h)/2);
        $("#" + target).css("right",x);
    }

}

function rssiWidgetInput(input){
    var target=$(input).closest("tr").attr("target");
    var wgt=RssiWidgets[target];
    if( typeof wgt == "undefined"){
        // create the widget
        wgt = new Rssibar("#"+target,{});
        RssiWidgets[target]=wgt;
        var w = parseInt($(input).closest("tr").find('input[target="width"]').val());
        var h = parseInt($(input).closest("tr").find('input[target="height"]').val());
        $("#" + target).width(isNaN(w)? 16:w);
        $("#" + target).height(isNaN(h)? 16:h);
    }
    var prop=$(input).attr("target");
    if(prop == "show"){
        if($(input).prop("checked"))  $("#"+ target).show();
        else $("#"+ target).hide();
        var cp=$(input).closest("tr").attr("exclusive");
        if(typeof cp != "undefined"){
            if($(input).prop("checked")){
                $("#"+ cp).hide();
                $('tr[target="'+ cp +'"]').find('input[target="show"]').prop("checked",false);
            }
        }
    }else if(prop == "value"){
        // do nothing, will set before return of function.
    }else if( prop == "align" || prop == "posx" || prop == "posy" || prop == "width" || prop == "height"){    
        positionRssiBarWidget(target,$(input).closest("tr"));
    }else if(prop == "bg-color"){
        $("#" + target).css("background-color","#"+$(input).val());
    }else if(prop == "color"){
        if($(input).val().trim() == "") wgt.rmProp("color");
        else wgt.updateProp({color:"#"+$(input).val().trim()});
    }else if(prop == "shadow-color"){
        if($(input).val().trim() == "") wgt.rmProp("shadow");
        else wgt.updateProp({shadow:"#"+$(input).val().trim()});
    }
    wgt.setRssi($(input).closest("tr").find('input[target="value"]').val());
}

/********************** */


var stxt_tr;
var recblock_tr;
$(function(){
    // hide all brewpi items
    $.each($("#input-table tr.brewpi-item"),function(i,tr){
        var i= $(tr).attr("target");
        $("#"+ i).hide();
        if(! $(tr).hasClass("rssi-widget")) $(tr).find('input[target="value"').val($("#"+ i).text());
    });
    $("#add-text").click(function(){
        newStaticText();
    });
    $("#add-block").click(function(){
        newRectangle();
    });
    $("#input-table input").change(function(){
        if($(this).closest("tr").hasClass("ststyles")) return staticTextStyleInput(this);
        
        if($(this).closest("tr").hasClass("rssi-widget")) return rssiWidgetInput(this);

        var target=$(this).closest("tr").attr("target");
        var prop=$(this).attr("target");
        if(prop == "show"){
            if($(this).prop("checked"))  $("#"+ target).show();
            else $("#"+ target).hide();
            var cp=$(this).closest("tr").attr("exclusive");
            if(typeof cp != "undefined"){
                if($(this).prop("checked")){
                    $("#"+ cp).hide();
                    $('tr[target="'+ cp +'"]').find('input[target="show"]').prop("checked",false);
                }
            }
        }else if(prop == "value"){            
            $("#"+ target).text($(this).val());
            if($(this).closest("tr").hasClass("static-text")){
                if($(this).val().trim() == ""){
                    $("#"+ target).hide();
                }else{
                    $("#"+ target).show();
                }
            }
        }else if(prop =="color"){
            if($(this).closest("tr").hasClass("rectangles")) $("#"+ target).css("border-color","#" + $(this).val());
            else $("#"+ target).css("color","#" + $(this).val());
        }else if(prop =="fill"){
            $("#"+ target).css("background-color","#" + $(this).val());
        }else if(prop =="border"){
            var bw= parseInt($(this).val());
            if(!isNaN(bw) && bw >0){
                $("#"+ target).css("border-width",$(this).val() + "px");
                $("#"+ target).css("border-style","solid");
            }
        }else if(prop =="corner"){
            $("#"+ target).css("border-radius",$(this).val() + "px");

        }else{
            if($(this).closest("tr").hasClass("rectangles")) calRectangle(target);
            else calTextPosition(target);
        }
    });
    // font
    $('select[name="font"').change(function(){
        if($(this).closest("tr").hasClass("ststyles")) return staticTextStyleInput(this);

        var target=$(this).closest("tr").attr("target");
        if(! $(this).val()) changeFont($("#"+target),"font-s");
        else changeFont($("#"+target),"font-" + $(this).val());
        calTextPosition($("#"+target));
    });
    // text alignment
    $('select[name="text-align"').change(function(){
        var target=$(this).closest("tr").attr("target");
        $("#" + target).css("text-align",$(this).val());
    });
    $('select[name="align"').change(function(){
        if($(this).closest("tr").hasClass("rssi-widget")) return rssiWidgetInput(this);
        calTextPosition($(this).closest("tr").attr("target"));
    });

    // decoration 
    $('select[name="decoration"').change(function(){
        staticTextStyleInput(this);
    });

    $("#parse-json").click(function(){
        var json;
        try{
            json= JSON.parse($("#json-input").val());
        }catch(e){
            alert("Error:"+e);
        }
        resetInput();
        processJson(json);
    });

    $("#generate-json").click(function(){
       $("#json-input").val(generateJson());
    });

    // clone a tr for static text
    stxt_tr = $("#input-table tr.static-text").clone(true);
    recblock_tr  = $("#input-table tr.rectangles").clone(true);
      // Attach the mousemove event to the myDiv element
      $('#lcd').mousemove(function updateCoordinates(event) {
        // Get the mouse coordinates
        var mouseX = event.pageX - $('#lcd').offset().left;
        var mouseY = event.pageY - $('#lcd').offset().top;
  
        // Update the content of the coordinates paragraph
        $('#coordinates').text('(' + mouseX + ', ' + mouseY + ')');
      });
    $(".lcd-dim").change(function(){
        $("#lcd").width(parseInt($("#lcd-width").val()));
       $("#lcd").height(parseInt($("#lcd-height").val()));
    });
    moveable($(".lcd-text, .rectangle-blocks, .rssi-bars"));
    initFileDrop();
});