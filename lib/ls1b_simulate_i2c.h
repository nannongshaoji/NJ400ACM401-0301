// ģ��i2c��ͷ�ļ�



#ifndef __OPENLOONGSON_SIMULATE_H
#define __OPENLOONGSON_SIMULATE_H


// ģ��i2c�Ľӿ���Ϣ
typedef struct
{
    unsigned int scl_gpio;          // SCL����gpio����
    unsigned int sda_gpio;          // SDA����gpio����
    int delay_time;                 // ���ڵ�1/2����λus
}simulate_i2c_t;



/*
 * ģ��i2c��ʼ��
 * @i2c_info i2c�Ľӿ���Ϣ
 */
void simulate_i2c_init(simulate_i2c_t *i2c_info);


/*
 * ģ��I2C�Ŀ�ʼ
 * @i2c_info i2c�ӿ���Ϣ
 */
void simulate_i2c_start(simulate_i2c_t *i2c_info);


/*
 * ģ��I2C��ֹͣ
 * @i2c_info i2c�ӿ���Ϣ
 */
void simulate_i2c_stop(simulate_i2c_t *i2c_info);


/*
 * �����豸����һ��ackӦ���ź�
 * @i2c_info i2c�ӿ���Ϣ
 */
void simulate_i2c_send_ack(simulate_i2c_t *i2c_info);


/*
 * �����豸����һ��no ack��Ӧ���ź�
 * @i2c_info i2c�ӿ���Ϣ
 */
void simulate_i2c_send_no_ack(simulate_i2c_t *i2c_info);


/*
 * ��ȡ���豸��ackӦ���ź�
 * @i2c_info i2c�ӿ���Ϣ
 * @ret ��ȡ�����źš�0��ʾӦ��1��ʾ��Ӧ��
 */
unsigned int simulate_i2c_read_ack(simulate_i2c_t *i2c_info);


/*
 * ���豸�Ӵ��豸�����ȡһ��8bit����
 * @i2c_info i2c�ӿ���Ϣ
 * @ret ��ȡ������
 */
unsigned char simulate_i2c_read_byte(simulate_i2c_t *i2c_info);


/*
 * ���豸д8bit���ݵ����豸
 * @i2c_info i2c�ӿ���Ϣ
 * @data ��д����
 */
void simulate_i2c_write_byte(simulate_i2c_t *i2c_info, unsigned char data);



#endif

