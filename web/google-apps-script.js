function doPost(e) {
  var sheet = SpreadsheetApp.getActiveSpreadsheet().getActiveSheet();
  var data = JSON.parse(e.postData.contents);

  var power = data.power;
  var networks = data.wifi;

  networks.forEach(function(net) {
    sheet.appendRow([
      new Date(),
      power,
      net.ssid,
      net.rssi,
      net.lat,
      net.lng
    ]);
  });

  return ContentService.createTextOutput("OK");
}
