#ifndef HANDLES_H
#define HANDLES_H

#include "../../Communication/Parseur_messages/PARSEUR.h"

#include "./Headers/HANDLE_Auxiliaires.h"
#include "./Headers/HANDLE_Courant.h"
#include "./Headers/HANDLE_Deplacement.h"
#include "./Headers/HANDLE_Detection.h"
#include "./Headers/HANDLE_Fonctionnement.h"
#include "./Headers/HANDLE_Pointeur_lumineux.h"
#include "./Headers/HANDLE_Prise_de_vue.h"
#include "./Headers/HANDLE_Signaux_sonores.h"

// Pointeurs de fonctions:
char* (*HANDLES[8])(Commande*) = {
    HANDLE_Fonctionnement, HANDLE_Deplacement,     HANDLE_Detection,
    HANDLE_Courant,        HANDLE_Signaux_sonores, HANDLE_Pointeur_lumineux,
    HANDLE_Prise_de_vue,   HANDLE_Auxiliaires};

#endif