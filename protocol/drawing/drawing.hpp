


#include <stdint.h>

#include "status.hpp"

#define WIDTH 32
#define HEIGHT 128
#define segmentPixelCount WIDTH
#define segmentCommandSize segmentPixelCount / 8 + 1
typedef union Segment_u {
    uint8_t bytes[segmentCommandSize];
    struct components_s
    {
        uint8_t dataCommand;
        uint8_t pixelBuffer[segmentPixelCount / 8];
    } components;
} Segment_t;


Status lookupPixelLocation(uint8_t *byteIdx, uint8_t *bitIdx, uint8_t segmentStart_x, uint8_t segmentStart_y, uint8_t segmentSize_x, uint8_t segmentSize_y, uint8_t pixel_x, uint8_t pixel_y);

Status pixelSet(Segment_t *buffer, uint8_t segmentStart_x, uint8_t segmentStart_y, uint8_t segmentSize_x, uint8_t segmentSize_y, uint8_t pixel_x, uint8_t pixel_y);

Status pixelClear(Segment_t *buffer, uint8_t segmentStart_x, uint8_t segmentStart_y, uint8_t segmentSize_x, uint8_t segmentSize_y, uint8_t pixel_x, uint8_t pixel_y);

Status text(Segment_t *buffer, uint8_t segmentStart_x, uint8_t segmentStart_y, uint8_t segmentSize_x, uint8_t segmentSize_y, uint8_t topLeft_x, uint8_t topLeft_y, char character);

Status words(Segment_t *buffer, uint8_t segmentStart_x, uint8_t segmentStart_y, uint8_t segmentSize_x, uint8_t segmentSize_y, uint8_t topLeft_x, uint8_t topLeft_y, char word[], uint8_t wordLength);





