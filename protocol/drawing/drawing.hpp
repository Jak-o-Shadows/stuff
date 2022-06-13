#ifndef LIB_PROTOCOL_DRAWING_DRAWING_HPP_
#define LIB_PROTOCOL_DRAWING_DRAWING_HPP_


#include <stdint.h>

#include "macros.h"
#include "status.hpp"

#include "protocol/drawing/fonts.h"

#define WIDTH 32
#define HEIGHT 1
#define segmentPixelCount WIDTH*HEIGHT
#define segmentCommandSize segmentPixelCount / 8 + 1
typedef struct Segment_t{
    union Segment_u {
        uint8_t bytes[segmentCommandSize];
        struct components_s
        {
            uint8_t dataCommand;
            uint8_t pixelBuffer[segmentPixelCount / 8];
        } components;
    } data;
    uint8_t size_x;
    uint8_t size_y;
};


Status lookupPixelLocation(uint8_t *byteIdx, uint8_t *bitIdx, uint8_t segmentStart_x, uint8_t segmentStart_y, uint8_t segmentSize_x, uint8_t segmentSize_y, uint8_t pixel_x, uint8_t pixel_y);

Status pixelSet(Segment_t *segment, uint8_t segmentStart_x, uint8_t segmentStart_y, uint8_t pixel_x, uint8_t pixel_y);

Status pixelClear(Segment_t *segment, uint8_t segmentStart_x, uint8_t segmentStart_y, uint8_t pixel_x, uint8_t pixel_y);

Status pixelInvert(Segment_t *segment, uint8_t segmentStart_x, uint8_t segmentStart_y, uint8_t pixel_x, uint8_t pixel_y);

Status text(Segment_t *segment, uint8_t segmentStart_x, uint8_t segmentStart_y, uint8_t topLeft_x, uint8_t topLeft_y, char character);

Status words(Segment_t *segment, uint8_t segmentStart_x, uint8_t segmentStart_y, uint8_t topLeft_x, uint8_t topLeft_y, char word[], uint8_t wordLength);


#endif


