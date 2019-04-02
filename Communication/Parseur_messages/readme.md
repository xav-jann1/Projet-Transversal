# Parseur de messages

Extrait la **commande** et les **paramètres** d'un [message]() reçu par la **Centrale de Commande** (ou autre).

**Exemples de message :**
- `RG`
- `TV 50`
- `RA D:160`
- `G X:-50 Y:60 A:90`
- `MOB D A:20`
- `SD F:50 P:20 W:10 B:30`

Le message est ainsi composé d'un **code**, puis parfois d'un ou plusieurs **paramètres**, assemblés dans certains cas avec des **valeurs** par le caractère `:`.


## Utilisation

Fonction qui décompose un message :
```c
Commande PARSEUR_message(char* message);
```

Cette fonction renvoie un `struct` avec les éléments suivants :
```c
struct Commande {
  char code[4];
  int nbParams,
  char params[4],
  int valeurs[4],
}
```
- `code` : code de la commande
- `nbParams` : nombre de paramètres dans la commande
- `params` : liste des paramètres
- `valeurs` : liste des valeurs des paramètres

*(Un même indice entre `params` et `valeurs` correspond au même mot)*

Si le **paramètre** ne possède pas de **valeur**, sa valeur dans la liste `valeurs` est `0xFF`.
Si le **paramètre** est un nombre, sa valeur est enregistrée dans la liste `valeurs`, et `'#'` dans la liste `params`.


## Fonctionnement

Pour correctement extraire la commande d'un message, celui-ci est d'abord décomposé en **mots** (*caractères* entourés par des espaces `' '`), qui sont stockés dans une liste.

Le premier mot correspond alors au **code** de la commande, et les autres aux **paramètres** et à leurs **valeurs**.

Le **code** et le **nombre de paramètres** sont ainsi ajoutés à la `commande`, puis les listes `params` et `valeurs` se remplissent des **paramètres** et des **valeurs** de chaque mot (en les séparant par le caractère `':'`).

Si le **paramètre** ne possède pas de **valeur**, sa valeur dans la liste `valeurs` est `0xFF`.
Si le **paramètre** est un nombre, sa valeur est enregistrée dans la liste `valeurs`, et `'#'` dans la liste `params`.


## Limites

L'utilisation de la fonction `PARSEUR_message()` possède plusieurs limites sur la forme du message.

Cependant, ces limites existent pour avoir une structure universelle afin de traiter efficacement toutes les commandes du cahier des charges.

Nombre **maximum** de caractères pour chaque élément du message : `code param1:val1 param2:val2 param3`

Elément | Maximum | Exemple
-|-|-
code      |  3  |  `MOS`
paramètre |  1  |  `P`
valeur    |  4  |  `-100`

Si le paramètre est seul et est un nombre, sa valeur est enregistrée dans la liste `valeurs`.

Ainsi, pour que le **code** soit enregistré dans une variable, il faut aussi ajouter le caractère de fin `\0`.

Par conséquent, un **mot** du **message** est composé au maximum de **6** caractères (c'est-à-dire **7** lorsqu'il est stocké dans la liste de `mots` : `P:VALE` + `\0`)

Aussi, une **commande** possède, selon le cahier des charges, au maximum **4** **paramètres**.

Finalement, un **message** doit se limiter à **5** mots de **6** caractères chacun,
sachant que seulement **3** seront utilisés pour le **code**, **1-3** pour le **paramètre** et **4** pour la **valeur**.

Autrement dit, la fonction `PARSEUR_message()` est étudiée pour respecter le cahier des charges,
et n'est donc pas adaptée à toutes formes de commandes.

Forme d'un message le plus complet possible : `COD 1:val1 2:val2 3:val3 4:val4`.

Il est possible de modifier ces limites en changeant des valeurs dans le `struct Commande` de `PARSEUR.h`, 
et dans la fonction `PARSEUR_message` (uniquement`mots`) dans `PARSEUR.c`.

**!!! Attention : il n'y pas de vérification sur le respect de ces limites.**
