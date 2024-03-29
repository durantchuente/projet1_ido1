
  $.get("/data", (data, status) => {
    dataString = JSON.stringify(text);
    dataJson = JSON.parse(dataString);
    $('#temp2').html((dataJson.temperature.value).toFixed+ ` °C`)
    $('#hum2').html((dataJson.humidity.value).toFixed+ ` %`)
    $('#gas2').html((dataJson.gas.value).toFixed+ ` ppm`)
  // $('#hum2').html(data.current.humidity+ ` %`)
  // $('#alarmResponse').html(data.current.pressure_mb)
  // $('#gas2').html(data.current.wind_kph+ ` ppm`)
  });
// });