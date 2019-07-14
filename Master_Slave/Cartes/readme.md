# Cartes

Programmes utilisés sur chaque carte du projet :
- [Master](./Master) : 8051
  - **Communications** : [UART](../Communication/UART), [SPI](../Communication/SPI)
  - **Actionneurs** : [Serializer](../Actionneurs/Serializer), [Servomoteur](../Actionneurs/Servomoteur)
  - **Capteurs** : [Courant](../Capteurs/Courant), Télémètres [Infrarouge](../Capteurs/Infrarouge) et [Ultrason](../Capteurs/Ultrason)

- [Slave](./Slave) : 8051
  - **Communications** : [SPI](../Communication/SPI), [Radio](../Communication/Radio)
  - **Actionneurs** : [Serializer](../Actionneurs/Serializer), [Servomoteur](../Actionneurs/Servomoteur), [Pointeur lumineux](../Actionneurs/Pointeur_lumineux)
  - **Capteurs** : [Photo](../Capteurs/Photo)


Le dossier [Ressources](./Ressources) contient des programmes pour configurer et gérer le temps sur `8051`.