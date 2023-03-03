
#include "main.h"
#include <stdio.h>


void lon(void) {
  HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
}

void lof(void) {
  HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
}


void lon_command(char *arguments) {
  lon();
  printf("OK\n\r");
}

void lof_command(char *arguments) {
  lof();
  printf("OK\n\r");
}





