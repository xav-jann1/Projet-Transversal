#include "BASE.h"
#include "../Serializer/SRLZR.h"

#include <math.h>  // pour atan2()
#define M_PI 3.14159265358979323846

/**
 * Paramètres de la Base:
 */

char BASE_speed = 20;                  // %
float BASE_rayon_entre_roues = 112.0;  // mm

/**  "TV vitesse"
 * Définit la vitesse de déplacement de la base
 * @param  {char} v : 5-100 %, vitesse désirée
 * @return {bit} 0: ok, 1: error
 */
bit BASE_setSpeed(char v) {
  // Vérification de la vitesse: 5 <= v <= 100
  if (v < 5 || v > 100) return 1;

  BASE_speed = v;
  return 0;
}

/**  "A [vitesse]"
 * Fait reculer la base avec la vitesse définit en paramètre ou par
 * BASE_setSpeed()
 * @param  {char} v : 5-100 %, vitesse désirée ; 0 = vitesse de BASE_setSpeed()
 * @return {bit} 0: ok, 1: error
 */
bit BASE_forward_v(char v) {
  // Si vitesse nulle:
  if (v == 0)
    // Vitesse définie par la commande TV:
    return SRLZR_pwm(BASE_speed, BASE_speed);

  // Vérification de la vitesse: 5 <= v <= 100
  if (v < 5 && v > 100) return 1;
  return SRLZR_pwm(v, v);
}

/**  "B [vitesse]"
 * Fait reculer la base avec la vitesse définit en paramètre ou par
 * BASE_setSpeed()
 * @param  {char} v : 5-100 % = vitesse désirée ; 0 = vitesse de BASE_setSpeed()
 * @return {bit} 0: ok, 1: error
 */
bit BASE_backard_v(char v) {
  // Si vitesse nulle:
  if (v == 0) return SRLZR_pwm(-BASE_speed, -BASE_speed);

  // Vérification de la vitesse: 5 <= v <= 100
  if (v < 5 && v > 100) return 1;
  return SRLZR_pwm(-v, -v);
}

/**  "S"
 * Arrête les moteurs, pour les commandes BASE_forward() et BASE_backward()
 * @return {bit} 0: ok, 1: error
 */
bit BASE_stop() { return SRLZR_stop(); }

/**  "RA sens:valeur"
 * Effectue une rotation 'optimisée' dans un sens et un angle précisés
 * @param {char} sens : 'G' ou 'D', sens de la rotation
 * @param {inr} valeur : 0-180, angle de rotation à réaliser
 * @return {bit} 0: ok, 1: error
 */
bit BASE_rotate(char sens, int valeur) {
  float theta;
  int d, v;
  // Vérification du sens:
  if (sens != 'G' && sens != 'D') return 1;

  // Vérification de l'angle:
  if (valeur > 180 && valeur != 360) return 1;

  // Calcul de la distance à parcourir par une roue:
  theta = (valeur / 360.0f) * M_PI;
  d = (int)(theta * BASE_rayon_entre_roues);

  // Vitesse de rotation:
  v = 10;  // TODO: définir une vitesse optimale

  if (sens == 'G')
    // Tourne à gauche:
    return SRLZR_digo(d, v, -d, -v);
  else
    // Tourne à droite:
    return SRLZR_digo(-d, -v, d, v);
}

/**  "RG"
 * Effectue une rotation 'optimisée' de 90° à gauche
 * @return {bit} 0: ok, 1: error
 */
bit BASE_rotate90Left() { return BASE_rotate('G', 90); }

/**  "RD"
 * Effectue une rotation 'optimisée' de 90° à droite
 * @return {bit} 0: ok, 1: error
 */
bit BASE_rotate90Right() { return BASE_rotate('D', 90); }

/**  "RC [D/G]"
 * Effectue une rotation 'optimisée' complète à droite ou à gauche
 * @param {char} sens : 'G' ou 'D', sens de la rotation (par défaut droite)
 * @return {bit} 0: ok, 1: error
 */
bit BASE_fullRotation(char sens) { return BASE_rotate(sens, 360); }

/**  "G X:valeur_x Y :valeur_y A:angle"
 * Déplace la base à des coordonnées relatives
 * Un message est envoyé lorsque la position est atteinte
 * @param {char} x : -99 - 99 dm, déplacement selon l'axe x
 * @param {char} y : -99 - 99 dm, déplacement selon l'axe y
 * @param {int}  a : -180 - 180°, position angulaire à la fin du déplacement
 * @return {bit} 0: ok, 1: error
 */
bit BASE_moveTo(char x, char y, int a) {
  int d, v;
  float new_theta;

  // Angle en direction de la position demandée:
  float theta = atan2(y, x);

  // Tourne la base en direction de la position demandée:
  if (theta > 0)
    BASE_rotate('G', (int)theta);
  else
    BASE_rotate('D', (int)-theta);

  // Attend la fin de la rotation:
  // wait()

  // Déplace la base:
  d = sqrt(x ^ 2 + y ^ 2);
  v = 10;  // TODO: définir une vitesse optimale
  SRLZR_digo(d, v, d, v);

  // Attend la fin du déplacement:
  // wait()

  // Correction de la rotation pour s'orienter selon l'angle finale:
  new_theta = theta - a;

  // Tourne la base à la position de fin:
  if (new_theta > 0)
    BASE_rotate('G', (int)new_theta);
  else
    BASE_rotate('D', (int)-new_theta);

  return 0;
}