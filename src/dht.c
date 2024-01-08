#include "dht.h"
#include "pico/stdlib.h"

// Variable para guardar el numero de GPIO usado
static uint8_t dht_gpio;

// Constante de tiempo para DHT11
static const uint timeout = 85;

/**
 * @brief Inicializa el GPIO para usar el DHT
 * @param gpio numero de GPIO
*/
void dht_init(uint8_t gpio) {
    // Guardo el numero de GPIO usado
    dht_gpio = gpio;
    // Inicializo el GPIO
    gpio_init(dht_gpio);
}

/**
 * @brief Hace una lectura del DHT
 * @param humidity puntero a donde guardar el valor de humedad
 * @param temp_c puntero a donde guardar el valor de temperatura
*/
void dht_read(float *humidity, float *temp_c) {
    // Array para guardar los 5 bytes de datos
    uint8_t data[5] = {0, 0, 0, 0, 0};
    // Variables auxiliares
    uint8_t last = 1;
    uint8_t j = 0;

    // Inicializo GPIO como salida
    gpio_set_dir(dht_gpio, GPIO_OUT);
    // Mando el GPIO a cero por 20 ms
    gpio_put(dht_gpio, 0);
    sleep_ms(20);
    // Configuro como entrada
    gpio_set_dir(dht_gpio, GPIO_IN);

    // Cuento los 40 bits mientras que no supere el timout
    for (uint8_t i = 0; i < timeout; i++) {
        uint8_t count = 0;
        // Espero que el valor anterior cambie
        while (gpio_get(dht_gpio) == last) {
            count++;
            sleep_us(1);
            if (count == 255) break;
        }
        // Actualizo el valor del bus
        last = gpio_get(dht_gpio);
        if (count == 255) break;

        if ((i >= 4) && (i % 2 == 0)) {
            data[j / 8] <<= 1;
            if (count > 16) data[j / 8] |= 1;
            j++;
        }
    }

    // Verifico que hayan llegado los 40 bits y que el checksum sea el correcto
    if ((j >= 40) && (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF))) {
        //
        *humidity = (float) ((data[0] << 8) + data[1]) / 10;
        if (*humidity > 100) {
            *humidity = data[0];
        }
        *temp_c = (float) (((data[2] & 0x7F) << 8) + data[3]) / 10;
        if (*temp_c > 125) {
            *temp_c = data[2];
        }
        if (data[2] & 0x80) {
            *temp_c = -(*temp_celsius);
        }
    } else {
        printf("Bad data\n");
    }
}