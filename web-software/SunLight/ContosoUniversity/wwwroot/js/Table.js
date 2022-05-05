console.log("TableJavaScript");
function mainTable() {
    console.log("In getAllMessages();");
    var tbl = $('#messagesTable');
    var i = 1;
    console.log("In windowonload");

    function roundToTwo(num) {
        return num.toFixed(2);
    }

    $.ajax({
        url: '/api/students',
        contentType: 'application/html ; charset:utf-8',
        type: 'GET',
        dataType: 'json',
        success: function (result) {
            console.log(result);
            tbl.empty();
            $.each(result, function (key, value) {
                //streetaddress.substring(0, streetaddress.indexOf(',')); anyString.length 
                tbl.append(
                    '<tr>' +
                    '<td>' + i + '</td>' +
                    '<td>' + value.createDate.substring(0, value.createDate.indexOf('T')) + '</td>' +
                    '<td>' + value.createDate.substring(value.createDate.indexOf('T') + 1, 19) + '</td>' +
                    '<td>' + ((value.ldR1 + value.ldR2 + value.ldR3 + value.ldR4 + value.ldR5) / 5) + " %" + '</td>' +
                    '<td>' + roundToTwo(value.humidity) + " %" + '</td>' +
                    '<td>' + roundToTwo(value.heatIndex) + " °C" + '</td>' +
                    '<td>' + roundToTwo(value.airPressure) + " Pa" + '</td>' +
                    '<td>' + roundToTwo(value.altitude) + " m" + '</td>' +
                    '<td>' + roundToTwo(value.lux) + " lx" + '</td>' +
                    '<td>' + roundToTwo(value.temperature) + " °C" + '</td>'
                    + '</tr>'
                );
                i = i + 1;
            }); updateTable();
        }
    });

    function updateTable() {
        console.log("uptadeTable");
        $.getJSON("https://localhost:44335/api/students" + "/" + (i))
            .done(function (value) {
                console.log("JSON Data: " + value);
                tbl.append(
                    '<tr>' +
                    '<td>' + i + '</td>' +
                    '<td>' + value.createDate.substring(0, value.createDate.indexOf('T')) + '</td>' +
                    '<td>' + value.createDate.substring(value.createDate.indexOf('T') + 1, 19) + '</td>' +
                    '<td>' + ((value.ldR1 + value.ldR2 + value.ldR3 + value.ldR4 + value.ldR5) / 5) + " %" + '</td>' +
                    '<td>' + roundToTwo(value.humidity) + " %" + '</td>' +
                    '<td>' + roundToTwo(value.heatIndex) + " °C" + '</td>' +
                    '<td>' + roundToTwo(value.airPressure) + " Pa" + '</td>' +
                    '<td>' + roundToTwo(value.altitude) + " m" + '</td>' +
                    '<td>' + roundToTwo(value.lux) + " lx" + '</td>' +
                    '<td>' + roundToTwo(value.temperature) + " °C" + '</td>'
                    + '</tr>'
                );
                i = i + 1;
            })
            .fail(function (jqxhr, textStatus, error) {
                var err = textStatus + ", " + error;
                console.log("Request Failed: " + err);
            });
    }
    setInterval(function () {
        updateTable();
    }, 1000);
}
