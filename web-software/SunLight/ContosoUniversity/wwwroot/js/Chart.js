function mainChart() {
    var dataPointsLDR1 = [];
    var dataPointsLDR2 = [];
    var dataPointsLDR3 = [];
    var dataPointsLDR4 = [];
    var dataPointsLDR5 = [];
    var dataPointsTemperature = [];
    var dataPointsLux = [];
    var dataPointsHeatIndex = [];
    var LDRChart;
    var TemperatureChart;
    var LuxChart;
    var HeatIndexChart;
    //new Date(value.createDate)

    $.getJSON("https://localhost:44335/api/students", function (data) {
        $.each(data, function (key, value) {
            dataPointsLDR1.push({ x: value.id, y: value.ldR1 });
            dataPointsLDR2.push({ x: value.id, y: value.ldR2 });
            dataPointsLDR3.push({ x: value.id, y: value.ldR3 });
            dataPointsLDR4.push({ x: value.id, y: value.ldR4 });
            dataPointsLDR5.push({ x: value.id, y: value.ldR5 });
            dataPointsTemperature.push({ x: value.id, y: value.temperature });
            dataPointsLux.push({ x: value.id, y: value.lux });
            dataPointsHeatIndex.push({ x: value.id, y: value.heatIndex });

        });
        LDRChart = new CanvasJS.Chart("LDRChartContainer", {
            animationEnabled: true,
            zoomEnabled: true,
            theme: "light2",
            title: {
                text: "LDR1-LDR2-LDR3-LDR4-LDR5 Table"
            },
            axisX: {
                //valueFormatString: "DD MMM",
                title: "Data Point",
                crosshair: {
                    enabled: true,
                    snapToDataPoint: false
                }
            },
            axisY: {
                title: "LDR Value",
                includeZero: true,
                crosshair: {
                    enabled: true
                }
            },
            toolTip: {
                shared: true
            },
            legend: {
                cursor: "pointer",
                itemclick: toogleDataSeries
            },
            data: [{
                type: "spline",
                showInLegend: true,
                name: "LDR1",
                //markerType: "square",
                lineDashType: "solid",
                color: "#12253f",
                dataPoints: dataPointsLDR1,
            },
            {
                type: "spline",
                showInLegend: true,
                name: "LDR2",
                lineDashType: "shortDash",
                color: "#6666ff",
                dataPoints: dataPointsLDR2,
            },
            {
                type: "spline",
                showInLegend: true,
                name: "LDR3",
                lineDashType: "dash",
                color: "#ffd700",
                dataPoints: dataPointsLDR3,
            },
            {
                type: "spline",
                showInLegend: true,
                name: "LDR4",
                lineDashType: "shortDashDot",
                color: "#40e0d0",
                dataPoints: dataPointsLDR4,
            },
            {
                type: "spline",
                showInLegend: true,
                name: "LDR5",
                lineDashType: "shortDashDotDot",
                color: "#40ff80",
                dataPoints: dataPointsLDR5,
            },



            ]
        });


        TemperatureChart = new CanvasJS.Chart("TempChartContainer", {
            animationEnabled: true,
            zoomEnabled: true,
            theme: "light2",
            title: {
                text: "Temperature Table"
            },
            axisX: {
                //valueFormatString: "DD MMM hh:mm:ss",
                title: "Data Point",
                crosshair: {
                    enabled: true,
                    snapToDataPoint: false
                }
            },
            axisY: {
                title: "Temp °C",
                suffix: " °C",
                includeZero: true,
                crosshair: {
                    enabled: true
                }
            },
            toolTip: {
                shared: true
            },
            legend: {
                cursor: "pointer",
                itemclick: toogleDataSeries
            },
            data: [{
                type: "spline",
                showInLegend: true,
                yValueFormatString: "#0.## °C",
                name: "°C",
                //markerType: "square",
                lineDashType: "solid",
                color: "#DE3163",
                dataPoints: dataPointsTemperature,
            },
            ]
        });


        LuxChart = new CanvasJS.Chart("LuxChartContainer", {
            animationEnabled: true,
            zoomEnabled: true,
            theme: "light2",
            title: {
                text: "Lux Table"
            },
            axisX: {
                //valueFormatString: "DD MMM",
                title: "Data Point",
                crosshair: {
                    enabled: true,
                    snapToDataPoint: false
                }
            },
            axisY: {
                title: "Lux lx",
                suffix: " lx",
                includeZero: true,
                crosshair: {
                    enabled: true
                }
            },
            toolTip: {
                shared: true
            },
            legend: {
                cursor: "pointer",
                itemclick: toogleDataSeries
            },
            data: [{
                type: "spline",
                showInLegend: true,
                yValueFormatString: "#0.## lx",
                name: "lx",
                //markerType: "square",
                lineDashType: "solid",
                color: "#ff8c00",
                dataPoints: dataPointsLux,
            },
            ]
        });

        HeatIndexChart = new CanvasJS.Chart("HeatIndexChartContainer", {
            animationEnabled: true,
            zoomEnabled: true,
            theme: "light2",
            title: {
                text: "Heat Index Table"
            },
            axisX: {
                //valueFormatString: "DD MMM hh:mm:ss",
                title: "Data Point",
                crosshair: {
                    enabled: true,
                    snapToDataPoint: false
                }
            },
            axisY: {
                title: "Heat Index °C",
                suffix: " °C",
                includeZero: true,
                crosshair: {
                    enabled: true
                }
            },
            toolTip: {
                shared: true
            },
            legend: {
                cursor: "pointer",
                itemclick: toogleDataSeries
            },
            data: [{
                type: "spline",
                showInLegend: true,
                yValueFormatString: "#0.## °C",
                name: "°C",
                //markerType: "square",
                lineDashType: "solid",
                color: "#ffdd00",
                dataPoints: dataPointsHeatIndex,
            },
            ]
        });


        LDRChart.render();
        TemperatureChart.render();
        LuxChart.render();
        HeatIndexChart.render();
        updateChart();
    });

    function toogleDataSeries(e) {
        if (typeof (e.dataSeries.visible) === "undefined" || e.dataSeries.visible) {
            e.dataSeries.visible = false;
        } else {
            e.dataSeries.visible = true;
        }
        LDRChart.render();
        TemperatureChart.render();
        LuxChart.render();
        HeatIndexChart.render();
    }

    function updateChart() {
        $.getJSON("https://localhost:44335/api/students" + "/" + (dataPointsLDR1.length), function (value) {

            dataPointsLDR1.push({
                x: value.id, y: value.ldR1
            });
            dataPointsLDR2.push({
                x: value.id, y: value.ldR2
            });
            dataPointsLDR3.push({
                x: value.id, y: value.ldR3
            });
            dataPointsLDR4.push({
                x: value.id, y: value.ldR4
            });
            dataPointsLDR5.push({
                x: value.id, y: value.ldR5
            });
            dataPointsTemperature.push({
                x: value.id, y: value.temperature
            });
            dataPointsLux.push({
                x: value.id, y: value.lux
            });
            dataPointsHeatIndex.push({
                x: value.id, y: value.heatIndex
            });


            LDRChart.render();
            TemperatureChart.render();
            LuxChart.render();
            HeatIndexChart.render();
            
        });
    }
    setInterval(function () {
        updateChart();
    }, 1000);
}
