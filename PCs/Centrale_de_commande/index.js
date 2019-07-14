//const serial = require('./serial.js');

//const IP_CALCUL = 'http://192.168.137.151:3000';
const IP_CALCUL = 'http://localhost:3001';

// Scripts:
const robot = require('./robot.js');
const utils = require('./utils.js');
const serial = require('./serial.js');

// Librairies:
const request = require('request-promise');
const express = require('express');
const app = express();
let server = app.listen(3000);
app.use(express.static('./public'));
console.log("Socket server is running: http://localhost:3000")

let socket = require('socket.io')
let io = socket(server);

io.sockets.on('connection', socket => {
  console.log('connection:', socket.id);
  socket.on('IHM', data => {
    serial.send(data);
    console.log(`Envoie la commande: '${data}'`);
  });

  socket.on('trajectoire', async (data) => {
    console.log('!!! Calcul trajectoire en cours !!!');

    let trajectoire = await getTrajectoire(data);

    // Envoie la trajectoire:
    if (trajectoire) socket.emit('trajectoire', trajectoire);
  });

  socket.on('pointeur', async (data) => {
    let Xc = data.Xc - data.Xr;
    let Yc = data.Yc - data.Yr;
    let Zc = data.Zc;
    console.log(data);
    let pos = await getPointeurPositions({ Xc, Yc, Zc });
    console.log(pos);
    let H = pos.H, V = pos.V;

    // Vérification:
    if (Number.isInteger(H) && Number.isInteger(V)) {
      // Envoie des commande au Robot:
      robot.setServo('H', H);
      await utils.wait(2);
      robot.setServo('V', V);

      // Envoie le résultat:
      socket.emit('pointeur', pos);
    } else console.log('Request Response Error: H et V ne sont pas des entiers');
  });

});


// Récupère les coordonnées des servos pour une position de la cible:
async function getPointeurPositions(data) {
  let Xc = data.Xc, Yc = data.Yc, Zc = data.Zc;

  if (!Number.isInteger(Xc) || !Number.isInteger(Yc) || !Number.isInteger(Zc)) {
    console.log("getPointeurPosition Error: Xc ou Yc ou Zc n'est pas entier")
    return false;
  }

  let options = {
    uri: `${IP_CALCUL}/calcul_pointeur`,
    method: 'POST',
    json: { Xc, Yc, Zc }
  };

  let result = await request(options);

  return result;
}

// Récupère la trajectoire que le robot doit parcourir:
async function getTrajectoire(data) {
  // Vérification:
  if (!('chemin' in data) || !('terrain' in data)) {
    console.log("getTrajectoire Error: l'objet ne contient pas les clés 'chemin' et/ou 'chemin'");
    return false;
  }

  let options = {
    uri: `${IP_CALCUL}/calcul_trajectoire`,
    method: 'POST',
    json: data
  };

  let result = await request(options);

  return result;
}
