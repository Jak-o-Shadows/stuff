


#includ <stdint.h>


Status lookupPixelLocation(uint8_t *byteIdx, uint8_t *bitIdx, uint8_t segmentStart_x, uint8_t segmentStart_y, uint8_t segmentSize_x, uint8_t segmentSize_y, uint8_t pixel_x, uint8_t pixel_y);

Status pixelSet(Segment_t *buffer, uint8_t segmentStart_x, uint8_t segmentStart_y, uint8_t segmentSize_x, uint8_t segmentSize_y, uint8_t pixel_x, uint8_t pixel_y);

Status pixelClear(Segment_t *buffer, uint8_t segmentStart_x, uint8_t segmentStart_y, uint8_t segmentSize_x, uint8_t segmentSize_y, uint8_t pixel_x, uint8_t pixel_y);

Status text(Segment_t *buffer, uint8_t segmentStart_x, uint8_t segmentStart_y, uint8_t segmentSize_x, uint8_t segmentSize_y, uint8_t topLeft_x, uint8_t topLeft_y, char character);

Status words(Segment_t *buffer, uint8_t segmentStart_x, uint8_t segmentStart_y, uint8_t segmentSize_x, uint8_t segmentSize_y, uint8_t topLeft_x, uint8_t topLeft_y, char word[], uint8_t wordLength);





