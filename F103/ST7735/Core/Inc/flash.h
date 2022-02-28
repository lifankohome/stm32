#ifndef _FLASH_H_
#define _FLASH_H_

#include "main.h"

#define FLASH_ADDR 0x0801fe00


uint32_t flash_get(uint16_t addr);
void flash_set(uint16_t addr, uint8_t val);


#endif
