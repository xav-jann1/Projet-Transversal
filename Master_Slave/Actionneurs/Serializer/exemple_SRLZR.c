#include <stdio.h>
#include "SRLZR.h"

void verif(char* text, char res, char test) {
  printf("   - %s: \t", text);

  if (res == test)
    printf("\x1B[32m%s\x1B[0m\n", "OK");
  else
    printf("\x1B[31m%s\x1B[0m\n", "NO");
}

int main(/*int argc, char const* argv[]*/) {
  // Initialisation de la communication avec le Serializer:
  SRLZR_init();

  // Envoie des commandes:

  if (SRLZR_sendCommand("cfg baud 19200") == 1) printf("error\n");

  printf("Test des commandes:\n");

  // Firmware:
  printf("\n** Firmware **\n");
  verif("firmware", 0, SRLZR_firmware());

  // Reset:
  printf("\n\n** Initialisation **\n");
  verif("reset", 0, SRLZR_reset());
  verif("restore", 0, SRLZR_restore());
  verif("rpid", 0, SRLZR_resetPID());

  // Config EncoderType:
  printf("\n\n** Config EncoderType **\n");
  verif("encoderType=0", 0, SRLZR_setEncoderType(0));
  verif("encoderType=1", 0, SRLZR_setEncoderType(1));
  verif("encoderType=2", 1, SRLZR_setEncoderType(2));
  verif("encoderType=?", 0, SRLZR_getEncoderType());

  // Config Baud Rate:
  printf("\n\n** Config Baud Rate **\n");
  verif("baud=0", 0, SRLZR_setBaud(0));
  verif("baud=1", 0, SRLZR_setBaud(1));
  verif("baud=2", 0, SRLZR_setBaud(2));
  verif("baud=6", 0, SRLZR_setBaud(6));
  verif("baud=9600", 0, SRLZR_setBaud(9600));
  verif("baud=57600", 0, SRLZR_setBaud(57600));
  verif("baud=115200", 0, SRLZR_setBaud(115200));
  verif("baud=?", 0, SRLZR_getBaud());

  // Config Units:
  printf("\n\n** Config Units **\n");
  verif("units=0", 0, SRLZR_setUnits(0));
  verif("units=1", 0, SRLZR_setUnits(1));
  verif("units=2", 0, SRLZR_setUnits(2));
  verif("units=-3", 1, SRLZR_setUnits(-3));
  verif("units=3", 1, SRLZR_setUnits(3));
  verif("units=?", 0, SRLZR_getUnits());

  return 0;
}
