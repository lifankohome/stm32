#ifndef _FLASH_H_
#define _FLASH_H_

#include "main.h"

#define FLASH_ADDR 0x0801fe00


void flash_set(uint16_t addr, uint32_t val);
uint32_t flash_get(uint8_t addr);


#endif
