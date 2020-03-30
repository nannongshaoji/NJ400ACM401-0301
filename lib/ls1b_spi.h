#ifndef __LS1B_SPI_H
#define __LS1B_SPI_H


// SPIģ����
typedef enum
{
    LS1B_SPI_0 = 0,
    LS1B_SPI_1,
}ls1b_spi_t;


// Ƭѡ
#define LS1B_SPI_INVALID_CS             (-1)    //
#define LS1B_SPI_CS_0                   (0)     //GPIO27
#define LS1B_SPI_CS_1                   (1)     //GPIO28
#define LS1B_SPI_CS_2                   (2)     //GPIO29
#define LS1B_SPI_CS_3                   (3)     //GPIO30

// ʱ�Ӽ��Ժ���λ
#define SPI_CPOL_1                      (1)
#define SPI_CPOL_0                      (0)
#define SPI_CPHA_1                      (1)
#define SPI_CPHA_0                      (0)


// Ӳ��SPI��Ϣ
typedef struct
{
    ls1b_spi_t SPIx;                    // SPIģ����
    unsigned long max_speed_hz;         // ���ͨ���ٶȣ���λhz
    unsigned char cs;                   // Ƭѡ
    unsigned char cpol;                 // ʱ�Ӽ���
    unsigned char cpha;                 // ʱ����λ
}ls1b_spi_info_t;



/*
 * ��ʼ��ָ��SPIģ��
 * @spi_info_p SPIģ����Ϣ
 */
void spi_init(ls1b_spi_info_t *spi_info_p);


/*
 * ����ָ��ƬѡΪָ��״̬
 * @spi_info_p SPIģ����Ϣ
 * @new_status Ƭѡ���ŵ���״̬��ȡֵΪ0��1�����ߵ�ƽ��͵�ƽ
 */
void spi_set_cs(ls1b_spi_info_t *spi_info_p, int new_status);


/*
 * ͨ��ָ��SPI���ͽ���һ���ֽ�
 * ע�⣬�ڶ������ϵͳ�У��˺�����Ҫ���⡣
 * ����֤�ں�ĳ�����豸�շ�ĳ���ֽڵĹ����У����ܱ��л�����������ͬʱ���������ͬһ��SPI�����ϵĴ��豸ͨ��
 * ��Ϊ��о1c��ÿ·SPI�Ͽ��ܽ��в�ͬ�Ĵ��豸��ͨ��Ƶ�ʡ�ģʽ�ȿ��ܲ�ͬ
 * @spi_info_p SPI�ӿ�
 * @tx_ch �����͵�����
 * @ret �յ�������
 */
unsigned char spi_txrx_byte(ls1b_spi_info_t *spi_info_p, unsigned char tx_ch);


/*
 * ��ӡָ��SPIģ������мĴ�����ֵ
 * @spi_info_p SPIģ����Ϣ
 */
void spi_print_all_regs_info(ls1b_spi_info_t *spi_info_p);



inline int fls(int x);


#endif


