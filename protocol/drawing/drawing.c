#include "protocol/drawing/drawing.h"

// Graphics Drawing Functions

Status lookupPixelLocation(uint8_t *byteIdx, uint8_t *bitIdx, uint8_t segmentStart_x, uint8_t segmentStart_y, uint8_t segmentSize_x, uint8_t segmentSize_y, uint8_t pixel_x, uint8_t pixel_y)
{
    // First check if the pixel is within the segment
    if (((pixel_x >= segmentStart_x) && (pixel_x < (segmentStart_x + segmentSize_x))) &&
        ((pixel_y >= segmentStart_y) && (pixel_y < (segmentStart_y + segmentSize_y))))
    {
        // Continue proceccessing
    }
    else
    {
        return STATUSbad;
    }

    // The pixel is within the display area set by the buffer - figure
    //  out where to set

    // First determine which pixel buffer byte the pixel is in
    //  Now, the bytes go x, then y. Always.
    uint8_t local_x = pixel_x - segmentStart_x;
    uint8_t local_y = pixel_y - segmentStart_y;
    uint8_t byteColumnIndex = local_x / 8;
    uint8_t byteRowIndex = local_y;
    *byteIdx = (byteRowIndex * segmentSize_x / 8) + byteColumnIndex;
    *bitIdx = local_x - byteColumnIndex * 8;

    return STATUSok;
}

Status pixelSet(Segment_t *buffer, uint8_t segmentStart_x, uint8_t segmentStart_y, uint8_t segmentSize_x, uint8_t segmentSize_y, uint8_t pixel_x, uint8_t pixel_y)
{
    // Set a pixel in the segment, if applicable

    uint8_t byteIdx;
    uint8_t bitIdx;
    Status sts = lookupPixelLocation(&byteIdx, &bitIdx, segmentStart_x, segmentStart_y, segmentSize_x, segmentSize_y, pixel_x, pixel_y);

    if (sts == STATUSok)
    {
        SetBit(bitIdx, buffer->components.pixelBuffer[byteIdx]);

        return STATUSok;
    }
    else
    {
        return sts;
    }
}

Status pixelClear(Segment_t *buffer, uint8_t segmentStart_x, uint8_t segmentStart_y, uint8_t segmentSize_x, uint8_t segmentSize_y, uint8_t pixel_x, uint8_t pixel_y)
{
    // Clear a pixel in the segment, if applicable
    uint8_t byteIdx;
    uint8_t bitIdx;
    Status sts = lookupPixelLocation(&byteIdx, &bitIdx, segmentStart_x, segmentStart_y, segmentSize_x, segmentSize_y, pixel_x, pixel_y);

    if (sts == STATUSok)
    {
        ClearBit(bitIdx, buffer->components.pixelBuffer[byteIdx]);

        return STATUSok;
    }
    else
    {
        return sts;
    }
}

Status text(Segment_t *buffer, uint8_t segmentStart_x, uint8_t segmentStart_y, uint8_t segmentSize_x, uint8_t segmentSize_y, uint8_t topLeft_x, uint8_t topLeft_y, char character)
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
        Status sts = lookupPixelLocation(&byteIdx, &bitIdx, segmentStart_x, segmentStart_y, segmentSize_x, segmentSize_y, y, x);
        if (sts == STATUSok)
        {
            ClearBit(bitIdx, buffer->components.pixelBuffer[byteIdx]);
        }
    }
}

Status words(Segment_t *buffer, uint8_t segmentStart_x, uint8_t segmentStart_y, uint8_t segmentSize_x, uint8_t segmentSize_y, uint8_t topLeft_x, uint8_t topLeft_y, char word[], uint8_t wordLength)
{
    // Write a word into the segment

    uint8_t x = topLeft_x;
    uint8_t y = topLeft_y;

    uint8_t spaceWidth = 3;

    for (int charIdx = 0; charIdx < wordLength; charIdx++)
    {
        text(buffer, segmentStart_x, segmentStart_y, segmentSize_x, segmentSize_y, x, y, word[charIdx]);
        x = x + spaceWidth + fontWidthGet(word[charIdx]);
    }
}