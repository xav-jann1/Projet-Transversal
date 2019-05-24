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
  - **Capteurs** : [Caméra](./Capteurs/Camera)

- [STM32](./Cartes/STM32) :
  - **Communications** : [???](./Communication/???), [Bluetooth](./Communication/Bluetooth), [SD](./Communication/SD)
  - **Actionneurs** : [Haut-parleur](./Actionneurs/Haut-parleur)
  - **Capteurs** : [Microphone](./Capteurs/Microphone)

- [FPGA](./Cartes/FPGA) :
  - **Communication** : [Radio](./Communication/Radio)
  - **Capteurs** : [Cible](./Capteurs/Cible)

---

## Actionneurs
- [Pointeur lumineux](./Actionneurs/Pointeur_lumineux) : contrôle du pointeur lumineux
- [Serializer](./Actionneurs/Serializer) : déplacement de la base
- [Servomoteur](./Actionneurs/Servomoteur) : contrôle de l'orientation d'un servomoteur

## Capteurs
- [Courant](./Capteurs/Courant) : mesure du courant par une résistance de Shunt
- [Infrarouge](./Capteurs/Infrarouge) : mesure d'une distance par télémètre infrarouge
- [Microphone](./Capteurs/Microphone) : acquisition d'un signal sonore
- [Photo](./Capteurs/Photo) : utilisation de la caméra pour les prises de vue
- [Ultrason](./Capteurs/Ultrason) : mesure d'une distance par un télémètre ultrason



## Communication
- [Parseur de messages](./Communication/Parseur_messages) : extrait le commande d'un message reçu par la liaison *UART* ou *SPI*
- [Bluetooth](./Communication/Bluetooth) : communication *Bluetooth* entre la **STM32** et la **Centrale de Commande**
- [Radio](./Communication/Radio) : communication *Radio* entre la **Carte Slave** et la **Cible**
- [SD](./Communication/SD) : sauvegarde de données sur une **carte SD**
- [SPI](./Communication/SPI) : communication *SPI* entre la **Carte Master** et la **Carte Slave**
- [UART](./Communication/UART) : communication *UART* entre la **Carte Master** et la **Centrale de Commande**


## PCs
- [Centrale de commande](./PCs/Centrale_de_commande) : communique avec tous les autres PCs et la carte Master pour donner des ordres
- [Calculs](./PCs/Calculs) : calcul la position des servos pour viser la cible, le chemin que doit parcourir le robot pour éviter les obstacles

