
#include "devices/ssd1306/ssd1306.h"

Status sendCommands(SSD1306 *dev, uint8_t commands[], uint8_t numCommands)
{

    uint8_t commandAddress = 0; // D/C = 0, Co = 0

    // Probably a nicer way of doing this?
    uint8_t commandBuffer[1 + 256]; // Size hardcoded to type of `numCommands`

    // Starts with the commandAddress
    commandBuffer[0] = commandAddress;
    for (int i = 0; i < numCommands; i++)
    {
        commandBuffer[i + 1] = commands[i];
    }
    return i2cSendBytes(I2C2, dev->address, commandBuffer, 1 + numCommands);
}

Status init(SSD1306 *dev)
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