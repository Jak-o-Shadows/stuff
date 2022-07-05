#ifndef DEVICES_GY521_HPP_
#define DEVICES_GY521_HPP_

#include <stdint.h>
#include <cstring>

#include "macros.h"

#include "status.hpp"
#include "periph/i2c/i2cMaster.hpp"

#define clearReg(reg) memset(&reg, 0, sizeof(reg))

//uint8_t address7bit = 0b110100x; // x controlled by logic level on AD0


// Register mapping
//  From RM-MPU-6000A-00 Rev 4.2
struct {
    unsigned short xg_test: 5;
    unsigned short xa_test: 3;
} SELF_TEST_X;

struct {
    unsigned short yg_test: 5;
    unsigned short ya_test: 3;
} SELF_TEST_Y;

struct {
    unsigned short zg_test: 5;
    unsigned short za_test: 3;
} SELF_TEST_Z;

struct {
    unsigned short za_test: 2;
    unsigned short ya_test: 2;
    unsigned short xa_test: 2;
    unsigned short reserved0: 2;
} SELF_TEST_A;

struct {
    unsigned short smplrt_div;
} SMPLRT_DIV;

struct {
    unsigned short dlpf_cfg: 3;
    unsigned short ext_sync_set: 3;
    unsigned short reserved0: 2;
} CONFIG;

struct {
    unsigned short reserved0: 3;
    unsigned short fs_sel: 2;
    unsigned short reserved1: 3;
} GYRO_CONFIG;

struct {
    unsigned short reserved0: 3;
    unsigned short afs_sel: 2;
    unsigned short za_st: 1;
    unsigned short ya_st: 1;
    unsigned short xa_st: 1;
} ACCEL_CONFIG;

struct {
    unsigned short slv0_fifo_en: 1;
    unsigned short slv1_fifo_en: 1;
    unsigned short slv2_fifo_en: 1;
    unsigned short accel_fifo_en: 1;
    unsigned short zg_fifo_en: 1;
    unsigned short yg_fifo_en: 1;
    unsigned short xg_fifo_en: 1;
    unsigned short temp_fifo_en: 1;
} FIFO_EN;

struct {
    unsigned short i2c_msg_clk: 4;
    unsigned short i2c_mst_p_nsr: 1;
    unsigned short slv_3_fifo_en: 1;
    unsigned short wait_for_es: 1;
    unsigned short mult_msg_en: 1;
} I2C_MSG_CTRL;

// Start Slave I2C devices
struct {
    unsigned short skip: 8;
} I2C_SLV0_ADDR;

struct {
    unsigned short skip: 8;
} I2C_SLV0_REG;

struct {
    unsigned short skip: 8;
} I2C_SLV0_CTRL;

// TODO: SLV1, SLV2, SLV3
//  They are basically the same as SLV0

struct {
    unsigned short skip: 8;
} I2C_SLV4_ADDR;

struct {
    unsigned short skip: 8;
} I2C_SLV4_REG;

struct {
    unsigned short i2c_slv4_do: 8;
} I2C_SLV4_D0;

struct {
    unsigned short skip: 8;
} I2C_SLV4_CTRL;

struct {
    unsigned short i2c_slv4_di: 8;
} I2C_SLV4_DI;

struct {
    unsigned short skip:8;
} I2C_MSG_STATUS;

// Finish I2C slave devices

struct {
    unsigned short reserved0: 1;
    unsigned short i2c_bypass_en: 1;
    unsigned short fsync_int_en: 1;
    unsigned short int_rd_clear: 1;
    unsigned short latch_int_en: 1;
    unsigned short int_open: 1;
    unsigned short int_level: 1;
} INT_PIN_CFG;

struct {
    unsigned short data_rdy_en: 1;
    unsigned short reserved0: 2;
    unsigned short i2c_mst_int_en: 1;
    unsigned short fifo_oflow_en: 1;
    unsigned short reserved1: 3;
} INT_ENABLE;

struct {
    unsigned short data_rd_int: 1;
    unsigned short reserved0: 2;
    unsigned short i2c_mst_int: 1;
    unsigned short fifo_oflow_int: 1;
    unsigned short reserved1: 3;
} INT_STATUS;

// Skip a few

struct {
    unsigned short temp_reset: 1;
    unsigned short accel_reset: 1;
    unsigned short gyro_reset: 1;
    unsigned short reserved0: 5;
} SIGNAL_PATH_RESET;

struct {
    unsigned short sig_cond_reset: 1;
    unsigned short i2c_msg_reset: 1;
    unsigned short fifo_reset: 1;
    unsigned short reserved0: 1;
    unsigned short i2c_if_dis: 1;
    unsigned short i2c_mst_en: 1;
    unsigned short fifo_en: 1;
    unsigned short reserved1: 1;
} USER_CTRL;

struct {
    unsigned short clksel: 2;
    unsigned short temp_dis: 1;
    unsigned short reserved0: 1;
    unsigned short cycle: 1;
    unsigned short sleep: 1;
    unsigned short device_reset: 1;
} PWR_MGMT_1;





// Define the address map

struct {
    uint8_t self_test_x         = 0x0d;
    uint8_t self_test_y         = 0x0e;
    uint8_t self_test_z         = 0x0f;
    uint8_t self_test_a         = 0x10;
    uint8_t smplrt_div          = 0x19;
    uint8_t config              = 0x1a;
    uint8_t gyro_config         = 0x1b;
    uint8_t accel_config        = 0x1c;
    uint8_t fifo_en             = 0x23;
    // Skip a few which I don't care about currently
    uint8_t int_pn_cfg          = 0x37;
    uint8_t int_enable          = 0x38;
    uint8_t int_status          = 0x3a;
    uint8_t accel_xout_h        = 0x3b;
    uint8_t accel_xout_l        = 0x3c;
    uint8_t accel_yout_h        = 0x3d;
    uint8_t accel_yout_l        = 0x3e;
    uint8_t accel_zout_h        = 0x3f;
    uint8_t accel_zout_l        = 0x40;
    uint8_t temp_out_h          = 0x41;
    uint8_t temp_out_l          = 0x42;
    uint8_t gyro_xout_h         = 0x43;
    uint8_t gyro_xout_l         = 0x44;
    uint8_t gyro_yout_h         = 0x45;
    uint8_t gyro_yout_l         = 0x46;
    uint8_t gyro_zout_h         = 0x47;
    uint8_t gyro_zout_l         = 0x48;
    // Skip more I don't care about
    uint8_t signal_path_reset   = 0x68;
    uint8_t user_ctrl           = 0x6a;
    uint8_t pwr_mgmt_1          = 0x6b;
    uint8_t pwr_mgmt_2          = 0x6c;
    uint8_t fifo_counth         = 0x72;
    uint8_t fifo_countl         = 0x73;
    uint8_t fifo_r_w            = 0x74;
    uint8_t who_am_i            = 0x75;
} ADDRESS_MAP;









typedef struct
{
    //state_t state;
    uint8_t address;
    //mode_t mode;
    uint32_t i2c;
} GY521_t;

Status gy521_init(GY521_t *dev);

Status gy521_read(GY521_t *dev, int16_t *a, int16_t *g);







#endif