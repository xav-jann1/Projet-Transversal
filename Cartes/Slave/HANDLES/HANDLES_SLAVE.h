#ifndef HANDLES_H
#define HANDLES_H

#include "../../Communication/Parseur_messages/PARSEUR.h"

#include "./Headers/HANDLE_Auxiliaires.h"
#include "./Headers/HANDLE_Fonctionnement.h"
#include "./Headers/HANDLE_Pointeur_lumineux.h"
#include "./Headers/HANDLE_Prise_de_vue.h"
#include "./Headers/HANDLE_Signaux_sonores.h"

// Pointeurs de fonctions:
char* (*HANDLES[5])(Commande*) = {
    HANDLE_Pointeur_lumineux, HANDLE_Signaux_sonores,
    HANDLE_Prise_de_vue, HANDLE_Fonctionnement, HANDLE_Auxiliaires};

#endif