/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 *
 * @file      raspberrypi4b_driver_bmp390_interface.c
 * @brief     raspberrypi4b driver bmp390 interface source file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2022-05-15
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2022/05/15  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#include "driver_bmp390_interface.h"
#include "iic.h"
#include "spi.h"
#include <stdarg.h>

/**
 * @brief iic device name definition
 */
#define IIC_DEVICE_NAME "/dev/i2c-1"        /**< iic device name */

/**
 * @brief spi device name definition
 */
#define SPI_DEVICE_NAME "/dev/spidev0.0"    /**< spi device name */

/**
 * @brief iic device hanble definition
 */
static int gs_iic_fd;                       /**< iic handle */

/**
 * @brief spi device hanble definition
 */
static int gs_spi_fd;                       /**< spi handle */

/**
 * @brief  interface iic bus init
 * @return status code
 *         - 0 success
 *         - 1 iic init failed
 * @note   none
 */
uint8_t bmp390_interface_iic_init(void)
{
    return iic_init(IIC_DEVICE_NAME, &gs_iic_fd);
}

/**
 * @brief  interface iic bus deinit
 * @return status code
 *         - 0 success
 *         - 1 iic deinit failed
 * @note   none
 */
uint8_t bmp390_interface_iic_deinit(void)
{   
    return iic_deinit(gs_iic_fd);
}

/**
 * @brief      interface iic bus read
 * @param[in]  addr is the iic device write address
 * @param[in]  reg is the iic register address
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the length of the data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t bmp390_interface_iic_read(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len)
{
    return iic_read(gs_iic_fd, addr, reg, buf, len);
}

/**
 * @brief     interface iic bus write
 * @param[in] addr is the iic device write address
 * @param[in] reg is the iic register address
 * @param[in] *buf points to a data buffer
 * @param[in] len is the length of the data buffer
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t bmp390_interface_iic_write(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len)
{
    return iic_write(gs_iic_fd, addr, reg, buf, len);
}

/**
 * @brief  interface spi bus init
 * @return status code
 *         - 0 success
 *         - 1 spi init failed
 * @note   none
 */
uint8_t bmp390_interface_spi_init(void)
{
    return spi_init(SPI_DEVICE_NAME, &gs_spi_fd, SPI_MODE_TYPE_3, 1000 * 1000 * 2);
}

/**
 * @brief  interface spi bus deinit
 * @return status code
 *         - 0 success
 *         - 1 spi deinit failed
 * @note   none
 */
uint8_t bmp390_interface_spi_deinit(void)
{   
    return spi_deinit(gs_spi_fd);
}

/**
 * @brief      interface spi bus read
 * @param[in]  reg is the register address
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the length of data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t bmp390_interface_spi_read(uint8_t reg, uint8_t *buf, uint16_t len)
{
    return spi_read(gs_spi_fd, reg, buf, len);
}

/**
 * @brief     interface spi bus write
 * @param[in] reg is the register address
 * @param[in] *buf points to a data buffer
 * @param[in] len is the length of data buffer
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t bmp390_interface_spi_write(uint8_t reg, uint8_t *buf, uint16_t len)
{
    return spi_write(gs_spi_fd, reg, buf, len);
}

/**
 * @brief     interface delay ms
 * @param[in] ms
 * @note      none
 */
void bmp390_interface_delay_ms(uint32_t ms)
{
    usleep(1000 * ms);
}

/**
 * @brief     interface print format data
 * @param[in] fmt is the format data
 * @note      none
 */
void bmp390_interface_debug_print(const char *const fmt, ...)
{
    char str[256];
    uint8_t len;
    va_list args;
    
    memset((char *)str, 0, sizeof(char)*256); 
    va_start(args, fmt);
    vsnprintf((char *)str, 256, (char const *)fmt, args);
    va_end(args);
    
    len = strlen((char *)str);
    (void)printf((uint8_t *)str, len);
}

/**
 * @brief     interface receive callback
 * @param[in] type is the interrupt type
 * @note      none
 */
void bmp390_interface_receive_callback(uint8_t type)
{
    switch (type)
    {
        case BMP390_INTERRUPT_STATUS_FIFO_WATERMARK :
        {
            bmp390_interface_debug_print("bmp390: irq fifo watermark.\n");
            
            break;
        }
        case BMP390_INTERRUPT_STATUS_FIFO_FULL :
        {
            bmp390_interface_debug_print("bmp390: irq fifo full.\n");
            
            break;
        }
        case BMP390_INTERRUPT_STATUS_DATA_READY :
        {
            bmp390_interface_debug_print("bmp390: irq data ready.\n");
            
            break;
        }
        default :
        {
            bmp390_interface_debug_print("bmp390: unknown code.\n");
            
            break;
        }
    }
}
