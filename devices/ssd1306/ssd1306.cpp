
#include "devices/ssd1306/ssd1306.hpp"

Status sendCommands(SSD1306_t *dev, uint8_t commands[], uint8_t numCommands)
{
    uint8_t commandAddress = 0; // D/C = 0, Co = 0
    return i2cMaster_sendreg(dev->i2c, dev->address, commandAddress, commands, numCommands);
}

Status init(SSD1306_t *dev)
{

    uint8_t commands[] = {
        // Display Off
        0xAE,
        // Display Offset
        0xD3, // Command
        0,    // Command payload, (No Offset)
        // Display Start line
        0x40,
        // Segment Re-Map
        0xA0 | 0x1, // Rotate screen 180
        // Vertical Mode - works better with the 'line based' paradigm
        0x20, // Set memory mode command
        1,    // Vertical
        // COM SCAN DEC
        0xC8,
        // COM PINS
        0xDA,
        0x02, // Specific to 128x32 px
        // Multiplex
        0xA8,
        32 - 1,
        // Set Oscillator Frequency
        0xD5, // Command
        0x80, // Recommended value (p64 of App Note)
        // Charge Pump
        0x8D,
        // pre-Charge
        0xD9,
        0xF1,
        // Internal VCC
        0x14,
        // VCOM Detect
        0xD8,
        0x40,
        // Display Contrast
        0x81, // Command
        128,  // Set between 1ish and 255ish
        // Display resume
        0xA4,
        // Normal display
        0xA6,
        // Display on
        0xAF};

    static const uint8_t numCommands = 25;

    return sendCommands(dev, commands, numCommands);

    return STATUSok;
}

void OLED_address(SSD1306_t *dev, uint8_t x, uint8_t y)
{
    uint8_t commands[] = {
        // Set column addressing
        0x21,
        0x00,
        128 - 1,
        // Set page addressing extents
        0x22,
        0x00,
        32 / 8 - 1};
    sendCommands(dev, commands, 6);
}

// Scrolling
Status setupScroll(SSD1306_t *dev, bool scrollRight)
{
    uint8_t scrollDir;
    if (scrollRight)
    {
        scrollDir = 0x26;
    }
    else
    {
        scrollDir = 0x27;
    }

    uint8_t commands[] = {
        scrollDir, // Command - scroll left or right
        0x00,      // Dummy Byte
        0,         // Start page address
        0,         // Scroll update rate - non-linear mapping in terms of frames form data sheet
        0x0F,      // End page address
        0,         // Dummy Byte
        0xFF       // Dummy Byte
    };
    return sendCommands(dev, commands, 7);
}

Status scrollState(SSD1306_t *dev, bool enabled)
{
    uint8_t bytes;
    if (enabled)
    {
        bytes = 0x2F;
    }
    else
    {
        bytes = 0x2E;
    }
    return sendCommands(dev, &bytes, 1);
}
