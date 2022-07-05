#include "periph/i2c/i2cMaster.hpp"

#include <libopencm3/stm32/rcc.h> // Must include to fix 'BEGIN_DECLS' does not name a type

#include <libopencm3/stm32/f1/i2c.h>

Status i2cMaster_setup(uint32_t i2c)
{
    //disable i2c before changing config
    i2c_peripheral_disable(i2c);
    i2c_reset(i2c);

    i2c_set_standard_mode(i2c);
    i2c_set_clock_frequency(i2c, I2C_CR2_FREQ_16MHZ);

    i2c_set_ccr(i2c, 0xAA);
    i2c_set_dutycycle(i2c, I2C_CCR_DUTY_DIV2);

    i2c_set_trise(i2c, 0x11);

    i2c_enable_ack(i2c);

    //enable it
    i2c_peripheral_enable(i2c);

    return STATUSok;
}


Status i2cMaster_sendreg(uint32_t i2c, uint8_t addr, uint8_t reg, uint8_t data[], uint8_t numData){
    uint32_t reg32 __attribute__((unused));

    //send start
    i2c_send_start(i2c);

    //wait for the switch to master mode.
    while (!((I2C_SR1(i2c) & I2C_SR1_SB) &
             (I2C_SR2(i2c) & (I2C_SR2_MSL | I2C_SR2_BUSY))))
        ;

    //send address
    i2c_send_7bit_address(i2c, addr, I2C_WRITE);
    //check for the ack
    while (!(I2C_SR1(i2c) & I2C_SR1_ADDR))
        ;
    //must read SR2 to clear it
    reg32 = I2C_SR2(i2c);

    i2c_send_data(i2c, reg);
    while (!(I2C_SR1(i2c) & I2C_SR1_BTF))
        ; //wait for byte transferred flag

    for (int i = 0; i < numData; i++)
    {
        i2c_send_data(i2c, data[i]);
        while (!(I2C_SR1(i2c) & I2C_SR1_BTF))
            ; //wait for byte transferred flag
    }

    //send stop condition
    i2c_send_stop(i2c);
    for (int i = 0; i < 200; i++)
    {
        __asm__("nop");
    }

    return STATUSok;
}


Status i2cMaster_send(uint32_t i2c, uint8_t addr, uint8_t data[], uint8_t numData)
{
    uint32_t reg32 __attribute__((unused));

    //send start
    i2c_send_start(i2c);

    //wait for the switch to master mode.
    while (!((I2C_SR1(i2c) & I2C_SR1_SB) &
             (I2C_SR2(i2c) & (I2C_SR2_MSL | I2C_SR2_BUSY))))
        ;

    //send address
    i2c_send_7bit_address(i2c, addr, I2C_WRITE);
    //check for the ack
    while (!(I2C_SR1(i2c) & I2C_SR1_ADDR))
        ;
    //must read SR2 to clear it
    reg32 = I2C_SR2(i2c);

    for (int i = 0; i < numData; i++)
    {
        i2c_send_data(i2c, data[i]);
        while (!(I2C_SR1(i2c) & I2C_SR1_BTF))
            ; //wait for byte transferred flag
    }

    //send stop condition
    i2c_send_stop(i2c);
    for (int i = 0; i < 200; i++)
    {
        __asm__("nop");
    }

    return STATUSok;
}


Status i2cMaster_readreg(uint32_t i2c, uint8_t addr, uint8_t reg, uint8_t data[], uint8_t numData){

    // Butcher the i2c_transfer7 from http://libopencm3.org/docs/latest/stm32f1/html/group__i2c__file.html#gaf3d168765388b62e0a798fc9aedc8291
    //  Into one function as do not have an up to date version of libopencm3

    // Write dest addr
         while ((I2C_SR2(i2c) & I2C_SR2_BUSY)) {
         }
  
         i2c_send_start(i2c);
  
         // Wait for the end of the start condition, master mode selected, and BUSY bit set
         while ( !( (I2C_SR1(i2c) & I2C_SR1_SB)
                 && (I2C_SR2(i2c) & I2C_SR2_MSL)
                 && (I2C_SR2(i2c) & I2C_SR2_BUSY) ));
  
         i2c_send_7bit_address(i2c, addr, I2C_WRITE);
  
         // Waiting for address is transferred.
         while (!(I2C_SR1(i2c) & I2C_SR1_ADDR));
  
         // Clearing ADDR condition sequence.
         (void)I2C_SR2(i2c);

        i2c_send_data(i2c, reg);
        while (!(I2C_SR1(i2c) & (I2C_SR1_BTF)));


    // Receive data
    //  The STM32F1 i2c hardware is BROKEN when receiving multiple bytes at a time.
    //  (see the erratta - DocID14574 Rev 13).
    //  Hence just do it one byte at a time, hence halving the throughput

    for (int i = 0; i < numData; ++i) {

        i2c_send_start(i2c);
        i2c_disable_ack(i2c);

        // Wait for the end of the start condition, master mode selected, and BUSY bit set
        while ( !( (I2C_SR1(i2c) & I2C_SR1_SB)
                && (I2C_SR2(i2c) & I2C_SR2_MSL)
                && (I2C_SR2(i2c) & I2C_SR2_BUSY) ));

        i2c_send_7bit_address(i2c, addr, I2C_READ);

        // Waiting for address is transferred.
        while (!(I2C_SR1(i2c) & I2C_SR1_ADDR));
        // Clearing ADDR condition sequence.
        (void)I2C_SR2(i2c);

        i2c_send_stop(i2c);

            while (!(I2C_SR1(i2c) & I2C_SR1_RxNE));
            data[i] = i2c_get_data(i2c);
    }
    i2c_send_stop(i2c);

    (void)I2C_SR2(i2c);
    (void)I2C_SR1(i2c);

    return STATUSok;
}