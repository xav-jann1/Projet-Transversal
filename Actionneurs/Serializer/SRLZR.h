#ifndef SRLZR_H
#define SRLZR_H

/***** Communication *****/

/**
 * Envoie une commande et attend une réponse du Serializer:
 * @return {char*} : réponse de la commande
 */
char* SRLZR_sendCommandAndWait(char* command);

/**
 * Envoie une commande au Serializer:
 * @param  {char*} command : commande à envoyer au Serializer
 * @return {bit} 0: ok, 1: error, pour savoir si la fonction s'est bien exécutée
 */
bit SRLZR_sendCommand(char* command);

/**
 * Attend une réponse du Serializer
 * Utilise un TimeOut pour ne pas attendre inutilement indéfiniment
 * @return {char*} : réponse de la commande
 */
char* SRLZR_waitResponse();

/***** Configuration *****/

/**
 * Initialisation de la communication avec le Serializer
 * Action: active et configure l'UART1
 * @return {bit} 0: ok, 1: error, pour savoir si la fonction s'est bien exécutée
 */
bit SRLZR_init();

/**  "fw"
 * Demande la version du firmware au Serializer
 * @return {char*} : version du firmware
 */
// char* SRLZR_firmware();
char SRLZR_firmware();

/**  "reset"
 * Redémarre la carte Serializer
 * @return {bit} 0: ok, 1: error, pour savoir si la fonction s'est bien exécutée
 */
bit SRLZR_reset();

/**  "restore"
 * Initialise et redémarre la carte aux paramètres par défaut d'usine
 * @return {bit} 0: ok, 1: error, pour savoir si la fonction s'est bien exécutée
 */
bit SRLZR_restore();

/**** Config ****/

/**  "cfg enc encoderType"
 * Configure le type des encodeurs (simple ou quadrature)
 * @param  {char} encoderType : 0,1 - (0:simple ou 1:quadraure)
 * @return {bit} 0: ok, 1: error, pour savoir si la fonction s'est bien exécutée
 */
bit SRLZR_setEncoderType(char encoderType);

/**  "cfg baud baudRate"
 * Modication de la vitesse de communication
 * @param  {int} baud : 0-5, 2400, 4800, 9600, 19200, 57600, 115200
 * @return {bit} 0: ok, 1: error, pour savoir si la fonction s'est bien exécutée
 */
bit SRLZR_setBaud(int baud);

/**  "cfg units unit_type"
 * Configure l'unité pour la lecture des capteurs
 * @param  {char} units : 0,1,2 (0:metric, 1:imperial, 2:raw)
 * @return {bit} 0: ok, 1: error, pour savoir si la fonction s'est bien exécutée
 */
bit SRLZR_setUnits(int units);

/**  "cfg enc"
 * Renvoie la configuration des encodeurs
 * @return {char} : '0','1' - (0:simple ou 1:quadrature)
 */
bit SRLZR_getEncoderType();

/**  "cfg baud"
 * Renvoie la vitesse de communication utilisée (baud rate)
 * @return {int} : baud_rate
 */
bit SRLZR_getBaud();

/**  "cfg units"
 * Renvoie l'unité utilisée pour les capteurs
 * @return {char} : 0,1,2 - (0:metric, 1:imperial, 2:raw)
 */
bit SRLZR_getUnits();

/**** PID ****/

/**  "rpid s"
 * Initialise le PID pour la base utilisée (Stinger)
 * @return {bit} 0: ok, 1: error, pour savoir si la fonction s'est bien exécutée
 */
bit SRLZR_resetPID();

// VPID:
/**  "vpid prop:integ:deriv:loop"
 * Configure le VPID du Serializer
 * @param  {int} p    : paramètre P du PIDL (Proportional)
 * @param  {int} i    : paramètre I du PIDL (Integral)
 * @param  {int} d    : paramètre D du PIDL (Derivative)
 * @param  {int} loop : paramètre L du PIDL (Loop)
 * @return {bit} 0: ok, 1: error, pour savoir si la fonction s'est bien exécutée
 */
bit SRLZR_setVPID(int p, int i, int d, int loop);

/**  "vpid"
 * Renvoie la configuration du VPID
 * @return {char*} : "P:p I:i D:d L:loop"
 */
char* SRLZR_getVPID();

// DPID:
/**  "dpid prop:integ:deriv:accel"
 * Configure le VPID du Serializer
 * @param  {int} p : paramètre P du PIDA (Proportional)
 * @param  {int} i : paramètre I du PIDA (Integral)
 * @param  {int} d : paramètre D du PIDA (Derivative)
 * @param  {int} a : paramètre A du PIDA (Acceleration)
 * @return {bit} 0: ok, 1: error, pour savoir si la fonction s'est bien exécutée
 */
bit SRLZR_setDPID(int p, int i, int d, int a);

/**  "dpid"
 * Renvoie la configuration du DPID
 * @return {char*} : "P:p I:i D:d A:a"
 */
char* SRLZR_getDPID();

/***** Utilisation *****/

/**  "digo 1:distance:vel 2:distance:vel"
 * Tourne les moteurs pour avancer à une certaine distance et vitesse
 * @param  {int} d1 : distance en dm pour le moteur 1 (à convertir en ticks)
 * @param  {int} v1 : 0-v_max, vitesse du moteur 1 (!: v_max dépend du VPID)
 * @param  {int} d2 : distance en dm pour le moteur 2 (à convertir en ticks)
 * @param  {int} v2 : 0-v_max, vitesse du moteur 2 (!: v_max dépend du VPID)
 * @return {bit} 0: ok, 1: error, pour savoir si la fonction s'est bien exécutée
 */
bit SRLZR_digo(float d1, int v1, float d2, int v2);

/**
 * Tourne les moteurs à une certaine vitesse en lien avec le PID:
 * @param  {int} v1 : -100 - 100, pwm du moteur 1
 * @param  {int} v2 : -100 - 100, pwm du moteur 2
 * @return {bit} 0: ok, 1: error, pour savoir si la fonction s'est bien exécutée
 */
bit SRLZR_mogo(int v1, int v2);

/**  "pwm 1:pwm 2:pwm"
 * Alimente directement les moteurs par PWM
 * @param  {int} v1 : -100 - 100, pwm du moteur 1
 * @param  {int} v2 : -100 - 100, pwm du moteur 2
 * @return {bit} 0: ok, 1: error, pour savoir si la fonction s'est bien exécutée
 */
bit SRLZR_pwm(int v1, int v2);

/**  "pwm r:rate 1:pwm 2:pwm"
 * Alimente progressivement, par rampe, les moteurs par PWM
 * Loop de la rampe: 51ms => moteur de 0 à 100 en 2000ms -> r = 2000/51 = 39
 * @param  {int} r : 0 < r < v, évolution progressive du PWM (0: pas de rampe)
 * @param  {int} v1 : -100 - 100, pwm du moteur 1
 * @param  {int} v2 : -100 - 100, pwm du moteur 2
 * @return {bit} 0: ok, 1: error, pour savoir si la fonction s'est bien exécutée
 */
bit SRLZR_pwmWithRate(int r, int v1, int v2);

/**  "stop"
 * Arrête les moteurs
 * @return {bit} 0: ok, 1: error, pour savoir si la fonction s'est bien exécutée
 */
bit SRLZR_stop();

/**  "vel"
 * Renvoie la vitesse des moteurs (en lien avec le PIDL)
 * @return {char*} : "v1 v2"
 */
char* SRLZR_vel();

/**  "pids"
 * Renvoie l'état du pid lors de l'utilisation de la commande "digo"
 * Permet de savoir si la base à atteint la position demandée
 * @return {char} : '1' ou '0' (1: en fonctionnement, 0: à l'arrêt)
 */
bit SRLZR_PIDstate();

#endif