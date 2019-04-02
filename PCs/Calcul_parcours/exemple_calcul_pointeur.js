const calcul_pointeur = require('./calcul_pointeur.js');

// Fichiers du terrain et du chemin:
const terrain_filename = './Fichiers JSON Exemples/Description Environnement v10-05-17.json';
const chemin_filename  = './Fichiers JSON Exemples/Evolution Base Roulante v10-05-17.json';

// Calcul les positions des servomoteurs:
let pos = calcul_pointeur.computeServoPositions(terrain_filename, chemin_filename);

console.log(pos);