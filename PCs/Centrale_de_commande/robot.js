const serial = require('./serial.js');

exports.setServo = (servo, pos) => {
  serial.send(`CS ${servo} A:${pos}`)
}