// ģ��i2c��Դ�ļ�


#include "ls1b_public.h"
#include "ls1b_gpio.h"
#include "ls1b_delay.h"
#include "ls1b_simulate_i2c.h"


/*
 * ����SCL����gpio����Ϊ���ģʽ
 * @i2c_info i2c�ӿ���Ϣ
 */
void simulate_i2c_config_scl_out(simulate_i2c_t *i2c_info)
{
    gpio_init(i2c_info->scl_gpio, gpio_mode_output);
    return ;
}


/*
 * ����SDA����gpio����Ϊ���ģʽ
 * @i2c_info i2c�ӿ���Ϣ
 */
void simulate_i2c_config_sda_out(simulate_i2c_t *i2c_info)
{
    gpio_init(i2c_info->sda_gpio, gpio_mode_output);
    return ;
}


/*
 * ����SDA����gpio����Ϊ����ģʽ
 * @i2c_info i2c�ӿ���Ϣ
 */
void simulate_i2c_config_sda_in(simulate_i2c_t *i2c_info)
{
    gpio_init(i2c_info->sda_gpio, gpio_mode_input);
    return ;
}


/*
 * SCL��������ߵ�ƽ
 * @i2c_info i2c�ӿ���Ϣ
 */
void simulate_i2c_scl_out_high(simulate_i2c_t *i2c_info)
{
    gpio_set(i2c_info->scl_gpio, gpio_level_high);
    return ;
}


/*
 * SCL��������͵�ƽ
 * @i2c_info i2c�ӿ���Ϣ
 */
void simulate_i2c_scl_out_low(simulate_i2c_t *i2c_info)
{
    gpio_set(i2c_info->scl_gpio, gpio_level_low);
    return ;
}


/*
 * SDA��������ߵ�ƽ
 * @i2c_info i2c�ӿ���Ϣ
 */
void simulate_i2c_sda_out_high(simulate_i2c_t *i2c_info)
{
    gpio_set(i2c_info->sda_gpio, gpio_level_high);
    return ;
}


/*
 * SDA��������͵�ƽ
 * @i2c_info i2c�ӿ���Ϣ
 */
void simulate_i2c_sda_out_low(simulate_i2c_t *i2c_info)
{
    gpio_set(i2c_info->sda_gpio, gpio_level_low);
    return ;
}


/*
 * ��ȡSDA����
 * @i2c_info i2c�ӿ���Ϣ
 * @ret SDA���ŵĵ�ƽֵ
 */
unsigned int simulate_i2c_sda_in(simulate_i2c_t *i2c_info)
{
    return gpio_get(i2c_info->sda_gpio);
}




/*
 * ģ��i2c��ʼ��
 * @i2c_info i2c�Ľӿ���Ϣ
 */
void simulate_i2c_init(simulate_i2c_t *i2c_info)
{
    // SCL����ߵ�ƽ
    simulate_i2c_config_scl_out(i2c_info);
    simulate_i2c_scl_out_high(i2c_info);
    
    return ;
}


/*
 * ģ��I2C�Ŀ�ʼ
 * @i2c_info i2c�ӿ���Ϣ
 */
void simulate_i2c_start(simulate_i2c_t *i2c_info)
{
    // SDA���ģʽ
    simulate_i2c_config_sda_out(i2c_info);

    // ���������Ҫһ��stop
    simulate_i2c_scl_out_high(i2c_info);
    delay_us(i2c_info->delay_time);
    simulate_i2c_sda_out_high(i2c_info);
    delay_us(2 * i2c_info->delay_time);

    // start
    simulate_i2c_sda_out_low(i2c_info);
    delay_us(i2c_info->delay_time);
    simulate_i2c_scl_out_low(i2c_info);
    delay_us(i2c_info->delay_time);

    return ;
}


/*
 * ģ��I2C��ֹͣ
 * @i2c_info i2c�ӿ���Ϣ
 */
void simulate_i2c_stop(simulate_i2c_t *i2c_info)
{
    // SDA���ģʽ
    simulate_i2c_config_sda_out(i2c_info);

    // �Ȱ�SCL��SDA����
    simulate_i2c_scl_out_low(i2c_info);
    delay_us(i2c_info->delay_time);
    simulate_i2c_sda_out_low(i2c_info);
    delay_us(i2c_info->delay_time);

    // stop
    simulate_i2c_scl_out_high(i2c_info);
    delay_us(i2c_info->delay_time);
    simulate_i2c_sda_out_high(i2c_info);
    delay_us(2 * i2c_info->delay_time);

    return ;
}


/*
 * �����豸����һ��ackӦ���ź�
 * @i2c_info i2c�ӿ���Ϣ
 */
void simulate_i2c_send_ack(simulate_i2c_t *i2c_info)
{
    // SDA���ģʽ
    simulate_i2c_config_sda_out(i2c_info);

    // SDA=0
    simulate_i2c_sda_out_low(i2c_info);
    delay_us(i2c_info->delay_time);

    // SCL����һ������
    simulate_i2c_scl_out_high(i2c_info);
    delay_us(i2c_info->delay_time);
    simulate_i2c_scl_out_low(i2c_info);
    delay_us(i2c_info->delay_time);

    return ;
}


/*
 * �����豸����һ��no ack��Ӧ���ź�
 * @i2c_info i2c�ӿ���Ϣ
 */
void simulate_i2c_send_no_ack(simulate_i2c_t *i2c_info)
{
    // SDA���ģʽ
    simulate_i2c_config_sda_out(i2c_info);

    // SDA=1
    simulate_i2c_sda_out_high(i2c_info);
    delay_us(i2c_info->delay_time);

    // SCL����һ������
    simulate_i2c_scl_out_high(i2c_info);
    delay_us(i2c_info->delay_time);
    simulate_i2c_scl_out_low(i2c_info);
    delay_us(i2c_info->delay_time);

    return ;
}


/*
 * ��ȡ���豸��ackӦ���ź�
 * @i2c_info i2c�ӿ���Ϣ
 * @ret ��ȡ�����źš�0��ʾӦ��1��ʾ��Ӧ��
 */
unsigned int simulate_i2c_read_ack(simulate_i2c_t *i2c_info)
{
    unsigned int ack = 1;
    
    // SDA����ģʽ���ͷ�SDA
    simulate_i2c_config_sda_in(i2c_info);

    delay_us(i2c_info->delay_time);
    simulate_i2c_scl_out_high(i2c_info);
    delay_us(i2c_info->delay_time);
    ack = simulate_i2c_sda_in(i2c_info);
    simulate_i2c_scl_out_low(i2c_info);
    delay_us(i2c_info->delay_time);

    return ack;
}


/*
 * ���豸�Ӵ��豸�����ȡһ��8bit����
 * @i2c_info i2c�ӿ���Ϣ
 * @ret ��ȡ������
 */
unsigned char simulate_i2c_read_byte(simulate_i2c_t *i2c_info)
{
    int i;
    unsigned char data = 0;

    // SDA����ģʽ
    simulate_i2c_config_sda_in(i2c_info);

    for (i=0; i<8; i++)
    {
        delay_us(i2c_info->delay_time);
        simulate_i2c_scl_out_high(i2c_info);
        delay_us(i2c_info->delay_time);

        // ��ȡһ��bit
        data <<= 1;
        if (gpio_level_high == simulate_i2c_sda_in(i2c_info))
            data |= 0x01;

        simulate_i2c_scl_out_low(i2c_info);
    }

    return data;
}


/*
 * ���豸д8bit���ݵ����豸
 * @i2c_info i2c�ӿ���Ϣ
 * @data ��д����
 */
void simulate_i2c_write_byte(simulate_i2c_t *i2c_info, unsigned char data)
{
    int i;

    // SDA���ģʽ
    simulate_i2c_config_sda_out(i2c_info);

    for (i=0; i<8; i++)
    {
        delay_us(i2c_info->delay_time);

        // дһ��bit
        if (data & 0x80)
            simulate_i2c_sda_out_high(i2c_info);
        else
            simulate_i2c_sda_out_low(i2c_info);

        delay_us(i2c_info->delay_time);
        simulate_i2c_scl_out_high(i2c_info);
        delay_us(i2c_info->delay_time);
        simulate_i2c_scl_out_low(i2c_info);

        data <<= 1;
    }
    
    delay_us(i2c_info->delay_time);

    return ;
}



