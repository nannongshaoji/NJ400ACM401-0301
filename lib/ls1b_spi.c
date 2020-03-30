#include <string.h>
#include "ls1b_public.h"
#include "ls1b_regs.h"
#include "ls1b_clock.h"
#include "ls1b_spi.h"
#include "ls1b_delay.h"

// �Ĵ���ƫ��
#define LS1B_SPI_SPCR_OFFSET                (0)     // ���ƼĴ���
#define LS1B_SPI_SPSR_OFFSET                (1)     // ״̬�Ĵ���
#define LS1B_SPI_TxFIFO_OFFSET              (2)     // ���͵����ݼĴ�������������ݼĴ�����ƫ����ͬ
#define LS1B_SPI_RxFIFO_OFFSET              (2)     // ���յ����ݼĴ������뷢�����ݼĴ�����ƫ����ͬ
#define LS1B_SPI_SPER_OFFSET                (3)     // �ⲿ�Ĵ���
#define LS1B_SPI_SFC_PARAM_OFFSET           (4)     // �������ƼĴ���
#define LS1B_SPI_SFC_SOFTCS_OFFSET          (5)     // Ƭѡ���ƼĴ���
#define LS1B_SPI_SFC_TIMING_OFFSET          (6)     // ʱ����ƼĴ���

// �Ĵ���SPCR�е�λ��
#define LS1B_SPI_SPCR_SPIE_BIT              (7)								//�ж����ʹ��λ������Ч
#define LS1B_SPI_SPCR_SPIE_MASK             (0x01 << LS1B_SPI_SPCR_SPIE_BIT)
#define LS1B_SPI_SPCR_SPE_BIT               (6)								//SPIʹ��λ������Ч
#define LS1B_SPI_SPCR_SPE_MASK              (0x01 << LS1B_SPI_SPCR_SPE_BIT)
#define LS1B_SPI_SPCR_CPOL_BIT              (3)								//ʱ�Ӽ���λ
#define LS1B_SPI_SPCR_CPOL_MASK             (0x01 << LS1B_SPI_SPCR_CPOL_BIT)
#define LS1B_SPI_SPCR_CPHA_BIT              (2)								//ʱ����λλ
#define LS1B_SPI_SPCR_CPHA_MASK             (0x01 << LS1B_SPI_SPCR_CPHA_BIT)
#define LS1B_SPI_SPCR_SPR_BIT               (0)								//��Ƶ����
#define LS1B_SPI_SPCR_SPR_MASK              (0x03 << LS1B_SPI_SPCR_SPR_BIT)

// �Ĵ���SPSR�е�λ��
#define LS1B_SPI_SPSR_SPIF_BIT              (7)								//�жϱ�־λ
#define LS1B_SPI_SPSR_SPIF_MASK             (0x01 << LS1B_SPI_SPSR_SPIF_BIT)
#define LS1B_SPI_SPSR_WCOL_BIT              (6)								//д�Ĵ��������־λ
#define LS1B_SPI_SPSR_WCOL_MASK             (0x01 << LS1B_SPI_SPSR_WCOL_BIT)

// �Ĵ���SPER�е�λ��
#define LS1B_SPI_SPER_SPRE_BIT              (0)								//��Ƶ����
#define LS1B_SPI_SPER_SPRE_MASK             (0x3 << LS1B_SPI_SPER_SPRE_BIT)

// �Ĵ���SFC_SOFTCS��λ��
#define LS1B_SPI_SFC_SOFTCS_CSN_BIT         (4)										//csn�������ֵ
#define LS1B_SPI_SFC_SOFTCS_CSN_MASK        (0x0f << LS1B_SPI_SFC_SOFTCS_CSN_BIT)
#define LS1B_SPI_SFC_SOFTCS_CSEN_BIT        (0)										//can����ʹ��λ
#define LS1B_SPI_SFC_SOFTCS_CSEN_MASK       (0x0f << LS1B_SPI_SFC_SOFTCS_CSEN_BIT)

// ���ͳ�ʱ������ֵ
#define LS1B_SPI_TX_TIMEOUT                 (20000)


/*
 * ��ȡָ��SPIģ��Ļ���ַ
 * @SPIx SPIģ��ı��
 */
inline void *spi_get_base(ls1b_spi_t SPIx)
{
    void *base = NULL;

    switch (SPIx)
    {
        case LS1B_SPI_0:
            base = (void *)LS1B_SPI0_BASE;
            break;

        case LS1B_SPI_1:
            base = (void *)LS1B_SPI1_BASE;
            break;

        default:
            base = NULL;
            break;
    }

    return base;
}


/*
 * ��ӡָ��SPIģ������мĴ�����ֵ
 * @spi_info_p SPIģ����Ϣ
 */
void spi_print_all_regs_info(ls1b_spi_info_t *spi_info_p)
{
    void *spi_base = spi_get_base(spi_info_p->SPIx);

    myprintf("[%s] SPI%d's info:\r\n PCR=0x%x, SPSR=0x%x, SPER=0x%x, SFC_PARAM=0x%x, SFC_SOFTCS=0x%x, SFC_TIMING=0x%x\r\n",             
              __FUNCTION__, spi_info_p->SPIx,
              reg_read_8(spi_base + LS1B_SPI_SPCR_OFFSET),
              reg_read_8(spi_base + LS1B_SPI_SPSR_OFFSET),
              reg_read_8(spi_base + LS1B_SPI_SPER_OFFSET),
              reg_read_8(spi_base + LS1B_SPI_SFC_PARAM_OFFSET),
              reg_read_8(spi_base + LS1B_SPI_SFC_SOFTCS_OFFSET),
              reg_read_8(spi_base + LS1B_SPI_SFC_TIMING_OFFSET));

    return ;
}


/*
 * ����SPIʱ��Ƶ�ʼ����Ƶϵ��
 * @max_speed_hz SPI���ͨ���ٶ�
 * @ret ��Ƶϵ��
 */
unsigned int spi_get_div(unsigned int max_speed_hz)
{
    unsigned long clk = 0;
    unsigned int div = 0;
    unsigned int div_tmp = 0;
    unsigned int bit = 0;

    clk = clk_get_apb_rate();
    div = DIV_ROUND_UP(clk, max_speed_hz);

    if (div < 2)
        div = 2;

    if (div > 4096)
        div = 4096;

    bit = fls(div) - 1;
    switch (1 << bit)
    {
        case 16:
            div_tmp = 2;
            if (div > (1 << bit))
            {
                div_tmp++;
            }
            break;

        case 32:
            div_tmp = 3;
            if (div > (1 << bit))
            {
                div_tmp += 2;
            }
            break;

        case 8:
            div_tmp = 4;
            if (div > (1 << bit))
            {
                div_tmp -= 2;
            }
            break;

        default:
            div_tmp = bit - 1;
            if (div > (1 << bit))
            {
                div_tmp++;
            }
            break;
    }
/*    
    myprintf("[%s] clk=%ld, max_speed_hz=%d, div_tmp=%d, bit=%d\r\n", 
              __FUNCTION__, clk, max_speed_hz, div_tmp, bit);
*/
    return div_tmp;
}


/*
 * ����ʱ��
 * @spi_info_p SPIģ����Ϣ
 */
void spi_set_clock(ls1b_spi_info_t *spi_info_p)
{
    void *spi_base = spi_get_base(spi_info_p->SPIx);
    unsigned int div = 0;
    unsigned char val = 0;

    // ��ȡ��Ƶϵ��
    div = spi_get_div(spi_info_p->max_speed_hz);

    // ����spr
    val = reg_read_8(spi_base + LS1B_SPI_SPCR_OFFSET);
    val &= (~LS1B_SPI_SPCR_SPR_MASK);                       // spr����
    val |= (div & LS1B_SPI_SPCR_SPR_MASK);                  // �����µ�spr
    reg_write_8(val, spi_base + LS1B_SPI_SPCR_OFFSET);

    // ����spre
    val = reg_read_8(spi_base + LS1B_SPI_SPER_OFFSET);
    val &= (~LS1B_SPI_SPER_SPRE_MASK);                      // spre����
    val |= ((div >> 2) & LS1B_SPI_SPER_SPRE_MASK);        	// �����µ�spre
    reg_write_8(val, spi_base + LS1B_SPI_SPER_OFFSET);

    return ;
}


/*
 * ����ͨ��ģʽ(ʱ�Ӽ��Ժ���λ)
 * @spi_info_p SPIģ����Ϣ
 */
void spi_set_mode(ls1b_spi_info_t *spi_info_p)
{
    void *spi_base = spi_get_base(spi_info_p->SPIx);
    unsigned char val = 0;

    val = reg_read_8(spi_base + LS1B_SPI_SPCR_OFFSET);
    
    // ����ʱ�Ӽ���--cpol
    val &= (~LS1B_SPI_SPCR_CPOL_MASK);                      // cpol��0
    val |= (spi_info_p->cpol << LS1B_SPI_SPCR_CPOL_BIT);    // д���µ�cpol
    
    // ����ʱ����λ--cpha
    val &= (~LS1B_SPI_SPCR_CPHA_MASK);                      // cpha��0
    val |= (spi_info_p->cpha << LS1B_SPI_SPCR_CPHA_BIT);    // д���µ�cpha
    
    reg_write_8(val, spi_base + LS1B_SPI_SPCR_OFFSET);

    return ;
}


/*
 * ����ָ��ƬѡΪָ��״̬
 * @spi_info_p SPIģ����Ϣ
 * @new_status Ƭѡ���ŵ���״̬��ȡֵΪ0��1�����ߵ�ƽ��͵�ƽ
 */
void spi_set_cs(ls1b_spi_info_t *spi_info_p, int new_status)
{
    void *spi_base = spi_get_base(spi_info_p->SPIx);
    unsigned char cs = spi_info_p->cs;
    unsigned char val = 0;

    val = reg_read_8(spi_base + LS1B_SPI_SFC_SOFTCS_OFFSET);
    
    if (new_status)         		// cs = 1
    {
        val |= (0x01 << cs);          	// ָ����csen=1
        val |= (0x10 << cs);        // ָ��csn=1
    }
    else                    		// cs = 0
    {
        val &= ~(0x10 << cs);       // ָ��csn=0
        val |= 0x0f;
    }
    reg_write_8(val, spi_base + LS1B_SPI_SFC_SOFTCS_OFFSET);

    return ;
}


/*
 * ��ʼ��ָ��SPIģ��
 * @spi_info_p SPIģ����Ϣ
 */
void spi_init(ls1b_spi_info_t *spi_info_p)
{
    void *spi_base = spi_get_base(spi_info_p->SPIx);
    unsigned char val = 0;

    // ʹ��SPI��������masterģʽ���ر��ж�
    reg_write_8(0x53, spi_base + LS1B_SPI_SPCR_OFFSET);

    // ���״̬�Ĵ���
    reg_write_8(0xc0, spi_base + LS1B_SPI_SPSR_OFFSET);

    // 1�ֽڲ����жϣ�����(��)�뷢��(д)ʱ��ͬʱ
    reg_write_8(0x03, spi_base + LS1B_SPI_SPER_OFFSET);//0x03ͬʱ 0x07��������

    // �ر�SPI flash
    val = reg_read_8(spi_base + LS1B_SPI_SFC_PARAM_OFFSET);
    val &= 0xfe;
    reg_write_8(val, spi_base + LS1B_SPI_SFC_PARAM_OFFSET);

    // spi flashʱ����ƼĴ���
    reg_write_8(0x07, spi_base + LS1B_SPI_SFC_TIMING_OFFSET);

    // ����ʱ��
    spi_set_clock(spi_info_p);

    // ����ͨ��ģʽ(ʱ�Ӽ��Ժ���λ)
    spi_set_mode(spi_info_p);

    // ��ӡ�Ĵ�����Ϣ(���ڵ���)
    //spi_print_all_regs_info(spi_info_p);

    return ;
}


/*
 * �ȴ��շ����
 */
inline void spi_wait_txrx_done(ls1b_spi_info_t *spi_info_p)
{
    void *spi_base = spi_get_base(spi_info_p->SPIx);
    int timeout = LS1B_SPI_TX_TIMEOUT;

    while (timeout--)
    {
        if (LS1B_SPI_SPSR_SPIF_MASK & reg_read_8(spi_base + LS1B_SPI_SPSR_OFFSET))
            break;
    }
    
    return ;
}


/*
 * ���жϺͱ�־λ
 */
inline void spi_clear(ls1b_spi_info_t *spi_info_p)
{
    void *spi_base = spi_get_base(spi_info_p->SPIx);
    unsigned char val = 0;

    // ���ж�
    val = reg_read_8(spi_base + LS1B_SPI_SPSR_OFFSET);
    val |= LS1B_SPI_SPSR_SPIF_MASK;
    reg_write_8(val, spi_base + LS1B_SPI_SPSR_OFFSET);

    // �������־λ(Write-Collision Clear)
    val = reg_read_8(spi_base + LS1B_SPI_SPSR_OFFSET);
    if (LS1B_SPI_SPSR_WCOL_MASK & val)
    {
        //printf("[%s] clear register SPSR's wcol!\r\n");       // �ֲ��linuxԴ���в�һ�����Ӹ���ӡ����
        reg_write_8(val & ~LS1B_SPI_SPSR_WCOL_MASK, spi_base + LS1B_SPI_SPSR_OFFSET);   // д0��linuxԴ������д0
		//reg_write_8(val | LS1B_SPI_SPSR_WCOL_MASK, spi_base + LS1B_SPI_SPSR_OFFSET);  // д1������1c�ֲᣬӦ��д1
    }

    return ;
}



/*
 * ͨ��ָ��SPI���ͽ���һ���ֽ�
 * ע�⣬�ڶ������ϵͳ�У��˺�����Ҫ���⡣
 * ����֤�ں�ĳ�����豸�շ�ĳ���ֽڵĹ����У����ܱ��л�����������ͬʱ���������ͬһ��SPI�����ϵĴ��豸ͨ��
 * ��Ϊ��о1c��ÿ·SPI�Ͽ��ܽ��в�ͬ�Ĵ��豸��ͨ��Ƶ�ʡ�ģʽ�ȿ��ܲ�ͬ
 * @spi_info_p SPI�ӿ�
 * @tx_ch �����͵�����
 * @ret �յ�������
 */
unsigned char spi_txrx_byte(ls1b_spi_info_t *spi_info_p, unsigned char tx_ch)
{
    void *spi_base = spi_get_base(spi_info_p->SPIx);
    unsigned char rx_ch = 0;

    // �շ�����
    reg_write_8(tx_ch, spi_base + LS1B_SPI_TxFIFO_OFFSET);      // ��ʼ����
    spi_wait_txrx_done(spi_info_p);                             // �ȴ��շ����
    rx_ch = reg_read_8(spi_base + LS1B_SPI_RxFIFO_OFFSET);      // ��ȡ�յ�������
    spi_clear(spi_info_p);                                      // ���жϺͱ�־λ

    return rx_ch;
}



/**
 * fls - find last (most-significant) bit set
 * @x: the word to search
 *
 * This is defined the same way as ffs.
 * Note fls(0) = 0, fls(1) = 1, fls(0x80000000) = 32.
 */

inline int fls(int x)
{
	int r = 32;

	if (!x)
		return 0;
	if (!(x & 0xffff0000u)) {
		x <<= 16;
		r -= 16;
	}
	if (!(x & 0xff000000u)) {
		x <<= 8;
		r -= 8;
	}
	if (!(x & 0xf0000000u)) {
		x <<= 4;
		r -= 4;
	}
	if (!(x & 0xc0000000u)) {
		x <<= 2;
		r -= 2;
	}
	if (!(x & 0x80000000u)) {
		x <<= 1;
		r -= 1;
	}
	return r;
}


