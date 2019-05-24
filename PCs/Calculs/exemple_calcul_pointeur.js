const calcul_pointeur = require('./calcul_pointeur.js');

// Position de la cible par rapport à la base:
let Xc, Yc, Zc = 100;  // cm

// Positions des servomoteurs:
let pos;

/** Calcule les positions des servomoteurs dans différents cadrans du repère **/

// Haut - Droite:
[Xc, Yc] = [80, 125];
pos = calcul_pointeur.computeServoPositions(Xc, Yc, Zc);
console.log(`pos.H =  ${pos.H} vs  33  ;\t pos.V =  ${pos.V} vs  25`);

// Haut - Gauche:
[Xc, Yc] = [-80, 125];
pos = calcul_pointeur.computeServoPositions(Xc, Yc, Zc);
console.log(`pos.H = ${pos.H} vs -33  ;\t pos.V =  ${pos.V} vs  25`);

// Bas - Droite:
[Xc, Yc] = [80, -125];
pos = calcul_pointeur.computeServoPositions(Xc, Yc, Zc);
console.log(`pos.H = ${pos.H} vs -33  ;\t pos.V = ${pos.V} vs -25`);

// Bas - Gauche:
[Xc, Yc] = [-80, -125];
pos = calcul_pointeur.computeServoPositions(Xc, Yc, Zc);
console.log(`pos.H =  ${pos.H} vs  33  ;\t pos.V = ${pos.V} vs -25`);
