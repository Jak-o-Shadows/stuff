#ifndef LIB_PROTOCOL_DRAWING_DRAWING_HPP_
#define LIB_PROTOCOL_DRAWING_DRAWING_HPP_


#include <stdint.h>

#include "macros.h"
#include "status.hpp"

#include "protocol/drawing/fonts.h"

#define WIDTH 32
#define HEIGHT 16

template<int BUF_LEN>
struct Segment_t{
    union Segment_u {
        uint8_t bytes[BUF_LEN+1];
        struct components_s
        {
            uint8_t dataCommand;
            uint8_t pixelBuffer[BUF_LEN];
        } components;
    } data;
    const uint8_t bytes_len = BUF_LEN+1;
    uint8_t size_x;
    uint8_t size_y;
};

template<int DW, int DH, int SW, int SH, int NUM_SEG>
class Display {
public:

    const uint8_t display_width = DW;
    const uint8_t display_height = DH;
    const uint8_t segment_width = SW;
    const uint8_t segment_height = SH;

    Segment_t<SW*SH/8> segments [NUM_SEG];
    const uint8_t num_segments = NUM_SEG;

    Display();



};


Status lookupPixelLocation(uint8_t *byteIdx, uint8_t *bitIdx, uint8_t segmentStart_x, uint8_t segmentStart_y, uint8_t segmentSize_x, uint8_t segmentSize_y, uint8_t pixel_x, uint8_t pixel_y);

template<int BUF_LEN>
Status pixelSet(Segment_t<BUF_LEN> *segment, uint8_t segmentStart_x, uint8_t segmentStart_y, uint8_t pixel_x, uint8_t pixel_y);

template<int BUF_LEN>
Status pixelClear(Segment_t<BUF_LEN> *segment, uint8_t segmentStart_x, uint8_t segmentStart_y, uint8_t pixel_x, uint8_t pixel_y);

template<int BUF_LEN>
Status pixelInvert(Segment_t<BUF_LEN> *segment, uint8_t segmentStart_x, uint8_t segmentStart_y, uint8_t pixel_x, uint8_t pixel_y);

template<int BUF_LEN>
Status text(Segment_t<BUF_LEN> *segment, uint8_t segmentStart_x, uint8_t segmentStart_y, uint8_t topLeft_x, uint8_t topLeft_y, char character);

template<int BUF_LEN>
Status words(Segment_t<BUF_LEN> *segment, uint8_t segmentStart_x, uint8_t segmentStart_y, uint8_t topLeft_x, uint8_t topLeft_y, char word[], uint8_t wordLength);






// Definitions of functions and classes
//  Required to also be in the header for templating reasons


template<int BUF_LEN>
Status pixelSet(Segment_t<BUF_LEN> *segment, uint8_t segmentStart_x, uint8_t segmentStart_y, uint8_t pixel_x, uint8_t pixel_y)
{
    // Set a pixel in the segment, if applicable

    uint8_t byteIdx;
    uint8_t bitIdx;
    Status sts = lookupPixelLocation(&byteIdx, &bitIdx, segmentStart_x, segmentStart_y, segment->size_x, segment->size_y, pixel_x, pixel_y);

    if (sts == STATUSok)
    {
        SetBit(bitIdx, segment->data.components.pixelBuffer[byteIdx]);

        return STATUSok;
    }
    else
    {
        return sts;
    }
}

template<int BUF_LEN>
Status pixelClear(Segment_t<BUF_LEN> *segment, uint8_t segmentStart_x, uint8_t segmentStart_y, uint8_t pixel_x, uint8_t pixel_y)
{
    // Clear a pixel in the segment, if applicable
    uint8_t byteIdx;
    uint8_t bitIdx;
    Status sts = lookupPixelLocation(&byteIdx, &bitIdx, segmentStart_x, segmentStart_y, segment->size_x, segment->size_y, pixel_x, pixel_y);

    if (sts == STATUSok)
    {
        ClearBit(bitIdx, segment->data.components.pixelBuffer[byteIdx]);

        return STATUSok;
    }
    else
    {
        return sts;
    }
}

template<int BUF_LEN>
Status pixelInvert(Segment_t<BUF_LEN> *segment, uint8_t segmentStart_x, uint8_t segmentStart_y, uint8_t pixel_x, uint8_t pixel_y)
{
    // Invert a pixel in the segment, if applicable
    uint8_t byteIdx;
    uint8_t bitIdx;
    Status sts = lookupPixelLocation(&byteIdx, &bitIdx, segmentStart_x, segmentStart_y, segment->size_x, segment->size_y, pixel_x, pixel_y);

    if (sts == STATUSok)
    {
        InvertBit(bitIdx, segment->data.components.pixelBuffer[byteIdx]);

        return STATUSok;
    }
    else
    {
        return sts;
    }
}

template<int BUF_LEN>
Status text(Segment_t<BUF_LEN> *segment, uint8_t segmentStart_x, uint8_t segmentStart_y, uint8_t topLeft_x, uint8_t topLeft_y, char character)
{
    // Draw a text character if it is appplicable to the segment at all
    // First check if any of the character is useful

    // First check if the character is within the segment

    /*
    uint8_t bottomRight_x = topLeft_x + charWidth;
    uint8_t bottomRight_y = topLeft_y + charHeight;
    bool draw = false;
    if ((topLeft_x >= segmentStart_x) && (topLeft_y >= segmentStart_y))
    {
        draw = true;
        // It MAY be contained.
        if ((bottomRight_x >= (segmentStart_x + segmentSize_x)) && (bottomRight_y <= (segmentStart_y + segmentSize_y)))
        {
            // Continue
            draw = true;
        }
    }
    draw = true;
    if (!draw)
    {
        return STATUSbad;
    }
    */

    // The character is within the segment - hence iterate over its pixels and draw it
    uint16_t charWidth = fontWidthGet(character);
    uint16_t charHeight = fontHeightGet(character);

    uint16_t numPixels = fontNumberPixelsGet(character);
    uint16_t *pixels = fontGet(character);

    // Clear a pixel in the segment, if applicable
    uint8_t byteIdx;
    uint8_t bitIdx;
    for (int pxNum = 0; pxNum < numPixels; pxNum++)
    {
        uint8_t x = topLeft_x + pixels[pxNum] - charWidth * (pixels[pxNum] / charWidth); // Remember, integer division floors
        uint8_t y = topLeft_y + pixels[pxNum] / charWidth;
        Status sts = lookupPixelLocation(&byteIdx, &bitIdx, segmentStart_x, segmentStart_y, segment->size_x, segment->size_y, y, x);  // Flip y, x for reasons???
        if (sts == STATUSok)
        {
            InvertBit(bitIdx, segment->data.components.pixelBuffer[byteIdx]);
        }
    }
    return STATUSok;
}

template<int BUF_LEN>
Status words(Segment_t<BUF_LEN> *segment, uint8_t segmentStart_x, uint8_t segmentStart_y, uint8_t topLeft_x, uint8_t topLeft_y, char word[], uint8_t wordLength)
{
    // Write a word into the segment

    uint8_t x = topLeft_x;
    uint8_t y = topLeft_y;

    uint8_t spaceWidth = 3;

    for (int charIdx = 0; charIdx < wordLength; charIdx++)
    {
        text(segment, segmentStart_x, segmentStart_y, x, y, word[charIdx]);
        x = x + spaceWidth + fontWidthGet(word[charIdx]);
    }

    return STATUSok;
}

template<int DW, int DH, int SW, int SH, int NUM_SEG>
Display<DW, DH, SW, SH, NUM_SEG>::Display() {
    for( int i = 0 ;i<num_segments;i++ ) {
        segments[i].data.components.dataCommand = 0x40;
        segments[i].size_x = segment_width;
        segments[i].size_y = segment_height;
    }
}





#endif


