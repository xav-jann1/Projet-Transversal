# Projet Transversal

Rassemblement de tous les programmes développés pour respecter le cahier des charges.


## [Cartes](./Cartes)

- [Master](./Cartes/Master) : 8051
  - **Communications** : [UART](./Communication/UART), [SPI](./Communication/SPI)
  - **Actionneurs** : [Serializer](./Actionneurs/Serializer), [Servomoteur](./Actionneurs/Servomoteur)
  - **Capteurs** : [Courant](./Capteurs/Courant), Télémètres [Infrarouge](./Capteurs/Infrarouge) et [Ultrason](./Capteurs/Ultrason)

- [Slave](./Cartes/Slave) : 8051
  - **Communications** : [SPI](./Communication/SPI), [Radio](./Communication/Radio)
  - **Actionneurs** : [Serializer](./Actionneurs/Serializer), [Servomoteur](./Actionneurs/Servomoteur), [Pointeur lumineux](./Actionneurs/Pointeur_lumineux)
  - **Capteurs** : [Caméra](./Capteurs/Caméra)

- [STM32](./Cartes/STM32) :
  - **Communications** : [???](./Communication/???), [Bluetooth](./Communication/Bluetooth), [SD](./Communication/SD)
  - **Actionneurs** : [Haut-parleur](./Actionneurs/Haut-parleur)
  - **Capteurs** : [Microphone](./Capteurs/Microphone)

- [FPGA](./Cartes/FPGA) :
  - **Communication** : [Radio](./Communication/Radio)
  - **Capteurs** : [Cible](./Capteurs/Cible)

---

## Actionneurs
- [Pointeur lumineux](./Pointeur_lumineux) : contrôle du pointeur lumineux
- [Serializer](./Serializer) : déplacement de la base
- [Servomoteur](./Servomoteur) : contrôle de l'orientation d'un servomoteur

## Capteurs
- [Caméra](./Caméra) : utilisation de la caméra pour les prises de vue
- [Courant](./Courant) : mesure du courant par une résistance de Shunt
- [Infrarouge](./Infrarouge) : mesure d'une distance par télémètre infrarouge
- [Microphone](./Microphone) : acquisition d'un signal sonore
- [Ultrason](./Ultrason) : mesure d'une distance par un télémètre ultrason



## Communication
- [Interpréteur de message](./Interpréteur_message) : interprète un message reçu par la liaison *UART* ou *SPI* pour en extraire la **commande** ou l'**information**
- [Bluetooth](./Bluetooth) : communication *Bluetooth* entre la **STM32** et la **Centrale de Commande**
- [Radio](./Radio) : communication *Radio* entre la **Carte Slave** et la **Cible**
- [SD](./SD) : sauvegarde de données sur une **carte SD**
- [SPI](./SPI) : communication *SPI* entre la **Carte Master** et la **Carte Slave**
- [UART](./UART) : communication *UART* entre la **Carte Master** et la **Centrale de Commande**


## PCs
- [Centrale de commande](./Centrale_de_commande) : communique avec tous les autres PCs et la carte Master pour donner des ordres
- [Calcul parcours](./Calcul_parcours) : calcul le chemin que doit parcourir le robot pour éviter les obstacles 


