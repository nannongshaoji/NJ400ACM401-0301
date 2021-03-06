#ifndef SJA1000_DEV_H
#define SJA1000_DEV_H

#define SJA1000_ECHO_SKB_MAX	1 	/* the SJA1000 has one TX buffer object */
#define SJA1000_MAX_IRQ 		20	/* max. number of interrupts handled in ISR */

/* SJA1000 registers - manual section 6.4 (Pelican Mode) */
#define REG_MOD		0x00
#define REG_CMR		0x01
#define REG_SR		0x02
#define REG_IR		0x03
#define REG_IER		0x04
#define REG_ALC		0x0B
#define REG_ECC		0x0C
#define REG_EWL		0x0D
#define REG_RXERR	0x0E
#define REG_TXERR	0x0F
#define REG_ACCC0	0x10
#define REG_ACCC1	0x11
#define REG_ACCC2	0x12
#define REG_ACCC3	0x13
#define REG_ACCM0	0x14
#define REG_ACCM1	0x15
#define REG_ACCM2	0x16
#define REG_ACCM3	0x17
#define REG_DATA4	0x18
#define REG_DATA5	0x19
#define REG_DATA6	0x1A
#define REG_DATA7	0x1B
#define REG_DATA8	0x1C

#define REG_RMC		0x1D
#define REG_RBSA	0x1E

#define REG_CDR		0x1F

/* Common registers - manual section 6.5 */
#define REG_BTR0	0x06
#define REG_BTR1	0x07
#define REG_OCR		0x08
#define REG_CDR		0x1F

#define REG_FI		0x10
#define SFF_BUF		0x13
#define EFF_BUF		0x15

#define FI_FF		0x80
#define FI_RTR		0x40

#define REG_ID1		0x11
#define REG_ID2		0x12
#define REG_ID3		0x13
#define REG_ID4		0x14

#define CAN_RAM		0x20

/* mode register */
#define MOD_RM		0x01
#define MOD_LOM		0x02
#define MOD_STM		0x04
#define MOD_AFM		0x08
#define MOD_SM		0x10

/* commands */
#define CMD_SRR		0x10
#define CMD_CDO		0x08
#define CMD_RRB		0x04
#define CMD_AT		0x02
#define CMD_TR		0x01

/* interrupt sources */
#define IRQ_BEI		0x80
#define IRQ_ALI		0x40
#define IRQ_EPI		0x20
#define IRQ_WUI		0x10
#define IRQ_DOI		0x08
#define IRQ_EI		0x04
#define IRQ_TI		0x02
#define IRQ_RI		0x01
#define IRQ_ALL		0xFF
#define IRQ_OFF		0x00

/* status register content */
#define SR_BS		0x80
#define SR_ES		0x40
#define SR_TS		0x20
#define SR_RS		0x10
#define SR_TCS		0x08
#define SR_TBS		0x04
#define SR_DOS		0x02
#define SR_RBS		0x01

#define SR_CRIT (SR_BS|SR_ES)

/* ECC register */
#define ECC_SEG		0x1F
#define ECC_DIR		0x20
#define ECC_ERR		6
#define ECC_BIT		0x00
#define ECC_FORM	0x40
#define ECC_STUFF	0x80
#define ECC_MASK	0xc0



#endif /* SJA1000_DEV_H */
