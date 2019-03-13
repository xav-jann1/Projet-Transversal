#include "SRLZR.h"
#include <stdio.h>
#include <string.h>
#include "../../Communication/UART/UART1.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/* TODO
- Intégrer waitResponse()
- Ajouter un timeOut sur waitResponse() pour éviter d'attendre inutilement
  indéfiniment
*/

/**
 * Paramètre du Serializer (et de la Base):
 */

float BASE_diametre_roue = 6.0;  // dm
float BASE_ticks_par_tour = 624.0;
float SRLZR_ticks_par_dm;
char* response;

/*** Communication ***/

// Envoie une commande et attend la réponse du Serializer:
char* SRLZR_sendCommandAndWait(char* command) {
  SRLZR_sendCommand(command);
  return SRLZR_waitResponse();
}

/**
 * Envoie une commande au Serializer:
 * @param  {char*} command : commande à envoyer au Serializer
 * @return {bit} 0: ok, 1: error, pour savoir si la fonction s'est bien exécutée
 */
bit SRLZR_sendCommand(char* c) {
  // Ajoute le caractère de fin:
  char command[30 + 1];
  sprintf(command, "%s\r", c);

  UART1_send(command);

  // Pour tests:
  // puts(command);
  // response = UART0_send(command);
  // printf("%s", response);

  // Si commande incomprise:
  // if (strcmp(response, "NACK\r\n>") == 0) return 1;

  return 0;
}

// Attend une réponse du Serializer:
char* SRLZR_waitResponse() {
  // todo: ajouter TimeOut
  /*
  //extern char UART1_bufferIncomplete;
  int timeOut = 1000;
  while(UART1_bufferIncomplete == 1 && timeOut > 0) {
    timeOut--;
  }

  if(timeOut <= 0) return "nope";
  return UART1_string_buffer;
  */
  return "OK";
}

void SRLZR_update() {
  // SRLZR_
}

/*
 * Configuration
 */

/**
 * Initialisation de la communication avec le Serializer
 * Action: active et configure l'UART1
 * @return {bit} 0: ok, 1: error, pour savoir si la fonction s'est bien exécutée
 */
bit SRLZR_init() {
  // SRLZR_stop();
  // SRLZR_resetPID();

  // Calcul du nombre de ticks des moteurs pour parcourir 1 dm:
  SRLZR_ticks_par_dm = BASE_ticks_par_tour / (BASE_diametre_roue * M_PI);
  return 0;
  // return SRLZR_stop() || SRLZR_resetPID();
}

/**  "fw"
 * Demande la version du firmware au Serializer
 * @return {char*} : version du firmware
 */
// char* SRLZR_firmware() {
char SRLZR_firmware() {
  return SRLZR_sendCommand("fw");
  // return SRLZR_sendCommandAndWait("fw");
}

/**  "reset"
 * Redémarre la carte Serializer
 * @return {bit} 0: ok, 1: error, pour savoir si la fonction s'est bien exécutée
 */
bit SRLZR_reset() { return SRLZR_sendCommand("reset"); }

/**  "restore"
 * Initialise et redémarre la carte aux paramètres par défaut d'usine
 * @return {bit} 0: ok, 1: error, pour savoir si la fonction s'est bien exécutée
 */
bit SRLZR_restore() { return SRLZR_sendCommand("restore"); }

/** Config **/

/**  "cfg enc encoderType"
 * Configure le type des encodeurs (simple ou quadrature)
 * @param  {char} encoderType : 0,1 - (0:simple ou 1:quadraure)
 * @return {bit} 0: ok, 1: error, pour savoir si la fonction s'est bien exécutée
 */
bit SRLZR_setEncoderType(char encoderType) {
  char command[20];

  // Si encoderType != 0,1:
  if (encoderType != 0 && encoderType != 1) return 1;

  sprintf(command, "cfg enc %d", encoderType);

  return SRLZR_sendCommand(command);
}

/**  "cfg baud baudRate"
 * Modication de la vitesse de communication
 * @param  {int} baud : 0-5, 2400, 4800, 9600, 19200, 57600, 115200
 * @return {bit} 0: ok, 1: error, pour savoir si la fonction s'est bien exécutée
 */
bit SRLZR_setBaud(int baud) {
  // TODO:
  // Problème pour 115200: int sur 2 octets => 2^16=65536 < 115200
  // Solution : transformer int en char* -> utiliser des 'strcmp()'

  // Vérification de la valeur de baud:
  if ((baud >= 0 && baud <= 5) || baud == 2400 || baud == 4800 ||
      baud == 9600 || baud == 19200 || baud == 57600 || baud == 115200) {
    char command[20];
    sprintf(command, "cfg baud %d", baud);
    return SRLZR_sendCommand(command);
  } else
    return 0;
}

/**  "cfg units unit_type"
 * Configure l'unité pour la lecture des capteurs
 * @param  {char} units : 0,1,2 (0:metric, 1:imperial, 2:raw)
 * @return {bit} 0: ok, 1: error, pour savoir si la fonction s'est bien exécutée
 */
bit SRLZR_setUnits(int units) {
  char command[20];

  // Si units != 0,1,2:
  if (units != 0 && units != 1 && units != 2) return 1;

  sprintf(command, "cfg units %d", units);
  return SRLZR_sendCommand(command);
}

/**  "cfg enc"
 * Renvoie la configuration des encodeurs
 * @return {char} : '0','1' - (0:simple ou 1:quadrature)
 */
bit SRLZR_getEncoderType() { return SRLZR_sendCommand("cfg enc"); }

/**  "cfg baud"
 * Renvoie la vitesse de communication utilisée (baud rate)
 * @return {int} : baud_rate
 */
bit SRLZR_getBaud() { return SRLZR_sendCommand("cfg baud"); }

/**  "cfg units"
 * Renvoie l'unité utilisée pour les capteurs
 * @return {char} : 0,1,2 - (0:metric, 1:imperial, 2:raw)
 */
bit SRLZR_getUnits() { return SRLZR_sendCommand("cfg units"); }

/** PID **/

// VPID:
/**  "vpid prop:integ:deriv:loop"
 * Configure le VPID du Serializer
 * @param  {int} p    : paramètre P du PIDL (Proportional)
 * @param  {int} i    : paramètre I du PIDL (Integral)
 * @param  {int} d    : paramètre D du PIDL (Derivative)
 * @param  {int} loop : paramètre L du PIDL (Loop)
 * @return {bit} 0: ok, 1: error, pour savoir si la fonction s'est bien exécutée
 */
bit SRLZR_setVPID(int p, int i, int d, int loop) {
  char command[30];
  sprintf(command, "vpid %d:%d:%d:%d", p, i, d, loop);
  return SRLZR_sendCommand(command);
}

/**  "vpid"
 * Renvoie la configuration du VPID
 * @return {char*} : "P:p I:i D:d L:loop"
 */
char* SRLZR_getVPID() {
  char* response;

  SRLZR_sendCommand("vpid");
  response = SRLZR_waitResponse();
  return response;
}

// DPID:
/**  "dpid prop:integ:deriv:accel"
 * Configure le VPID du Serializer
 * @param  {int} p : paramètre P du PIDA (Proportional)
 * @param  {int} i : paramètre I du PIDA (Integral)
 * @param  {int} d : paramètre D du PIDA (Derivative)
 * @param  {int} a : paramètre A du PIDA (Acceleration)
 * @return {bit} 0: ok, 1: error, pour savoir si la fonction s'est bien exécutée
 */
bit SRLZR_setDPID(int p, int i, int d, int a) {
  char command[30];
  sprintf(command, "dpid %d:%d:%d:%d", p, i, d, a);
  return SRLZR_sendCommand(command);
}

/**  "dpid"
 * Renvoie la configuration du DPID
 * @return {char*} : "P:p I:i D:d A:a"
 */
char* SRLZR_getDPID() {
  SRLZR_sendCommand("dpid");
  return SRLZR_waitResponse();
}

/**  "rpid s"
 * Initialise le PID pour la base utilisée (Stinger)
 * @return {bit} 0: ok, 1: error, pour savoir si la fonction s'est bien exécutée
 */
bit SRLZR_resetPID() { return SRLZR_sendCommand("rpid s"); }

/**
 * Utilisation
 */

/**  "stop"
 * Arrête les moteurs
 * @return {bit} 0: ok, 1: error, pour savoir si la fonction s'est bien exécutée
 */
bit SRLZR_stop() { return SRLZR_sendCommand("stop"); }

/**  "digo 1:distance:vel 2:distance:vel"
 * Tourne les moteurs pour avancer à une certaine distance et vitesse
 * @param  {float} d1 : distance en dm pour le moteur 1 (à convertir en ticks)
 * @param  {int} v1 : 0-v_max, vitesse du moteur 1 (!: v_max dépend du VPID)
 * @param  {float} d2 : distance en dm pour le moteur 2 (à convertir en ticks)
 * @param  {int} v2 : 0-v_max, vitesse du moteur 2 (!: v_max dépend du VPID)
 * @return {bit} 0: ok, 1: error, pour savoir si la fonction s'est bien exécutée
 */
bit SRLZR_digo(float d1, int v1, float d2, int v2) {
  // Nombre de ticks de chaque moteur par parcourir la distance en dm:
  int t1 = d1 * SRLZR_ticks_par_dm;
  int t2 = d2 * SRLZR_ticks_par_dm;

  char command[30];
  sprintf(command, "digo 1:%d:%d 2:%d:%d", t1, v1, t2, v2);
  return SRLZR_sendCommand(command);
}

/**
 * Tourne les moteurs à une certaine vitesse en lien avec le PID:
 * @param  {int} v1 : -100 - 100, pwm du moteur 1
 * @param  {int} v2 : -100 - 100, pwm du moteur 2
 * @return {bit} 0: ok, 1: error, pour savoir si la fonction s'est bien exécutée
 */
bit SRLZR_mogo(int v1, int v2) {
  char command[30];
  sprintf(command, "mogo 1:%d 2:%d", v1, v2);
  return SRLZR_sendCommand(command);
}

/**  "pwm 1:pwm 2:pwm"
 * Alimente directement les moteurs par PWM
 * @param  {int} v1 : -100 - 100, pwm du moteur 1
 * @param  {int} v2 : -100 - 100, pwm du moteur 2
 * @return {bit} 0: ok, 1: error, pour savoir si la fonction s'est bien exécutée
 */
bit SRLZR_pwm(int v1, int v2) {
  char command[20];

  // Vérification des paramètre: -100 <= v <= 100
  if (v1 < -100 || v1 > 100 || v2 < -100 || v2 > 100) return 1;

  sprintf(command, "pwm 1:%d 2:%d", v1, v2);
  return SRLZR_sendCommand(command);
}

/**  "pwm r:rate 1:pwm 2:pwm"
 * Alimente progressivement, par rampe, les moteurs par PWM
 * Loop de la rampe: 51ms => moteur de 0 à 100 en 2000ms -> r = 2000/51 = 39
 * @param  {int} r : 0 < r < v, évolution progressive du PWM (0: pas de rampe)
 * @param  {int} v1 : -100 - 100, pwm du moteur 1
 * @param  {int} v2 : -100 - 100, pwm du moteur 2
 * @return {bit} 0: ok, 1: error, pour savoir si la fonction s'est bien exécutée
 */
bit SRLZR_pwmWithRate(int r, int v1, int v2) {
  char command[30];

  // Vérification des paramètre: -100 <= v <= 100
  if (r < -100 || r > 100 || v1 < -100 || v1 > 100 || v2 < -100 || v2 > 100)
    return 1;

  sprintf(command, "pwm r:%d 1:%d 2:%d", r, v1, v2);
  return SRLZR_sendCommand(command);
}

/**  "vel"
 * Renvoie la vitesse des moteurs (en lien avec le PIDL)
 * @return {char*} : "v1 v2"
 */
bit SRLZR_getVel() {
  return SRLZR_sendCommand("vel");
  // return SRLZR_waitResponse();
}

/**  "pids"
 * Renvoie l'état du pid lors de l'utilisation de la commande "digo"
 * Permet de savoir si la base à atteint la position demandée
 * @return {char} : '1' ou '0' (1: en fonctionnement, 0: à l'arrêt)
 */
bit SRLZR_PIDstate() {
  return SRLZR_sendCommand("pids");
  /*
  char* response = SRLZR_sendCommand_waitResponse("pids");

  // Si la commande n'a pas correctement été exécutée:
  if (strcmp(response, "NACK\r\n>") == 0) return 1;

  return response[0];
  */
}