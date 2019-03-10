#ifndef BASE_H
#define BASE_H

/**
 * Communication avec le Serializer 
 */

/**
 * Envoie une commande et attend une réponse du Serializer:
 * @return {char*} : réponse de la commande
 */
char* SRLZR_sendCommandAndWait(char*);

/**
 * Envoie une commande au Serializer:
 * @param  {char*} command : commande à envoyer au Serializer
 * @return {char} : 'y' ou 'n', pour savoir si la fonction s'est bien exécutée
 */
char SRLZR_sendCommand(char* command);

/**
 * Attend une réponse du Serializer
 * Utilise un TimeOut pour ne pas attendre inutilement indéfiniment
 * @return {char*} : réponse de la commande
 */
char* SRLZR_waitResponse();


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
bit BASE_forward(char v);

/**  "B [vitesse]"
 * Fait reculer la base avec la vitesse définit en paramètre ou par
 * BASE_setSpeed()
 * @param  {char} v : 5-100 % = vitesse désirée ; 0 = vitesse de BASE_setSpeed()
 * @return {bit} : 0 (ok) ou 1 (erreur), pour savoir si la fonction s'est bien exécutée
 */
bit BASE_backard(char v);

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
 * @param {int} valeur : 0-180, angle de rotation à réaliser
 * @return {bit} : 0 (ok) ou 1 (erreur), pour savoir si la fonction s'est bien exécutée
 */
bit BASE_rotate(char sens, int valeur);

/**  "G X:valeur_x Y :valeur_y A:angle"
 * Déplace la base à des coordonnées relatives
 * Un message est envoyé lorsque la position est atteinte
 * @param {char} x : -99 - 99 dm, déplacement selon l'axe x
 * @param {char} y : -99 - 99 dm, déplacement selon l'axe y
 * @param {int}  a : -180 - 180°, position angulaire à la fin du déplacement
 * @return {bit} : 0 (ok) ou 1 (erreur), pour savoir si la fonction s'est bien exécutée
 */
bit BASE_moveTo(char x, char y, int a);

/**  "IPO X:valeur_x Y:valeur_y A:angle"
 * Initialise la position de la base dans la zone d'exploration
 * @param {char} x : -99 - 99 dm, position selon l'axe x
 * @param {char} y : -99 - 99 dm, position selon l'axe y
 * @param {int}  a : -180 - 180°, rotation selon l'axe x
 * @return {bit} : 0 (ok) ou 1 (erreur), pour savoir si la fonction s'est bien exécutée
 */
bit BASE_initPos(int x, int y, int a);

/**  "POS"
 * Retourne la position de la base relative dans la zone d'évolution
 * @return {char*} : "VPO X:valeur_x Y:valeur_y A:angle"
 */
char* BASE_getPos();

#endif  // BASE_H
