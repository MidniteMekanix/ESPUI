const char JS_CONTROLS[] PROGMEM = R"=====(
const UI_INITIAL_GUI=200;const UI_RELOAD=201;const UPDATE_OFFSET=100;const UI_EXTEND_GUI=210;const UI_TITEL=0;const UI_PAD=1;const UPDATE_PAD=101;const UI_CPAD=2;const UPDATE_CPAD=102;const UI_BUTTON=3;const UPDATE_BUTTON=103;const UI_LABEL=4;const UPDATE_LABEL=104;const UI_SWITCHER=5;const UPDATE_SWITCHER=105;const UI_SLIDER=6;const UPDATE_SLIDER=106;const UI_NUMBER=7;const UPDATE_NUMBER=107;const UI_TEXT_INPUT=8;const UPDATE_TEXT_INPUT=108;const UI_GRAPH=9;const ADD_GRAPH_POINT=10;const CLEAR_GRAPH=109;const UI_TAB=11;const UPDATE_TAB=111;const UI_SELECT=12;const UPDATE_SELECT=112;const UI_OPTION=13;const UPDATE_OPTION=113;const UI_MIN=14;const UPDATE_MIN=114;const UI_MAX=15;const UPDATE_MAX=115;const UI_STEP=16;const UPDATE_STEP=116;const UI_GAUGE=17;const UPDATE_GAUGE=117;const UI_ACCEL=18;const UPDATE_ACCEL=118;const UI_SEPARATOR=19;const UPDATE_SEPARATOR=119;const UI_TIME=20;const UPDATE_TIME=120;const UP=0;const DOWN=1;const LEFT=2;const RIGHT=3;const CENTER=4;const C_TURQUOISE=0;const C_EMERALD=1;const C_PETERRIVER=2;const C_WETASPHALT=3;const C_SUNFLOWER=4;const C_CARROT=5;const C_ALIZARIN=6;const C_DARK=7;const C_NONE=255;var graphData=new Array();var hasAccel=false;var sliderContinuous=false;function colorClass(colorId){colorId=Number(colorId);switch(colorId){case C_TURQUOISE:return"turquoise";case C_EMERALD:return"emerald";case C_PETERRIVER:return"peterriver";case C_WETASPHALT:return"wetasphalt";case C_SUNFLOWER:return"sunflower";case C_CARROT:return"carrot";case C_ALIZARIN:return"alizarin";case C_DARK:case C_NONE:return"dark";default:return"";}}
var websock;var websockConnected=false;function requestOrientationPermission(){}
function saveGraphData(){localStorage.setItem("espuigraphs",JSON.stringify(graphData));}
function restoreGraphData(id){var savedData=localStorage.getItem("espuigraphs",graphData);if(savedData!=null){savedData=JSON.parse(savedData);return savedData[id];}
return[];}
function restart(){$(document).add("*").off();$("#row").html("");websock.close();start();}
function conStatusError(){websockConnected=false;$("#conStatus").removeClass("color-green");$("#conStatus").addClass("color-red");$("#conStatus").html("Error / No Connection &#8635;");$("#conStatus").off();$("#conStatus").on({click:restart,});}
function handleVisibilityChange(){if(!websockConnected&&!document.hidden){restart();}}
function start(){document.addEventListener("visibilitychange",handleVisibilityChange,false);if(window.location.port!=""||window.location.port!=80||window.location.port!=443){websock=new WebSocket("ws://"+window.location.hostname+":"+window.location.port+"/ws");}else{websock=new WebSocket("ws://"+window.location.hostname+"/ws");}
websock.onopen=function(evt){console.log("websock open");$("#conStatus").addClass("color-green");$("#conStatus").text("Connected");websockConnected=true;};websock.onclose=function(evt){console.log("websock close");conStatusError();};websock.onerror=function(evt){console.log(evt);conStatusError();};var handleEvent=function(evt){console.log(evt);var data=JSON.parse(evt.data);var e=document.body;var center="";switch(data.type){case UI_INITIAL_GUI:$("#row").html("");$("#tabsnav").html("");$("#tabscontent").html("");if(data.sliderContinuous){sliderContinuous=data.sliderContinuous;}
data.controls.forEach(element=>{var fauxEvent={data:JSON.stringify(element),};handleEvent(fauxEvent);});if(data.totalcontrols>(data.controls.length-1)){websock.send("uiok:"+(data.controls.length-1));}
break;case UI_EXTEND_GUI:data.controls.forEach(element=>{var fauxEvent={data:JSON.stringify(element),};handleEvent(fauxEvent);});if(data.totalcontrols>data.startindex+(data.controls.length-1)){websock.send("uiok:"+(data.startindex+(data.controls.length-1)));}
break;case UI_RELOAD:window.location.reload();break;case UI_TITEL:document.title=data.label;$("#mainHeader").html(data.label);break;case UI_LABEL:case UI_NUMBER:case UI_TEXT_INPUT:case UI_SELECT:case UI_GAUGE:case UI_SEPARATOR:if(data.visible)addToHTML(data);break;case UI_BUTTON:if(data.visible){addToHTML(data);$("#btn"+data.id).on({touchstart:function(e){e.preventDefault();buttonclick(data.id,true);},touchend:function(e){e.preventDefault();buttonclick(data.id,false);},});}
break;case UI_SWITCHER:if(data.visible){addToHTML(data);switcher(data.id,data.value);}
break;case UI_CPAD:case UI_PAD:if(data.visible){addToHTML(data);$("#pf"+data.id).on({touchstart:function(e){e.preventDefault();padclick(UP,data.id,true);},touchend:function(e){e.preventDefault();padclick(UP,data.id,false);},});$("#pl"+data.id).on({touchstart:function(e){e.preventDefault();padclick(LEFT,data.id,true);},touchend:function(e){e.preventDefault();padclick(LEFT,data.id,false);},});$("#pr"+data.id).on({touchstart:function(e){e.preventDefault();padclick(RIGHT,data.id,true);},touchend:function(e){e.preventDefault();padclick(RIGHT,data.id,false);},});$("#pb"+data.id).on({touchstart:function(e){e.preventDefault();padclick(DOWN,data.id,true);},touchend:function(e){e.preventDefault();padclick(DOWN,data.id,false);},});$("#pc"+data.id).on({touchstart:function(e){e.preventDefault();padclick(CENTER,data.id,true);},touchend:function(e){e.preventDefault();padclick(CENTER,data.id,false);},});}
break;case UI_SLIDER:if(data.visible){addToHTML(data);rangeSlider(!sliderContinuous);}
break;case UI_TAB:if(data.visible){$("#tabsnav").append("<li><a onmouseup='tabclick("+data.id+")' href='#tab"+data.id+"'>"+data.value+"</a></li>");$("#tabscontent").append("<div id='tab"+data.id+"'></div>");tabs=$(".tabscontent").tabbedContent({loop:true}).data("api");$("a").filter(function(){return $(this).attr("href")==="#click-to-switch";}).on("click",function(e){var tab=prompt("Tab to switch to (number or id)?");if(!tabs.switchTab(tab)){alert("That tab does not exist :\\");}
e.preventDefault();});}
break;case UI_OPTION:if(data.parentControl){var parent=$("#select"+data.parentControl);parent.append("<option id='option"+
data.id+
"' value='"+
data.value+
"' "+
data.selected+
">"+
data.label+
"</option>");}
break;case UI_MIN:if(data.parentControl){var parent=$("#id"+data.parentControl+" input");if(parent.size()){parent.attr("min",data.value);}}
break;case UI_MAX:if(data.parentControl){var parent=$("#id"+data.parentControl+" input");if(parent.size()){if(!parent.attr("type")){parent.attr("maxlength",data.value);}else{parent.attr("max",data.value);}}}
break;case UI_STEP:if(data.parentControl){var parent=$("#id"+data.parentControl+" input");if(parent.size()){parent.attr("step",data.value);}}
break;case UI_GRAPH:if(data.visible){addToHTML(data);graphData[data.id]=restoreGraphData(data.id);renderGraphSvg(graphData[data.id],"graph"+data.id);}
break;case ADD_GRAPH_POINT:var ts=Math.round(new Date().getTime()/1000);graphData[data.id].push({x:ts,y:data.value});saveGraphData();renderGraphSvg(graphData[data.id],"graph"+data.id);break;case CLEAR_GRAPH:graphData[data.id]=[];saveGraphData();renderGraphSvg(graphData[data.id],"graph"+data.id);break;case UI_ACCEL:if(hasAccel)break;hasAccel=true;if(data.visible){addToHTML(data);requestOrientationPermission();}
break;case UPDATE_LABEL:$("#l"+data.id).html(data.value);if(data.hasOwnProperty('elementStyle')){$("#l"+data.id).attr("style",data.elementStyle);}
break;case UPDATE_SWITCHER:switcher(data.id,data.value=="0"?0:1);if(data.hasOwnProperty('elementStyle')){$("#sl"+data.id).attr("style",data.elementStyle);}
break;case UPDATE_SLIDER:$("#sl"+data.id).attr("value",data.value)
slider_move($("#id"+data.id),data.value,"100",false);if(data.hasOwnProperty('elementStyle')){$("#sl"+data.id).attr("style",data.elementStyle);}
break;case UPDATE_NUMBER:$("#num"+data.id).val(data.value);if(data.hasOwnProperty('elementStyle')){$("#num"+data.id).attr("style",data.elementStyle);}
break;case UPDATE_TEXT_INPUT:$("#text"+data.id).val(data.value);if(data.hasOwnProperty('elementStyle')){$("#text"+data.id).attr("style",data.elementStyle);}
break;case UPDATE_SELECT:$("#select"+data.id).val(data.value);if(data.hasOwnProperty('elementStyle')){$("#select"+data.id).attr("style",data.elementStyle);}
break;case UPDATE_BUTTON:$("#btn"+data.id).val(data.value);$("#btn"+data.id).text(data.value);if(data.hasOwnProperty('elementStyle')){$("#btn"+data.id).attr("style",data.elementStyle);}
break;case UPDATE_PAD:case UPDATE_CPAD:break;case UPDATE_GAUGE:$("#gauge"+data.id).val(data.value);if(data.hasOwnProperty('elementStyle')){$("#gauge"+data.id).attr("style",data.elementStyle);}
break;case UPDATE_ACCEL:break;case UPDATE_TIME:var rv=new Date().toISOString();websock.send("time:"+rv+":"+data.id);break;default:console.error("Unknown type or event");break;}
if(data.type>=UPDATE_OFFSET&&data.type<UI_INITIAL_GUI){var element=$("#id"+data.id);if(data.hasOwnProperty('panelStyle')){$("#id"+data.id).attr("style",data.panelStyle);}
if(data.type==UPDATE_SLIDER){element.removeClass("slider-turquoise slider-emerald slider-peterriver slider-wetasphalt slider-sunflower slider-carrot slider-alizarin");element.addClass("slider-"+colorClass(data.color));}else{element.removeClass("turquoise emerald peterriver wetasphalt sunflower carrot alizarin");element.addClass(colorClass(data.color));}}
$(".range-slider__range").each(function(){$(this)[0].value=$(this).attr("value");$(this).next().html($(this).attr("value"));});};websock.onmessage=handleEvent;}
function sliderchange(number){var val=$("#sl"+number).val();websock.send("slvalue:"+val+":"+number);$(".range-slider__range").each(function(){$(this).attr("value",$(this)[0].value);});}
function numberchange(number){var val=$("#num"+number).val();websock.send("nvalue:"+val+":"+number);}
function textchange(number){var val=$("#text"+number).val();websock.send("tvalue:"+val+":"+number);}
function tabclick(number){var val=$("#tab"+number).val();websock.send("tabvalue:"+val+":"+number);}
function selectchange(number){var val=$("#select"+number).val();websock.send("svalue:"+val+":"+number);}
function buttonclick(number,isdown){if(isdown)websock.send("bdown:"+number);else websock.send("bup:"+number);}
function padclick(type,number,isdown){switch(type){case CENTER:if(isdown)websock.send("pcdown:"+number);else websock.send("pcup:"+number);break;case UP:if(isdown)websock.send("pfdown:"+number);else websock.send("pfup:"+number);break;case DOWN:if(isdown)websock.send("pbdown:"+number);else websock.send("pbup:"+number);break;case LEFT:if(isdown)websock.send("pldown:"+number);else websock.send("plup:"+number);break;case RIGHT:if(isdown)websock.send("prdown:"+number);else websock.send("prup:"+number);break;}}
function switcher(number,state){if(state==null){if(!$("#sl"+number).hasClass("checked")){websock.send("sactive:"+number);$("#sl"+number).addClass("checked");}else{websock.send("sinactive:"+number);$("#sl"+number).removeClass("checked");}}else if(state==1){$("#sl"+number).addClass("checked");$("#sl"+number).prop("checked",true);}else if(state==0){$("#sl"+number).removeClass("checked");$("#sl"+number).prop("checked",false);}}
var rangeSlider=function(isDiscrete){var range=$(".range-slider__range");var slidercb=function(){sliderchange($(this).attr("id").replace(/^\D+/g,""));};range.on({input:function(){$(this).next().html(this.value)}});range.each(function(){$(this).next().html(this.value);if($(this).attr("callbackSet")!="true"){if(!isDiscrete){$(this).on({input:slidercb});}else{$(this).on({change:slidercb});}
$(this).attr("callbackSet","true");}});};var addToHTML=function(data){panelStyle=data.hasOwnProperty('panelStyle')?" style='"+data.panelStyle+"' ":"";panelwide=data.hasOwnProperty('wide')?"wide":"";if(!data.hasOwnProperty('parentControl')||$("#tab"+data.parentControl).length>0){var parent=data.hasOwnProperty('parentControl')?$("#tab"+data.parentControl):$("#row");var html="";switch(data.type){case UI_LABEL:case UI_BUTTON:case UI_SWITCHER:case UI_CPAD:case UI_PAD:case UI_SLIDER:case UI_NUMBER:case UI_TEXT_INPUT:case UI_SELECT:case UI_GRAPH:case UI_GAUGE:case UI_ACCEL:html="<div id='id"+data.id+"' "+panelStyle+" class='two columns "+panelwide+" card tcenter "+
colorClass(data.color)+"'><h5>"+data.label+"</h5><hr/>"+
elementHTML(data)+
"</div>";break;case UI_SEPARATOR:html="<div id='id"+data.id+"' "+panelStyle+" class='sectionbreak columns'>"+
"<h5>"+data.label+"</h5><hr/></div>";break;case UI_TIME:break;}
parent.append(html);}else{var parent=$("#id"+data.parentControl);parent.append(elementHTML(data));}}
var elementHTML=function(data){var id=data.id
var elementStyle=data.hasOwnProperty('elementStyle')?" style='"+data.elementStyle+"' ":"";switch(data.type){case UI_LABEL:return"<span id='l"+id+"' "+elementStyle+
" class='label label-wrap'>"+data.value+"</span>";case UI_BUTTON:return"<button id='btn"+id+"' "+elementStyle+
" onmousedown='buttonclick("+id+", true)'"+
" onmouseup='buttonclick("+id+", false)'>"+
data.value+"</button>";case UI_SWITCHER:return"<label id='sl"+id+"' "+elementStyle+
" class='switch "+(data.value=="1"?"checked":"")+
(data.hasOwnProperty('vertical')?" vert-switcher ":"")+
"'>"+
"<div class='in'>"+
"<input type='checkbox' id='s"+id+"' onClick='switcher("+id+",null)' "+
(data.value=="1"?"checked":"")+"/></div></label>";case UI_CPAD:case UI_PAD:return"<nav class='control'><ul>"+
"<li><a onmousedown='padclick(UP, "+id+", true)' "+
"onmouseup='padclick(UP, "+id+", false)' id='pf"+id+"'>&#9650;</a></li>"+
"<li><a onmousedown='padclick(RIGHT, "+id+", true)' "+
"onmouseup='padclick(RIGHT, "+id+", false)' id='pr"+id+"'>&#9650;</a></li>"+
"<li><a onmousedown='padclick(LEFT, "+id+", true)' "+
"onmouseup='padclick(LEFT, "+id+", false)' id='pl"+id+"'>&#9650;</a></li>"+
"<li><a onmousedown='padclick(DOWN, "+id+", true)' "+
"onmouseup='padclick(DOWN, "+id+", false)' id='pb"+id+"'>&#9650;</a></li>"+
"</ul>"+
(data.type==UI_CPAD?"<a class='confirm' onmousedown='padclick(CENTER,"+id+", true)' "+
"onmouseup='padclick(CENTER, "+id+", false)' id='pc"+id+"'>OK</a>":"")+
"</nav>";case UI_SLIDER:return"<div class='range-slider "+
(data.hasOwnProperty('vertical')?" vert-slider ":"")+
"'>"+
"<input id='sl"+id+"' type='range' min='0' max='100' value='"+data.value+"' "+
elementStyle+" class='range-slider__range'><span class='range-slider__value'>"+
data.value+"</span></div>";case UI_NUMBER:return"<input style='color:black;' "+elementStyle+" id='num"+id+
"' type='number' value='"+data.value+"' onchange='numberchange("+id+")' />";case UI_TEXT_INPUT:return"<input style='color:black;' "+elementStyle+" id='text"+id+
"' value='"+data.value+"' onchange='textchange("+id+")' />";case UI_SELECT:return"<select style='color:black;' "+elementStyle+" id='select"+id+
"' onchange='selectchange("+id+")' />";case UI_GRAPH:return"<figure id='graph"+id+"'><figcaption>"+data.label+"</figcaption></figure>";case UI_GAUGE:return"WILL BE A GAUGE <input style='color:black;' id='gauge"+id+
"' type='number' value='"+data.value+"' onchange='numberchange("+id+")' />";case UI_ACCEL:return"ACCEL // Not implemented fully!<div class='accelerometer' id='accel"+id+
"' ><div class='ball"+id+"'></div><pre class='accelerometeroutput"+id+"'></pre>";default:return"";}}
)=====";

const uint8_t JS_CONTROLS_GZIP[3941] PROGMEM = { 31,139,8,0,36,32,244,97,2,255,197,91,91,119,218,72,18,126,247,175,80,148,57,65,172,49,151,220,38,3,150,115,8,38,9,59,216,120,13,158,204,217,76,214,71,136,198,232,88,72,26,73,248,50,14,255,125,171,171,47,234,22,2,51,120,102,247,37,70,213,165,234,175,171,186,187,110,138,27,6,73,106,92,244,46,123,167,189,81,175,221,191,252,116,209,179,95,214,235,45,87,12,156,119,251,131,246,49,208,26,130,118,118,220,30,117,47,7,31,63,14,187,35,187,161,242,118,127,29,117,79,143,153,140,134,66,31,245,70,221,190,173,16,206,64,98,78,30,146,178,73,122,151,29,74,121,169,51,117,24,215,203,140,235,195,197,104,52,56,181,95,233,124,156,218,168,191,202,56,251,237,15,0,226,181,206,200,136,141,250,235,140,111,248,165,55,234,124,238,158,219,111,116,86,73,111,212,223,40,220,253,222,49,208,222,230,120,25,181,81,127,155,113,158,94,156,124,0,218,143,58,39,167,54,234,63,42,234,2,61,130,65,206,46,70,246,59,157,91,25,105,212,223,101,111,124,58,111,159,125,182,127,226,132,246,241,49,163,92,158,13,122,167,148,149,15,116,250,221,246,57,103,110,212,127,82,102,108,127,176,27,57,131,48,154,98,144,97,183,223,237,128,180,156,77,4,185,161,88,101,112,54,234,81,253,231,204,34,200,13,197,44,39,61,32,228,172,130,180,134,98,147,147,246,175,118,35,103,15,164,53,84,83,140,186,103,118,35,111,9,36,54,20,59,124,106,95,124,234,218,141,156,29,56,181,161,152,161,221,233,208,189,145,51,1,167,54,222,169,138,57,107,159,183,71,3,48,227,79,121,221,200,145,134,170,238,222,73,23,78,84,78,223,148,216,80,168,242,192,28,15,190,156,202,227,210,239,126,28,201,99,113,222,251,244,121,36,55,127,167,123,58,130,189,36,244,214,185,28,93,156,255,235,98,208,27,118,165,168,206,101,247,164,123,222,238,103,199,175,115,121,214,133,183,206,123,191,192,171,47,37,241,75,119,212,30,158,125,110,247,21,241,151,195,139,211,143,253,193,23,109,142,78,251,252,124,48,146,135,165,115,217,238,247,254,221,62,7,11,190,149,164,227,246,249,207,114,227,119,46,79,7,167,176,252,55,111,90,55,78,108,92,197,78,52,59,118,82,199,14,200,173,209,142,99,231,222,42,227,200,204,73,218,174,75,124,123,234,248,9,65,82,226,123,19,18,119,194,32,245,130,69,184,72,248,208,116,17,184,169,23,6,134,27,250,97,220,241,157,36,177,240,103,111,82,126,224,63,236,211,197,124,76,98,73,111,37,183,94,234,206,20,62,39,33,170,206,154,49,73,23,113,96,194,63,191,47,66,47,33,102,139,179,112,21,10,6,50,39,177,227,79,228,112,166,79,193,17,145,148,196,177,119,67,98,201,148,233,87,48,221,146,212,73,162,153,227,167,146,73,234,91,240,36,139,96,234,135,183,138,28,166,126,49,238,58,113,28,102,239,11,83,136,97,199,247,254,112,98,47,144,12,212,48,77,254,155,90,69,48,78,156,248,218,108,77,200,212,89,248,169,32,154,173,229,114,143,90,225,150,140,147,208,189,110,41,191,193,36,1,113,83,50,201,91,36,38,191,47,72,146,14,98,143,4,169,67,73,103,36,158,123,73,2,191,172,242,195,114,79,114,38,206,13,249,36,54,3,12,249,161,235,248,195,52,140,157,43,82,77,72,218,75,201,220,50,73,18,45,60,220,51,137,89,249,231,112,112,90,77,82,88,209,149,55,189,183,228,86,42,151,91,138,224,24,230,15,99,69,182,7,214,198,221,4,51,78,112,235,105,115,93,21,206,149,9,111,121,83,75,190,250,204,14,22,190,95,126,200,100,33,168,200,137,19,146,113,149,91,76,135,217,148,95,189,201,55,0,201,200,95,191,229,241,58,113,10,42,248,193,154,132,238,98,14,154,43,87,157,201,196,50,255,97,150,171,225,116,10,7,228,7,203,124,30,135,183,240,60,75,231,190,101,154,229,22,55,69,213,245,67,152,27,54,56,147,162,138,134,19,56,4,43,44,146,46,236,147,24,102,88,99,62,42,93,242,194,28,49,153,135,55,132,29,44,19,79,204,193,85,76,72,96,150,87,88,1,167,198,23,147,73,1,23,3,141,40,140,154,113,26,26,28,1,5,249,226,249,187,183,175,222,180,10,222,202,150,174,81,3,235,193,245,61,247,186,201,53,87,89,106,139,158,57,193,196,39,191,120,137,55,246,124,47,189,239,0,225,10,20,244,0,118,124,150,87,192,139,23,207,132,206,171,51,111,50,33,65,249,65,26,132,30,128,108,191,114,35,73,118,88,121,247,6,126,244,189,36,37,1,92,53,230,141,156,211,197,57,205,74,49,150,10,106,29,55,214,173,23,76,194,219,42,221,144,116,150,106,20,198,233,51,219,52,191,127,47,30,121,87,95,55,242,250,245,43,105,95,188,91,191,144,241,16,126,147,212,50,111,147,102,173,102,238,231,95,156,133,73,26,56,115,178,111,54,87,7,169,212,125,179,118,11,42,111,45,9,224,221,89,56,151,177,39,54,108,24,132,17,9,108,161,89,139,220,164,244,218,14,146,208,39,240,254,21,136,100,156,6,229,219,98,207,173,219,155,41,185,3,124,210,214,217,153,201,246,127,26,47,72,107,217,202,160,225,105,218,6,27,50,130,200,252,25,211,164,17,74,219,32,141,18,138,36,48,127,72,247,14,110,177,199,36,80,246,73,238,50,130,129,234,4,239,34,58,74,108,185,111,199,225,228,30,105,46,60,145,24,54,155,240,141,148,189,154,222,71,132,123,71,61,79,104,22,220,65,148,148,58,227,36,112,110,138,200,0,20,142,70,170,14,193,158,199,105,242,174,29,174,212,188,179,47,228,131,125,132,116,42,58,14,253,164,58,13,227,174,3,224,137,79,232,242,236,35,188,235,193,147,221,49,213,61,80,246,102,206,117,112,230,114,101,217,82,212,108,201,183,192,6,25,212,52,76,29,95,204,119,100,233,211,251,36,184,74,103,7,141,178,60,124,224,188,2,184,187,23,94,120,13,199,106,45,59,44,100,28,19,231,186,37,84,157,101,83,205,255,239,10,153,222,233,117,7,71,154,220,237,239,180,226,109,222,95,213,1,203,62,155,249,187,36,38,126,232,76,224,100,232,220,152,103,54,229,198,78,189,212,39,108,215,248,206,152,248,184,13,231,142,23,124,38,14,236,33,177,11,51,134,188,60,204,14,155,226,137,165,106,242,49,203,197,36,137,165,66,242,17,115,10,101,144,231,2,77,161,101,116,15,62,41,195,245,53,10,63,143,78,250,22,59,159,58,8,150,203,174,188,244,144,127,139,46,110,156,6,230,62,178,65,140,131,142,49,13,23,238,12,117,223,204,238,140,242,3,169,70,49,161,102,63,102,49,30,85,229,34,77,233,117,7,142,212,226,34,42,244,54,4,163,84,80,10,216,116,23,25,220,183,45,153,87,214,215,38,114,234,199,87,199,110,36,112,170,66,44,99,119,124,4,152,19,75,203,4,82,239,244,247,86,202,139,166,59,235,46,114,38,108,209,23,103,149,93,117,87,36,67,213,29,66,244,159,14,145,38,144,79,7,169,73,89,129,25,63,29,38,166,182,79,199,169,139,89,1,58,126,58,80,154,157,63,29,167,38,101,5,166,251,116,152,172,56,240,116,160,57,57,27,79,55,86,193,30,63,124,49,141,129,135,232,215,173,103,43,113,192,138,216,81,251,195,170,76,61,240,112,162,8,61,208,161,239,29,29,58,70,24,204,65,18,89,68,118,9,120,216,58,164,70,247,205,114,201,152,197,100,106,151,168,4,133,94,58,226,15,120,205,236,155,135,53,231,232,176,6,34,11,35,26,57,231,196,187,49,188,9,78,165,9,59,172,193,8,125,151,190,103,131,128,170,46,0,158,198,100,210,97,207,22,228,191,97,212,164,102,90,150,49,110,179,76,39,242,216,204,14,112,79,61,31,130,53,75,154,141,166,41,152,98,254,96,165,51,47,1,60,105,10,41,8,93,152,89,182,109,27,34,97,186,238,131,52,60,96,119,41,164,243,184,153,76,164,155,21,117,3,208,120,2,224,216,81,28,206,35,8,153,71,206,216,72,67,131,189,72,127,89,1,86,83,12,72,223,96,79,190,103,113,220,51,186,158,42,99,130,55,44,120,132,152,192,241,73,76,69,204,156,148,202,52,38,33,73,140,32,76,13,114,7,137,146,209,252,237,55,76,6,10,246,92,193,150,98,21,68,105,126,136,108,225,141,14,139,36,24,108,70,162,234,125,158,64,204,227,166,220,6,58,107,139,61,102,70,11,35,76,234,168,221,216,79,115,127,79,216,110,207,44,25,184,5,236,146,160,178,29,65,7,4,133,77,70,40,247,145,160,97,80,1,132,195,26,147,121,100,174,174,232,164,183,237,114,188,73,209,82,246,77,195,11,162,69,202,76,192,151,149,120,127,64,130,91,126,16,171,196,173,48,247,2,83,119,155,43,88,218,191,254,125,88,232,6,209,240,208,196,194,92,1,233,220,177,120,48,7,21,19,206,60,103,126,57,43,23,208,168,123,246,63,82,46,164,252,209,35,218,197,186,251,227,183,161,44,52,125,229,251,239,155,189,82,193,18,206,160,5,8,224,182,196,129,225,205,149,181,250,110,197,68,90,230,63,244,13,152,107,21,52,241,224,39,246,137,147,206,170,113,184,128,163,65,147,123,144,8,43,166,133,177,145,55,135,95,181,70,189,94,47,66,90,141,22,201,204,122,184,107,166,73,229,190,153,105,3,78,114,174,188,183,11,114,5,183,210,201,104,22,40,236,235,183,191,120,62,209,14,160,230,19,117,233,50,27,151,101,106,44,29,60,238,236,54,22,68,115,215,131,210,164,194,108,91,13,255,178,220,133,111,56,49,53,0,26,220,6,103,113,24,193,197,123,111,149,120,230,55,76,239,125,82,42,51,87,169,10,18,27,24,134,249,14,86,223,40,134,36,3,247,13,113,57,248,156,186,249,190,222,108,252,57,104,201,147,177,177,176,99,141,44,132,166,157,212,61,22,114,92,210,18,167,165,221,5,240,150,194,88,49,97,219,155,74,173,238,127,183,36,158,122,82,89,224,120,21,97,0,108,231,61,160,75,218,5,150,146,2,99,60,68,238,210,191,8,91,78,212,78,219,128,37,227,43,129,192,83,161,173,8,219,5,28,79,234,87,115,246,60,178,85,14,44,98,238,10,94,23,181,11,242,44,181,206,90,242,205,85,54,86,251,160,83,94,57,139,43,242,23,233,62,47,107,151,5,176,123,188,96,51,247,78,186,232,255,226,27,91,113,122,105,216,27,14,134,88,70,179,178,30,11,43,112,165,224,13,155,230,126,124,131,5,243,156,223,16,221,51,81,156,197,210,175,101,94,4,215,65,120,27,24,52,250,161,225,51,198,188,166,120,105,185,39,75,112,48,126,100,107,31,91,188,120,33,71,14,245,82,44,11,103,68,61,48,127,135,173,85,113,228,4,196,215,20,172,190,86,160,221,236,133,114,14,170,109,107,183,47,228,144,12,140,222,58,98,55,237,129,108,169,242,134,238,1,111,161,138,199,172,95,42,40,89,115,84,80,100,43,84,16,88,239,83,60,201,86,103,185,37,144,100,93,2,206,99,238,43,253,98,94,147,132,231,178,8,55,11,151,144,97,23,160,21,180,42,76,137,143,3,219,132,104,45,142,229,30,205,19,49,65,62,224,110,234,18,159,32,3,36,180,6,172,228,127,60,241,251,90,255,198,253,175,158,9,50,199,71,111,20,70,13,232,61,194,195,136,66,78,150,128,41,205,139,57,73,18,231,138,216,74,253,88,237,179,49,124,172,211,197,243,67,182,47,65,158,45,220,32,167,227,45,144,63,78,137,143,51,195,81,130,191,120,164,56,119,235,79,43,65,119,246,121,205,240,212,82,34,103,211,108,64,142,174,114,19,244,96,29,114,101,22,122,113,111,152,131,185,188,77,147,164,219,76,34,234,27,133,83,208,138,196,198,25,156,241,22,115,48,15,184,201,208,220,69,110,52,246,22,19,169,229,100,54,92,241,146,9,220,158,152,77,242,159,186,216,49,165,41,210,232,65,54,114,44,139,168,120,58,89,227,162,23,90,37,55,33,111,137,41,221,48,86,9,107,174,67,18,185,143,67,137,92,13,139,230,148,214,11,158,110,33,120,186,78,48,45,52,174,23,189,133,250,162,241,58,209,180,38,188,94,180,191,133,104,127,157,104,44,227,174,151,29,111,33,59,46,144,173,245,243,69,38,195,13,159,64,118,70,112,163,225,47,155,127,232,65,203,24,249,203,12,252,170,232,63,207,136,123,77,59,203,249,94,88,226,192,44,55,68,191,211,52,33,74,19,91,8,209,251,236,66,146,23,60,42,75,255,94,35,19,135,242,140,108,73,141,44,63,217,4,35,207,19,65,4,145,141,139,90,114,78,118,125,85,246,26,88,143,136,23,37,102,246,241,145,82,44,206,186,224,94,114,236,37,110,76,82,94,187,68,38,123,173,215,80,190,37,115,199,182,226,61,52,23,166,187,18,136,140,232,2,34,223,113,137,85,251,207,111,199,251,181,171,138,137,94,146,21,176,177,48,143,213,162,102,129,63,82,189,45,165,112,71,180,92,242,242,247,90,71,182,230,69,26,211,233,0,93,199,247,199,142,123,61,36,16,78,62,179,77,106,21,147,237,87,85,61,226,165,12,173,80,196,82,236,55,149,133,233,66,227,217,91,63,111,133,207,218,90,46,197,87,11,178,254,145,233,25,11,33,15,89,44,105,63,26,156,190,55,13,12,67,105,1,54,23,136,238,211,26,108,211,52,91,72,186,5,152,197,226,232,8,21,68,255,34,63,85,204,154,137,149,10,96,169,252,253,187,116,157,5,117,68,222,189,62,170,107,21,197,109,228,190,223,36,54,251,182,130,125,250,1,214,223,252,93,134,222,170,230,233,229,74,155,117,109,131,52,215,178,217,189,229,141,21,185,226,6,56,75,188,216,82,100,147,68,73,56,208,146,251,170,101,13,151,94,21,118,41,189,13,233,167,157,139,121,144,8,14,106,70,202,224,196,19,35,101,31,174,208,82,124,113,32,141,221,151,217,27,209,206,97,101,121,243,176,6,164,195,89,92,163,245,122,30,146,103,133,58,44,219,99,195,38,87,7,204,250,248,187,44,37,97,31,185,161,72,177,38,218,103,130,217,54,0,44,70,130,25,171,200,27,245,134,6,133,38,142,243,86,149,238,124,71,100,69,31,242,2,86,70,242,71,154,14,131,38,184,18,84,238,13,231,92,207,243,87,78,186,58,44,207,250,99,231,128,127,177,122,152,128,9,208,54,224,93,132,85,52,129,123,210,48,168,115,3,255,61,184,141,157,104,181,249,71,133,29,153,249,239,50,196,84,44,86,197,201,176,200,178,110,58,222,139,164,1,11,112,42,1,46,123,165,98,160,55,165,141,38,83,237,91,22,113,50,199,88,58,210,155,82,128,148,49,43,88,229,5,32,208,178,213,82,176,201,227,170,225,109,63,241,37,143,40,238,54,204,247,210,73,131,81,224,196,20,87,25,32,49,78,61,240,18,104,92,250,112,32,66,45,131,191,103,242,19,64,79,18,159,211,11,56,13,125,20,86,75,236,18,206,54,14,239,74,12,185,0,30,6,29,170,23,1,148,126,121,201,52,132,17,27,182,232,30,65,110,138,35,118,88,67,205,40,170,91,185,42,133,10,3,71,130,229,159,50,193,37,179,240,25,106,173,239,204,108,173,126,223,97,232,198,166,8,77,197,216,133,172,220,218,184,116,250,169,10,107,42,191,120,254,211,219,55,245,86,214,154,126,108,118,246,65,196,182,0,114,220,26,134,120,103,12,248,241,200,182,16,116,102,13,129,191,51,2,252,220,98,91,4,58,179,134,96,188,17,65,141,109,7,173,74,198,118,212,123,19,160,101,187,103,234,197,243,210,26,168,252,131,139,45,177,114,238,98,180,174,64,59,248,153,2,21,167,239,176,6,123,89,189,45,88,16,32,54,186,114,42,213,104,58,59,86,91,28,120,254,134,126,220,217,209,214,47,33,118,208,113,158,146,49,247,64,17,117,248,235,220,217,165,70,189,174,180,225,213,11,15,181,161,251,137,34,192,60,252,135,83,138,110,161,144,3,37,22,220,168,120,247,11,47,156,139,142,132,158,216,114,184,247,194,192,163,57,134,108,225,186,181,114,181,154,184,104,44,42,241,207,11,216,178,89,2,180,118,149,112,255,99,52,46,24,121,158,194,84,7,54,174,41,216,148,80,109,87,124,172,32,149,255,254,97,29,36,165,186,85,8,136,7,138,210,51,99,137,232,79,160,17,53,37,142,39,155,88,171,70,21,78,205,130,82,49,243,212,187,90,196,4,101,242,134,47,255,60,7,6,92,135,127,167,145,139,195,148,33,124,0,1,170,124,12,115,185,252,47,189,126,223,248,208,53,218,6,146,141,77,106,71,12,172,165,241,119,237,3,22,116,115,108,248,96,212,232,127,184,128,115,55,143,152,150,201,196,152,130,163,188,127,166,158,116,135,182,180,73,28,206,105,113,155,33,69,146,68,122,164,114,143,33,7,148,122,100,142,52,2,29,23,201,10,23,41,253,160,66,50,71,168,202,130,255,106,244,95,91,230,122,124,155,58,0,0 };
