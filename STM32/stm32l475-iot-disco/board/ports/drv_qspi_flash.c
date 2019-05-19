/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-08-17     zylx         first implementation.
 */
 
#include <board.h>
#include <drv_qspi.h>
#include <rtdevice.h>
#include <rthw.h>
#include <finsh.h>

// #ifdef BSP_USING_FLASH

#include "spi_flash.h"
#include "spi_flash_sfud.h"

char n25qxx_read_status_register2(struct rt_qspi_device *device)
{
    /* 0x35 read status register2 */
    char instruction = 0x35, status;

    rt_qspi_send_then_recv(device, &instruction, 1, &status, 1);

    return status;
}

void n25qxx_write_enable(struct rt_qspi_device *device)
{
    /* 0x06 write enable */
    char instruction = 0x06;

    rt_qspi_send(device, &instruction, 1);
}


void n25qxx_enter_qspi_mode(struct rt_qspi_device *device)
{
    char status = 0;
    /* 0x38 enter qspi mode */
    char instruction = 0x38;
    char write_status2_buf[2] = {0};

    /* 0x31 write status register2 */
    write_status2_buf[0] = 0x31;

    status = n25qxx_read_status_register2(device);
    if (!(status & 0x02))
    {
        status |= 1 << 1;
        n25qxx_write_enable(device);
        write_status2_buf[1] = status;
        rt_qspi_send(device, &write_status2_buf, 2);
        rt_qspi_send(device, &instruction, 1);
        rt_kprintf("flash already enter qspi mode\n");
        rt_thread_mdelay(10);
    }
}


static int rt_hw_qspi_flash_with_sfud_init(void)
{
    stm32_qspi_bus_attach_device("qspi1", "qspi10", RT_NULL, 4, n25qxx_enter_qspi_mode, RT_NULL);
    
    // /* init n25q128 */
    // if (RT_NULL == rt_sfud_flash_probe("MX25R6435F", "qspi10"))
    // {
    //     return -RT_ERROR;
    // }

    return RT_EOK;
}
INIT_COMPONENT_EXPORT(rt_hw_qspi_flash_with_sfud_init);


void flash_test(void)
{
    static rt_device_t flash_dev;
    rt_err_t res = RT_EOK;
    rt_size_t size=0;
    uint8_t readed[30]={0};
    flash_dev  = rt_device_find("MX25R6435F");
    if(!flash_dev)
    {
        rt_kprintf("find %s failed!\n", "MX25R6435F");
        return ;
    }
    res = rt_device_init(flash_dev);
    if (res != RT_EOK)
    {
        rt_kprintf("initialize %s failed!\n", "MX25R6435F");
        return ;
    }
    res = rt_device_open(flash_dev, RT_DEVICE_OFLAG_RDWR );
        if (res != RT_EOK)
    {
        rt_kprintf("open %s failed!\n", "MX25R6435F");
        return ;
    }
    size =  rt_device_write(flash_dev, 0,"0123", sizeof("0123"));
    rt_kprintf("The size is %d\n", size);
    if(size > 0)
    {
         rt_device_read(flash_dev, 0,(uint8_t *)readed, 8);
         rt_kprintf("The readed:%s\n",readed);
    }
    rt_kprintf("Test finish\n");
    rt_device_close(flash_dev);
}
FINSH_FUNCTION_EXPORT(flash_test, test flash);
MSH_CMD_EXPORT(flash_test, test flash);
// INIT_COMPONENT_EXPORT(rt_hw_qspi_flash_with_sfud_init);

// #endif/* BSP_USING_FLASH */
