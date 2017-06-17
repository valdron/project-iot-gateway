window.onload = function () {




    client = new Paho.MQTT.Client("localhost", 1884, "", "visualId");

    client.onConnectionLost = onConnectionLost;
    client.onMessageArrived = onMessageArrived;

    client.connect({ onSuccess: onConnect });

    function onConnect() {
        // Once a connection has been made, make a subscription and send a message.
        console.log("onConnect");
        client.subscribe("#");
    }

    function onConnectionLost(responseObject) {
        if (responseObject.errorCode !== 0) {
            console.log("onConnectionLost:" + responseObject.errorMessage);
        }
    }


    var dpsTemp = []; // dataPoints

    for (i = 0; i < 40; i++) {
        dpsTemp.push({
            x: i,
            y: undefined
        })
    }

    var tempchart = new CanvasJS.Chart("tempchartContainer", {
        title: {
            text: "Temperatur in Grad Celcius"
        },
        data: [{
            type: "line",
            dataPoints: dpsTemp
        }]
    });

    var dpsPressure = []; // dataPoints

     for(i = 0; i < 40;i++) {
         dpsPressure.push({
             x: i,
             y: undefined
         })
     }

     var pressurechart = new CanvasJS.Chart("pressurechartContainer", {
        title: {
            text: "Druck"
        },
        data: [{
            type: "line",
            dataPoints: dpsPressure
        }]
    });

    

    var tempxVal = 40;
    var pressurexVal = 40
    var dataLength = 40; // number of dataPoints visible at any point


    // called when a message arrives
    function onMessageArrived(message) {
        console.log("onMessageArrived:" + message.payloadString + " onTopic:" + message.destinationName);
        var msgjson = JSON.parse(message.payloadString);
        console.log("Incoming value:" + typeof msgjson.value);
        if (message.destinationName == "/maschine1/temp/current") {
            dpsTemp.push({
                x: tempxVal,
                y: msgjson.value
            });
            tempxVal++;
            dpsTemp.shift();
            tempchart.render();
        } else if (message.destinationName == "/maschine1/pressure/current") {
            dpsPressure.push({
                x: pressurexVal,
                y: msgjson.value
            });
            pressurexVal++;
            dpsPressure.shift();
            pressurechart.render();
        }



    }

    // var updateChart = function (count) {
    //     count = count || 1;
    //     // count is number of times loop runs to generate random dataPoints.

    //     for (var j = 0; j < count; j++) {
    //         yVal = yVal + Math.round(5 + Math.random() * (-5 - 5));
    //         dpsTemp.push({
    //             x: tempxVal,
    //             y: yVal
    //         });
    //         tempxVal++;
    //     };
    //     if (dps.length > dataLength) {
    //         dps.shift();
    //     }

    //     chart.render();
    // };
    // generates first set of dataPoints
    // update chart after specified time. 
}