const fs = require('fs');
const calcul_pointeur = require('./calcul_pointeur.js');

// Chargement et conversion du fichier:
let filename = './exemple_data.json';
let file = JSON.parse(fs.readFileSync(filename, 'utf8'));

// Vérification:
if (!verif(file, ['Xc', 'Yc', 'Zc'])) return;

// Position de la cible par rapport à la base:
let Xc = file.Xc, Yc = file.Yc, Zc = file.Zc;

// Calcul les positions des servomoteurs:
let pos = calcul_pointeur.computeServoPositions(Xc, Yc, Zc);
console.log(`pos.H = ${pos.H} vs 33;\t pos.V = ${pos.V} vs 25`);

// Enregistre le résultat dans un fichier:
let result_name = 'exemple_result.json';
fs.writeFileSync(result_name, JSON.stringify(pos, null, 2), 'utf8');


// Vérification que l'objet contient toutes les valeurs nécessaires:
function verif(object, values) {
  let isOK = true;
  values.map(value => {
    // Si la valeur ne se trouve pas dans le fichier:
    if (!(value in file)) {
      console.log(`Error: '${value}' ne se trouve pas dans l'objet`);
      isOK = false;
    }
  })
  return isOK;
}