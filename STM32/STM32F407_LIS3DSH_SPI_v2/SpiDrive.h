/*
*  This is a Header file for SPI driver
*  This is NOT A COMPLETE SPI DRIVER HEADER FILE
*  This file contain only the necessary declaration to setup SPI
*
*/

#ifndef INC_SPIDRIVE_H_
#define INC_SPIDRIVE_H_

#include "stm32f407xx.h"

//This struct is used to cofigure the SPI
typedef struct{
	uint8_t SpiMode;
	uint8_t SpiBus;
	uint8_t SpiClk;
	uint8_t SpiCPHA;
	uint8_t SpiCPOL;
	uint8_t SpiSSI;
	uint8_t SpiSSM;
	uint8_t SpiDFF;
	uint8_t SpiLSB;
}config_spi;

// Macros for SPI mode
#define SpiMode_master				1
#define SpiMode_slave				0

// Macros for SPI Full duplex or Half Duplex
#define SpiBus_fullDup	1
#define SpiBus_halfDup	0

// Macros for SPI baud rate
#define SpiClk_div2	0
#define SpiClk_div4	1
#define SpiClk_div32	4

// Macros for SPI CPHA
#define SpiCPHA_low	0
#define SpiCPHA_high	1

// Macros for SPI CPOL
#define SpiCPOL_low	0
#define SpiCPOL_high	1

// Macros for SPI SSM
#define SpiSSM_disable	0
#define SpiSSM_enable	1

// Macros for SPI SSI
#define SpiSSI_disable	0
#define SpiSSI_enable	1

// Macros for SPI Data format
#define SpiDFF_8	0
#define SpiDFF_16	1

// Macros for SPI bit order
#define SpiLSB_MSB	0
#define SpiLSB_LSB	1

// Macros for SPI Eable
#define SPI_ENABLE 1
#define SPI_DISABLE 0

// Declaring the Functions
//Only necessary functions are declared
void spi_init(SPI_RegDef_t *SPIx, config_spi *SPI_configer,GPIO_TypeDef *gpiox, uint16_t pinVAL);
void spi_write(SPI_RegDef_t *SPIx,uint8_t reg, uint8_t data,GPIO_TypeDef *gpiox, uint16_t pinVAL);
int8_t spiReaD(SPI_RegDef_t *SPIx,uint8_t reg,GPIO_TypeDef *gpiox, uint16_t pinVAL);

#endif /* INC_SPIDRIVE_H_ */
