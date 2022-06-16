#include "protocol/drawing/drawing.hpp"

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
    uint8_t byteColumnIndex = local_x / 8;  // Integer divide floors
    uint8_t byteRowIndex = local_y;
    *byteIdx = (byteRowIndex * segmentSize_x / 8) + byteColumnIndex;
    *bitIdx = local_x - byteColumnIndex * 8;

    return STATUSok;
}

