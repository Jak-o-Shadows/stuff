#ifndef PERIPH_I2CMASTER_HPP_
#define PERIPH_I2CMASTER_HPP_

#include <stdint.h>

#include "status.hpp"

Status i2cMaster_setup(uint32_t i2c);

Status i2cMaster_sendreg(uint32_t i2c, uint8_t addr, uint8_t reg, uint8_t data[], uint8_t numData);

Status i2cMaster_send(uint32_t i2c, uint8_t addr, uint8_t data[], uint8_t numData);

Status i2cMaster_readreg(uint32_t i2c, uint8_t addr, uint8_t reg, uint8_t data[], uint8_t numData);

#endif
