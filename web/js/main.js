var lastmsg = "";
var lastcommand = "";
var objectsList;
var objplotlineList;

var requestIndex = 0;



var line1 = new TimeSeries();
var line2 = new TimeSeries();


function openWebSocket()
{
    var url = "ws://" + $("#ip").val() + ":" + $("#port").val() + "/" ;

    websocket = new WebSocket(url);

    websocket.onopen = function(evt) { onOpen(evt) };
    websocket.onclose = function(evt) { onClose(evt) };
    websocket.onmessage = function(evt) { onMessage(evt) };
    websocket.onerror = function(evt) { onError(evt) };
}


function objectDiscovery()
{
    if (websocket.readyState == WebSocket.OPEN)
    {
        lastcommand="discovery";
        websocket.send(lastcommand);
    }
}

function plotData()
{
    var queryInterval = $('#interval').val();

    //Updating function
    setInterval(function()
    {
        askForData();
    }, queryInterval);

     for(i=0; i<objplotlineList.length; i++)
     {
     console.log("Create Chart:" +objplotlineList[i].plotline +"-"+ objplotlineList[i].object);

     var smoothie = new SmoothieChart({ millisPerPixel:70, grid: { fillStyle:'#ffffff',strokeStyle:'#d4d4d4',sharpLines:true,verticalSections:6}, labels:{fillStyle:'#808080'}, maxValue:100,minValue:50 });
     smoothie.addTimeSeries(objplotlineList[i].plotline, { lineWidth:1.7,strokeStyle:'#000000'});
     smoothie.streamTo(document.getElementById(objplotlineList[i].object+"canvas"), queryInterval);

     }

}




<!-- WebSockets Messages Callback -->
function onOpen(evt)
{
    var url = "ws://" + $("#ip").val() + ":" + $("#port").val() + "/" ;
    console.log("WebSocket Connected @"+url);
    $("#webSocketStatus").html("Connected");
    $("#webSocketStatus").css("background-color", "#10ee09");
    $("#webSocketStatus").css("color", "black");

    $("#discoverButton").prop("disabled", false);

}

function onError(evt)
{
    console.log(evt.data);
    websocket.close();

}

function askForData() {

    lastcommand = "";
    if (requestIndex == (objectsList.length -1))
        requestIndex = 0;
    if (websocket.readyState == WebSocket.OPEN && requestIndex == 0 )
    {
        websocket.send(objectsList[0]);
    }
}


<!-- Received Messages Callback -->
function onMessage(evt)
{

    lastmsg = evt.data;

    if(lastcommand == "discovery")
    {
        objectsList = lastmsg.split(", ");
        objplotlineList = new Array(objectsList.length);

        $("#availableObjects").html("");
        for(i=0; i<objectsList.length; i++)
        {
            $("#availableObjects").append( "<input type='checkbox'>"+ objectsList[i]+"<br>" );

            var plotline = new TimeSeries();
            var objplotline = {object:objectsList[i], plotline: plotline};
            objplotlineList[i] = objplotline;

            $("#responseField").append("<label id="+objectsList[i]+"label></label></br>");
            $("#responseField").append("<canvas id="+objectsList[i]+"canvas width='1000' height='200'></canvas></br>");

        }

        $("#availableObjects").append("<br><label>Refresh Interval: </label> "+
            "  <select id='interval'> "+
            "  <option value='1000'>1 sec</option> "+
            "  <option value='2000'>2 sec</option> "+
            "  <option value='5000'>5 sec</option> "+
            "  <option value='10000'>10 sec</option> "+
            "  </select><br>");

        $("#availableObjects").append("<button  id='plotButton' onclick='plotData()' style='width: 185px; margin-bottom: 10px' >Plot Data</button><br>");
    }

    else
    {
        var data = evt.data;
        var plotPoint = parseFloat(data);
        var time = new Date().getTime();

        var objPlotLine = objplotlineList[requestIndex];

        console.log("OUT ReqIndex: "+requestIndex);

        objPlotLine.plotline.append(time, plotPoint);

        $("#"+objPlotLine.object+"label").html(data);

        console.log("Label for: "+objPlotLine.object +">> lastcommand: "+lastcommand +">> value: " +data);

        //Call the next command
        var newindex = requestIndex + 1;
        console.log("newIndex:" +requestIndex);
        if (newindex < objectsList.length)
        {
            requestIndex = newindex;
            console.log("IN ReqIndex: "+requestIndex);
            websocket.send(objectsList[newindex]);
        }

    }


}


