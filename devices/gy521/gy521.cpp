#include "devices/gy521/gy521.hpp"


Status gy521_init(GY521_t *dev)
{
    Status sts;

    // Set the data range
    clearReg(GYRO_CONFIG);
    GYRO_CONFIG.fs_sel = 0;  // 0=> +- 250 deg/s
    sts = i2cMaster_sendreg(dev->i2c, dev->address, ADDRESS_MAP.gyro_config, (uint8_t*) &GYRO_CONFIG, 1);

    clearReg(ACCEL_CONFIG);
    ACCEL_CONFIG.afs_sel = 0;  // 0=> +- 2g
    sts = i2cMaster_sendreg(dev->i2c, dev->address, ADDRESS_MAP.accel_config, (uint8_t*) &ACCEL_CONFIG, 1);

    // Set the clock source and turn off sleep mode
    clearReg(PWR_MGMT_1);
    PWR_MGMT_1.clksel = 1;  // X gyroscope reference
    PWR_MGMT_1.sleep = 0;
    sts = i2cMaster_sendreg(dev->i2c, dev->address, ADDRESS_MAP.pwr_mgmt_1, (uint8_t*) &PWR_MGMT_1, 1);

    return sts;
}


Status gy521_read(GY521_t *dev, int16_t *a, int16_t *g)
{
    uint8_t buffer[14];
    // Read all the way from ADDRESS_MAP.accel_xout_h to gyro_zout_l (inclusive)
    Status sts = i2cMaster_readreg(dev->i2c, dev->address, ADDRESS_MAP.accel_xout_h, buffer, 14);

    a[0] = JoinInt8(buffer[0], buffer[1]);
    a[1] = JoinInt8(buffer[2], buffer[3]);
    a[2] = JoinInt8(buffer[4], buffer[5]);
    g[0] = JoinInt8(buffer[8], buffer[9]);
    g[1] = JoinInt8(buffer[10], buffer[11]);
    g[2] = JoinInt8(buffer[12], buffer[13]);

    return sts;
}
