#ifndef BASE_H
#define BASE_H

/**
 * Déplacement 
 */

/**
 * Fonction déclenchée toutes les ms pour mettre à jour le déplacement de la base
 */
void BASE_update();

/**
 * Etapes pour réaliser le déplacement évolué
 */
void BASE_deplacement_next_step();

/**  "IPO X:valeur_x Y:valeur_y A:angle"
 * Initialisation de la position enregistrée sur la base
 * @param {int} x : -99 - 99 dm, position selon l'axe x
 * @param {int} y : -99 - 99 dm, position selon l'axe y
 * @param {int}  a : -180 - 180°, rotation selon l'axe x
 * @return {bit} 0: ok, 1: error, pour savoir si la fonction s'est bien exécutée
 */
bit BASE_setPosition(int x, int y, int a);

/**  "POS"
 * Retourne la position de la base relative dans la zone d'évolution
 * @return {char*} : "VPO X:valeur_x Y:valeur_y A:angle"
 */
char* BASE_getPosition();

/**  "TV vitesse"
 * Définit la vitesse de déplacement de la base
 * @param  {char} v : 5-100 %, vitesse désirée
 * @return {bit} : 0 (ok) ou 1 (erreur), pour savoir si la fonction s'est bien exécutée
 */
bit BASE_setSpeed(char v);

/**  "A [vitesse]"
 * Fait reculer la base avec la vitesse définit en paramètre ou par
 * BASE_setSpeed()
 * @param  {char} v : 5-100 %, vitesse désirée ; 0 = vitesse de BASE_setSpeed()
 * @return {bit} : 0 (ok) ou 1 (erreur), pour savoir si la fonction s'est bien exécutée
 */
bit BASE_forward_v(char v);

/**  "B [vitesse]"
 * Fait reculer la base avec la vitesse définit en paramètre ou par
 * BASE_setSpeed()
 * @param  {char} v : 5-100 % = vitesse désirée ; 0 = vitesse de BASE_setSpeed()
 * @return {bit} : 0 (ok) ou 1 (erreur), pour savoir si la fonction s'est bien exécutée
 */
bit BASE_backward_v(char v);

/**  "S"
 * Arrête les moteurs, pour les commandes BASE_forward() et BASE_backward()
 * @return {bit} : 0 (ok) ou 1 (erreur), pour savoir si la fonction s'est bien exécutée
 */
bit BASE_stop();

/**  "RG"
 * Effectue une rotation 'optimisée' de 90° à gauche
 * @return {bit} : 0 (ok) ou 1 (erreur), pour savoir si la fonction s'est bien exécutée
 */
bit BASE_rotate90Left();

/**  "RD"
 * Effectue une rotation 'optimisée' de 90° à droite
 * @return {bit} : 0 (ok) ou 1 (erreur), pour savoir si la fonction s'est bien exécutée
 */
bit BASE_rotate90Right();

/**  "RC [D/G]"
 * Effectue une rotation 'optimisée' complète à droite ou à gauche
 * @param {char} sens : 'G' ou 'D', sens de la rotation (par défaut droite)
 * @return {bit} : 0 (ok) ou 1 (erreur), pour savoir si la fonction s'est bien exécutée
 */
bit BASE_fullRotation(char sens);

/**  "RA sens:valeur"
 * Effectue une rotation 'optimisée' dans un sens et un angle précisés
 * @param {char} sens : 'G' ou 'D', sens de la rotation
 * @param {unsigned int} valeur : 0-180 ou 360, angle de rotation à réaliser
 * @return {bit} : 0 (ok) ou 1 (erreur), pour savoir si la fonction s'est bien exécutée
 */
bit BASE_rotate(char sens, unsigned int valeur);

/**  "G X:valeur_x Y :valeur_y A:angle"
 * Déplace la base à des coordonnées relatives
 * Un message est envoyé lorsque la position est atteinte
 * @param {char} x : -99 - 99 dm, déplacement selon l'axe x
 * @param {char} y : -99 - 99 dm, déplacement selon l'axe y
 * @param {int}  a : -180 - 180°, position angulaire à la fin du déplacement
 * @return {bit} : 0 (ok) ou 1 (erreur), pour savoir si la fonction s'est bien exécutée
 */
bit BASE_moveTo(char x, char y, int a);

#endif  // BASE_H
