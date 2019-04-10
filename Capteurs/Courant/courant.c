
#include "c8051F020.h"
#include "courant.h"
#include "../Ressources/CONFIG_8051.h
#include <stdio.h>

// CAN
sfr16 ADC0 = 0xBE;
float COURANT_TOT;
float V_MES_TABL[10];
float V;
char message_courant[50];
int i;

/**
 * Initialisation des registres pour permettre la lecture du courant consomm�
 * Actions:
 *  - mise en place de l'ADC
 *  - initialisation des variables : �nergie_consomm�e
 */
void COURANT_init(){
	
	CONFIG_init();
	
	REF0CN = 0x03;
  AMX0CF = 0x0;
  AMX0SL = 0x0;  // AIN0 par defaut
  AD0CM0 = 0;    // conversion pour AD0BUSY = 1
  AD0CM1 = 0;
  AD0TM = 0;
  AD0LJST = 0;  // alignement a droite
  AD0EN = 1;    // activation du  CAN
	
	COURANT_reset();
}

/**
 * Mets � z�ro les valeurs d'�nergie consomm�e
 */
void COURANT_reset(){
	
	COURANT_TOT = 0;
	
}

/**
 * Effectue une mesure instantan�e du courant consomm� par la base
 * @return {float} : courant_mesur�
 */
float COURANT_mesure(){
	for(i = 0; i < 10; i++){
		
	
		AMX0SL = 0x0;  // AIN0
		AD0INT = 0;
		AD0BUSY = 1;
		while (AD0INT != 1) ;
		
		// Conversion en tension
		V = ((ADC0 * 2.43f) / 4095.0f);
		
		V_MES_TABL[i] = V;
	
	}
	
	// Moyenne sur 10 valeurs
	V = 0;
	for(i = 0; i < 10; i++){
		V += V_MES_TABL[i];
	}
	V /= 10;
		
	return V / 0.05f;
}

/**
 * Met � jour le courant consomm� depuis le d�but de l'�preuve
 */
void COURANT_update(){
	
	COURANT_TOT += COURANT_mesure();	
}

/**
 * Renvoie l'�nergie consomm�e depuis le d�but de l'�preuve
 * @param {int} : �nergie_consomm�e
 */
float COURANT_getEnergie(){
	
	return COURANT_TOT*0.001f; 
}

/**  "MI"
 * Effectue une mesure instantan�e du courant consomm� par la base
 * Et renvoie la valeur obtenue
 * @return {char*} : "KI courant_mesur�"
 */
void COURANT_MI(){

	sprintf(message_courant, "%f KI", COURANT_mesure());	
}

/**  "ME"
 * Renvoie un message contenant l'�nergie consomm�e depuis le d�but de l'�preuve
 * @return {char*} : "KE �nergie_consomm�e"
 */
void COURANT_ME(){
	
	sprintf(message_courant, "%f KE", COURANT_getEnergie());
}


void main(){
	
	COURANT_init();
	COURANT_mesure();
	COURANT_ME();
	
	while(1);
}
	

