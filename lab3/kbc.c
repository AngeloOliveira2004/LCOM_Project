//TODO:
//Função que lê o valor do kbc (Antes precisa de enviar valor para avisar que vai ler)
//Função que escreve o valor no kbc (Antes precisa de enviar valor para avisar que vai escrever)
#include <lcom/lcf.h>
#include <lcom/lab3.h>
#include <stdbool.h>
#include <stdint.h>
#include "i8042.h"

int(kbc_write_command)(uint8_t cmd){ //Usado para passar o valor de 0x20 -> Avisar que vai ler o command byte do kbc através do output buffer e 0x60 -> Avisar que vai enviar um novo command byte do kbc através do 0x64 como argumento
  if(sys_outb(IN_KBC_COMMANDS,cmd) != 0){
    return 1;
  }
  return 0;
}

int(kbc_write_command_args)(uint8_t *command_byte){ //Usado para passar command args para o endereço 0x64
  if(sys_outb(IN_KBC_COMMAND_ARGS,*command_byte)){
    return 1;
  }
  return 0;
}
