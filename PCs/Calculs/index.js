const express = require('express')
const app = express();
app.use(express.static('public'));
app.use(express.json());

// Scripts
const calcul_pointeur = require('./calcul_pointeur.js');
const calcul_trajectoire = require('./calcul_trajectoire.js');

app.listen(3001, function () {
  console.log('Serveur de Calcul connecté au port 3001');
})

app.post('/calcul_pointeur', function (req, res) {
  //console.log(req.body);
  let Xc = req.body.Xc, Yc = req.body.Yc, Zc = req.body.Zc;
  console.log(`Calcul du pointeur en cours:\n  - Xc = ${Xc}\n  - Yc = ${Yc}\n  - Zc = ${Zc}`);

  let pos = calcul_pointeur.computeServoPositions(Xc, Yc, Zc);
  console.log(`Résultat du calcul du pointeur: H = ${pos.H}, V = ${pos.V}`);
  
  res.send(pos);
})

app.post('/calcul_trajectoire', function (req, res) {
  //console.log(req.body);
  let data = req.body;
  let terrain = data.terrain;
  let chemin = data.chemin;

  console.log(`Calcul de la trajectoire en cours`);

  // Calculs:
  let trajectoire = calcul_trajectoire.compute(terrain, chemin);
  
  console.log(`Fin du calcul de trajectoire`);
  res.send(trajectoire)
})


