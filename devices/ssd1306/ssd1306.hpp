
#include <stdint.h>

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

typedef struct SSD1306_s
{
    state_t state;
    uint8_t address;
    mode_t mode;
} SSD1306


Status sendCommands(SSD1306 *dev, uint8_t commands[], uint8_t numCommands);

Status init(SSD1306 *dev);

