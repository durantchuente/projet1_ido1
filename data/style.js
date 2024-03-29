$.get("/data", (data, status) => {
  dataJson = JSON.parse(data);
  $('#temp2').html((dataJson.temperature.value).toFixed(2)+ ` °C`);
  $('#hum2').html((dataJson.humidity.value).toFixed(2)+ ` %`);
  $('#gas2').html((dataJson.gas.value).toFixed(2)+ ` ppm`);
  $('#tempMessage').html(dataJson.temperature.message);
  $('#humMessage').html(dataJson.humidity.message);
  $('#co2Message').html(dataJson.gas.message);
  if (dataJson.temperature.message == "Anormal")
    $('#tempMessage').addClass( "text-danger" );
  else
    $('#tempMessage').addClass( "text-success" );
  if (dataJson.humidity.message == "Anormal")
    $('#humMessage').addClass( "text-danger" );
  else
    $('#humMessage').addClass( "text-success" );
  if (dataJson.gas.message == "Anormal")
    $('#co2Message').addClass( "text-danger" );
  else
    $('#co2Message').addClass( "text-success" );
});