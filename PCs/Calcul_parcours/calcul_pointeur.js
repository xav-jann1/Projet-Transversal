const fs = require('fs');

// Hauteur du pointeur sur la base (à mesurer!!!):
const pointeurHeight = 30;  // cm

/**
 * Calcul les positions des servomoteurs pour correctement placer le pointeur lumineux sur la cible
 * @param terrain_filename {string} : chemin vers le fichier qui décrit le terrain
 * @param chemin_filename  {string} : chemin vers le fichier qui décrit le chemin de la base sur le terrain
 * @return pos = { H:int, V:int }
 */
exports.computeServoPositions = (terrain_filename, chemin_filename) => {
  // Charge les fichiers:
  let terrain = fs.readFileSync(terrain_filename);
  let chemin = fs.readFileSync(chemin_filename);

  // Conversion en objet:
  terrain = JSON.parse(terrain);
  chemin = JSON.parse(chemin);

  // Position de la cible:
  let xc = terrain.cible.centre.x;
  let yc = terrain.cible.centre.y;

  // Position de la base:
  let xb = chemin["position-tir_cible"].coordonnees.x;
  let yb = chemin["position-tir_cible"].coordonnees.y;
  let angle_cible = chemin["position-tir_cible"].angle;

  // Vecteur horizontal entre base et cible (x, y):
  let x = xc - xb;
  let y = yc - yb;

  // Angle du vecteur:
  let angleH = - (Math.atan2(y, x) * 180 / Math.PI) + angle_cible;

  console.log(`x: ${x}, y: ${y}, angleH: ${angleH}`);

  // Vecteur vertical (longueur, z) :
  let z = terrain.cible.hauteur - pointeurHeight;
  let longueur = Math.sqrt(Math.pow(x, 2) + Math.pow(y, 2));
  let angleV = Math.atan2(z, longueur) * 180 / Math.PI - 90;

  console.log(`z: ${z}, longueur: ${longueur}, angleV: ${angleV}`);

  // Si le vecteur horizontal est dirigé à l'opposé de la cible:
  if (Math.abs(angleH) > 90) {
    angleH = angleH - Math.sign(angleH) * 180;
    angleV += 90;
  }

  console.log(`angleH: ${angleH}, angleV: ${angleV}`);

  // Arrondi les angles:
  angleH = Math.round(angleH);
  angleV = Math.round(angleV);

  let pos = { H: angleH, V: angleV };
  return pos
}