// ���Ź���(��ͨgpio��pwm�����õ�)��ؽӿ�


#include "ls1b_public.h"
#include "ls1b_regs.h"
#include "ls1b_gpio.h"
#include "ls1b_pin.h"


/*
 * ��ָ��pin����Ϊָ����;(��ͨgpio����gpio)
 * @gpio gpio���ű��
 * @purpose ��;
 */
void pin_set_purpose(unsigned int gpio, pin_purpose_t purpose)
{
    volatile unsigned int *gpio_cfgx;           // GPIO_CFGx�Ĵ���
    unsigned int pin = GPIO_GET_PIN(gpio);

    gpio_cfgx = gpio_get_cfg_reg(gpio);
    if (PIN_PURPOSE_GPIO == purpose)            // ����������ͨgpio
    {
        reg_set_one_bit(gpio_cfgx, pin);
    }
    else                                        // ����������������(��gpio)
    {
        reg_clr_one_bit(gpio_cfgx, pin);
    }

    return ;
}



/*
 * ���ø��ù���
 * @remapΪ���ô���
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



