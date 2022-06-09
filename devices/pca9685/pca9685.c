#include "pca9685/pca9685.h"

void pca9685_setup(uint32_t i2c, uint8_t address)
{
    uint8_t addressShifted = address >> 1; //loc3 bitshifts for us

    uint8_t singleData[1];

    //set the pwm frequency
    singleData[0] = 0x10;
    i2cMaster_send(i2c, addressShifted, 0x00, singleData, 1); //mode1 -> sleep mode
    singleData[0] = 0x70;
    i2cMaster_send(i2c, addressShifted, 0xFE, singleData, 1); //prescale reg -> 60hz????
    singleData[0] = 0x20 | 0x01 | 0x80;                       //allcall, auto increment, restart
    i2cMaster_send(i2c, addressShifted, 0x00, singleData, 1); //mode1 -> auto increment | allcall | restart
}

void pca9685_setServoPos(uint32_t i2c, uint8_t address, uint8_t channel, uint16_t delay_count, uint16_t on_counts){
	// Calculate and then send the on & off time
	// delay_count from start of frame
	// on_counts from start of on time
	if (on_counts==0x00){
		// Avoid overflow - the off_count is subtracted to get the number anyway
		on_counts++;
	}
	uint16_t off_count = delay_count + on_counts - 1;
	if (off_count >= 4096) {
		off_count = off_count - 4096;
	}
	//Must send on time, off time
	uint8_t data[4];
	data[0] = (uint8_t) delay_count;
	data[1] = (uint8_t) (delay_count >> 8);
	data[2] = (uint8_t) off_count;
	data[3] = (uint8_t) (off_count >> 8);
	i2cMaster_send(i2c, address >> 1, 0x06+4*channel, data, 4);
}

