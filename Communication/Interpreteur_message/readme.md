# Interpréteur de message

## Objectif

- interpréter un message provenant de la communication UART ou SPI, et extraire la commande et les paramètres à exécuter


## Idée de la forme d'un message interprété

```c
struct Commande {
  char* code,
  int nbParams,
  char*[] params,
  char*[] valeurs,
}
```