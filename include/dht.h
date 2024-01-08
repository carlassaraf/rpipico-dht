/*
 * Copyright (c) 2023 Fabrizio Carlassara <fabrizio.carlassara@gmail.com>
 */

#ifndef _DHT_H_
#define _DHT_H_

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include <stdint.h>

typedef enum {
    DHT_OK,
    DHT_TIMEOUT,
    DHT_BAD_CHECKSUM
} dht_status_t;

// Prototipos de funciones

void dht_init(uint8_t gpio);
dht_status_t dht_read(float *humidity, float *temp_c);

#endif // _DHT_H_