/*
 * Copyright (c) 2023 Fabrizio Carlassara <fabrizio.carlassara@gmail.com>
 */

#ifndef _DHT_H_
#define _DHT_H_

#include "hardware/gpio.h"
#include <stdint.h>

// Prototipos de funciones

void dht_init(uint8_t gpio);
void dht_read(float *temp_c, float *humidity);

#endif // _DHT_H_