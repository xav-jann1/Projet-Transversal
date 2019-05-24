const SerialPort = require("serialport");
const port = new SerialPort("/dev/ttyS22", { baudRate: 19200 });

port.on('open', function () {
  send('S\r');
});

// open errors will be emitted as an error event
port.on('error', function (err) {
  console.log('Error: ', err.message);
});

// When receive data
port.on("open", function () {
  console.log('open');
  port.on('data', function (data) {
    console.log('data received: ' + data);
  });
});

/* Sending data to the server
   @param text : send the text on the serial port
*/
function send(text) {
  port.write(text + '\r', function (err) {
    if (err) {
      return console.log('Error on write: ', err.message);
    }
    console.log('message written: ' + text);
  });
}

module.exports = { send };