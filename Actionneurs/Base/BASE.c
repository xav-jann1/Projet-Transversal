#include "BASE.h"
#include "../Serializer/SRLZR.h"
//#include "../../Cartes/Ressources/TIME_8051.h"
#include "../../Communication/UART/UART1.h"

#include <math.h>  // pour atan2()
#include <stdio.h>
#include <string.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/**
 * Paramètres de la Base:
 */

char BASE_speed = 20;  // %
int BASE_x = 0, BASE_y = 0, BASE_angle = 0;
float BASE_rayon_entre_roues = 0.95;  // dm

// Déplacement évolué:
bit BASE_deplacement_inProgress = 0;
unsigned int BASE_deplacement_timeout_ms = 0;
bit BASE_send2SRLZR = 1;
unsigned char BASE_deplacement_step = 0;
char BASE_deplacement_x, BASE_deplacement_y;
int BASE_deplacement_a;
float BASE_deplacement_theta;

/**
 * Fonction déclenchée toutes les ms pour mettre à jour
 * le déplacement de la base
 * @return {bit} 0: rien, 1: base arrivée à destination
 */
bit BASE_update() {
	bit res = 0;
  if (BASE_deplacement_inProgress == 1) {
    // TimeOut pour continuer le déplacement, même sans réponse du SRLZR:
    BASE_deplacement_timeout_ms++;

    // Attend au minimum 1s avant de passer à l'étape suivante:
    if (BASE_deplacement_timeout_ms < 1000) return 0;

    // Envoie la commande 'pids' au Serializer, pour connaître l'état du PID:
    if (BASE_send2SRLZR == 1) {
      UART1_resetResponse();
      SRLZR_PIDstate();
      BASE_send2SRLZR = 0;
    }

    // Attend la fin du déplacement:
    else if (UART1_hasResponse() == 1) {
      char response[10];
      strcpy(response, UART1_getResponse());

      if (response[0] == '0') {
        res = BASE_deplacement_next_step();
        BASE_deplacement_timeout_ms = 0;
      }

      BASE_send2SRLZR = 1;
    }

    // TimeOut: passe automatiquement à l'étape suivante après 2s
    else if (BASE_deplacement_timeout_ms > 4000) {
      res = BASE_deplacement_next_step();
      BASE_deplacement_timeout_ms = 0;
      BASE_send2SRLZR = 1;
    }
  }
	
	return res;
}

/**
 * Etapes pour réaliser le déplacement évolué
 * @return {bit} 0: rien, 1: base arrivée à destination
 */
bit BASE_deplacement_next_step() {
  BASE_deplacement_step++;

  // Rotation:
  if (BASE_deplacement_step == 1) {
    // Angle en direction de la position demandée:
    BASE_deplacement_theta = atan2(BASE_deplacement_x, BASE_deplacement_y) * 180.0f / M_PI;

    // Tourne la base en direction de la position demandée:
    if (BASE_deplacement_theta > 0)
      BASE_rotate('G', (int)BASE_deplacement_theta);
    else
      BASE_rotate('D', (int)-BASE_deplacement_theta);
  }

  // Déplace la base:
  else if (BASE_deplacement_step == 2) {
    float d, v;
    d = sqrt(BASE_deplacement_x * BASE_deplacement_x +
             BASE_deplacement_y * BASE_deplacement_y);
    v = 30;  // TODO: définir une vitesse optimale
    SRLZR_digo(d, v, d, v);

    // Enregistrement de la nouvelle position:
    BASE_x += BASE_deplacement_x;
    BASE_y += BASE_deplacement_y;
  }

  // Rotation:
  else if (BASE_deplacement_step == 3) {
    // Correction de la rotation pour s'orienter selon l'angle finale:
    float new_theta = BASE_deplacement_a - BASE_deplacement_theta;

    // Tourne la base à la position de fin:
    if (new_theta > 0)
      BASE_rotate('G', (int)new_theta);
    else
      BASE_rotate('D', (int)-new_theta);

    // Enregistrement de la nouvelle position:
    BASE_angle += BASE_deplacement_a;
  }

  // Fin du déplacement:
  else if (BASE_deplacement_step == 4) {
    BASE_deplacement_inProgress = 0;
		return 1;
  }
	
	return 0;
}

/**  "IPO X:valeur_x Y:valeur_y A:angle"
 * Initialisation de la position enregistrée sur la base
 * @param {int} x : -99 - 99 dm, position selon l'axe x
 * @param {int} y : -99 - 99 dm, position selon l'axe y
 * @param {int} a : -180 - 180°, rotation selon l'axe x
 * @return {bit} 0: ok, 1: error, pour savoir si la fonction s'est bien exécutée
 */
bit BASE_setPosition(int x, int y, int a) {
  // Vérifications:
  if (x < -99 || x > 99) return 1;
  if (y < -99 || y > 99) return 1;
  if (a < -180 || a > 180) return 1;

  // Nouvelle position:
  BASE_x = x;
  BASE_y = y;
  BASE_angle = a;

  return 0;
}

/**  "POS"
 * Retourne la position de la base relative dans la zone d'évolution
 * @return {char*} : "VPO X:valeur_x Y:valeur_y A:angle"
 */
char* BASE_getPosition() {
  char string[25];
  sprintf(string, "VPO X:%d Y:%d Z:%d", BASE_x, BASE_y, BASE_angle);
  return string;
}

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
  if (v < 5 || v > 100) return 1;
  return SRLZR_pwm(v, v);
}

/**  "B [vitesse]"
 * Fait reculer la base avec la vitesse définit en paramètre ou par
 * BASE_setSpeed()
 * @param  {char} v : 5-100 % = vitesse désirée ; 0 = vitesse de BASE_setSpeed()
 * @return {bit} 0: ok, 1: error
 */
bit BASE_backward_v(char v) {
  // Si vitesse nulle:
  if (v == 0) return SRLZR_pwm(-BASE_speed, -BASE_speed);

  // Vérification de la vitesse: 5 <= v <= 100
  if (v < 5 || v > 100) return 1;
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
 * @param {unsigned int} valeur : 0-180 ou 360, angle de rotation à réaliser
 * @return {bit} 0: ok, 1: error
 */
bit BASE_rotate(char sens, unsigned int valeur) {
  float theta, d;
  int v;
  // Vérification du sens:
  if (sens != 'G' && sens != 'D') return 1;

  // Vérification de l'angle:
  //if (valeur > 180 && valeur != 360) return 1;

  // Calcul de la distance à parcourir par une roue:
  theta = (valeur / 180.0f) * M_PI;
  d = theta * BASE_rayon_entre_roues;

  // Vitesse de rotation:
  v = 30;  // TODO: définir une vitesse optimale

  if (sens == 'G')
    // Tourne à gauche:
    return SRLZR_digo(d, v, -d, v);
  else
    // Tourne à droite:
    return SRLZR_digo(-d, v, d, v);
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
bit BASE_fullRotation(char sens) { return BASE_rotate(sens, 170); }

/**  "G X:valeur_x Y:valeur_y A:angle"
 * Déplace la base à des coordonnées relatives
 * Un message est envoyé lorsque la position est atteinte
 * @param {char} x : -99 - 99 dm, déplacement selon l'axe x
 * @param {char} y : -99 - 99 dm, déplacement selon l'axe y
 * @param {int}  a : -180 - 180°, position angulaire à la fin du déplacement
 * @return {bit} 0: ok, 1: error
 */
bit BASE_moveTo(char x, char y, int a) {
  // Vérifications:
  if (x < -99 || x > 99) return 1;
  if (y < -99 || y > 99) return 1;
  if (a < -180 || a > 180) return 1;

  // Enregistrement du déplacement:
  BASE_deplacement_x = -x;
  BASE_deplacement_y = y;
  BASE_deplacement_a = a;

  // Pour commencer le déplacement:
  BASE_send2SRLZR = 0;
  BASE_deplacement_step = 0;
  BASE_deplacement_timeout_ms = 0;
  BASE_deplacement_inProgress = 1;
  BASE_deplacement_next_step();

  return 0;
}