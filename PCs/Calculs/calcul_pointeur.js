// Hauteur du pointeur sur la base (à mesurer!!!):
const pointeurHeight = 30;  // cm

/**
 * Calcule les positions des servomoteurs pour correctement placer le pointeur lumineux sur la cible
 * @param x {int} : position de la cible selon l'axe x
 * @param y {int} : position de la cible selon l'axe y
 * @param z {int} : position de la cible selon l'axe z
 * @return pos = { H:int, V:int }
 */
exports.computeServoPositions = (Xc, Yc, Zc) => {

  // Angle du vecteur entre la base et la cible (Xc, Yc):
  let angleH = -Math.atan2(Xc, Yc) * 180 / Math.PI;

  //console.log(`x: ${Xc}, y: ${Yc} => angleH: ${angleH}`);

  // Vecteur vertical (longueur, z) :
  let z = Zc - pointeurHeight;
  let longueur = Math.sqrt(Math.pow(Xc, 2) + Math.pow(Yc, 2));
  let angleV = -Math.atan2(longueur, z) * 180 / Math.PI;

  //console.log(`z: ${z}, longueur: ${longueur}, angleV: ${angleV}`);

  // Si le vecteur horizontal est dirigé à l'opposé de la cible:
  if (angleH < 0) {
    angleH = 180 - Math.abs(angleH);
    angleV = -angleV;
  }

  // Conversion entre [-90, 90]:
  angleH -= 90;

  // Arrondi les angles:
  angleH = Math.round(angleH);
  angleV = Math.round(angleV);

  //console.log(`angleH: ${angleH}, angleV: ${angleV}`);

  let pos = { H: angleH, V: angleV };
  return pos
}