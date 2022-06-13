#ifndef DEVICES_SSD1306_HPP_
#define DEVICES_SSD1306_HPP_

#include <stdint.h>

#include "status.hpp"
#include "periph/i2c/i2cMaster.hpp"

// SDS1036 OLED Specific

typedef enum state_e
{
    DEAD = 0,
    INIT,
    OK
} state_t;

typedef enum mode_e
{
    PAGE = 0,
    HORI,
    VERT
} mode_t;

typedef struct
{
    state_t state;
    uint8_t address;
    mode_t mode;
    uint32_t i2c;
} SSD1306_t;

#define SCREEN_HEIGHT 128
#define SCREEN_WIDTH 32

void OLED_address(SSD1306_t *dev, uint8_t x, uint8_t y);

Status sendCommands(SSD1306_t *dev, uint8_t commands[], uint8_t numCommands);

Status init(SSD1306_t *dev);

Status setupScroll(SSD1306_t *dev, bool scrollRight);

Status scrollState(SSD1306_t *dev, bool enabled);

#endif