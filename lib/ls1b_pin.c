// 引脚功能(普通gpio，pwm，复用等)相关接口


#include "ls1b_public.h"
#include "ls1b_regs.h"
#include "ls1b_gpio.h"
#include "ls1b_pin.h"


/*
 * 把指定pin设置为指定用途(普通gpio，非gpio)
 * @gpio gpio引脚编号
 * @purpose 用途
 */
void pin_set_purpose(unsigned int gpio, pin_purpose_t purpose)
{
    volatile unsigned int *gpio_cfgx;           // GPIO_CFGx寄存器
    unsigned int pin = GPIO_GET_PIN(gpio);

    gpio_cfgx = gpio_get_cfg_reg(gpio);
    if (PIN_PURPOSE_GPIO == purpose)            // 引脚用作普通gpio
    {
        reg_set_one_bit(gpio_cfgx, pin);
    }
    else                                        // 引脚用作其它功能(非gpio)
    {
        reg_clr_one_bit(gpio_cfgx, pin);
    }

    return ;
}



/*
 * 设置复用功能
 * @remap为复用代码
 */
void pin_set_remap(pin_remap_t remap)
{
    unsigned int high =remap/32;
    unsigned int low  = remap%32;

    switch (high)
    {
        case 0:
          reg_set_one_bit((volatile unsigned int *) LS1B_GPIO_MUX_CTRL0 , low);
            break;

        case 1:
          reg_set_one_bit((volatile unsigned int *) LS1B_GPIO_MUX_CTRL1 , low);
            break;

        default:
            return ;
    }

    return ;
}



