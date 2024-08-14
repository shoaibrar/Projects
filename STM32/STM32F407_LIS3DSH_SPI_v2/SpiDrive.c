/*
 *  This is the C file for SPI driver
 *  Only necessary function are defined in this File
 *  Not all functions are defined
 */

//Including the SPI header file
#include "SpiDrive.h"

//Function to initialize the SPI
//This is a simple function that only performs the necessary setup
//This function does not configure all the setting of SPI

// The SPI used, SPI configuration using struct, the GPIO port used for clock and the GPIO pin used is passed as arguments
 void spi_init(spi_regi *SPIx, config_spi *SPI_configer,GPIO_TypeDef *gpiox, uint16_t pinVAL){

	SPIx->CR1 = 0;                  //Clearing the CR1 register of SPI
	SPIx->CR1 |= (SPI_configer->SpiCPOL<<0)|(SPI_configer->SpiCPHA<<1); //The SPI configurations are set
	SPIx->CR1 |= (SPI_configer->SpiMode <<2);                            // Values are passed accessig the struct
	SPIx->CR1 |= (SPI_configer->SpiClk <<3);
	SPIx->CR1 |= (SPI_configer->SpiSSI <<8)|(SPI_configer->SpiSSM <<9);;
	gpiox->ODR |= (1<<pinVAL);              // Clk pin is set high
	SPIx->CR1 |=(SPI_ENABLE<< 6);          // SPI enabled
};


//Function to Write data through SPI
//This is a simple function that only writes data for 8bit
// The SPI used, Register address, Data to be written, the GPIO port used for clock and the GPIO pin used is passed as arguments
 void spi_write(spi_regi *SPIx,uint8_t reg, uint8_t data,GPIO_TypeDef *gpiox, uint16_t pinVAL){
	uint8_t garb = 0; // Garbage variable to read from buffer

	gpiox->ODR &= ~(1<<pinVAL); // Chip Select pin PE3 set to low. Initializing the Communication
	while(!(SPIx->SR & (1<<1))); // Check if Tx Buffer flag is empty. Send Data only when its empty

	SPIx->DR = reg; // Load the register address to the Buffer.

	while(!(SPIx->SR & (1<<0))); // Check if Rx Buffer flag is not empty. Read data when not empty

	garb = SPIx->DR; // Read garbage value from slave

	while(!(SPIx->SR & (1<<1))); // Again check for Tx buffer flag

	SPIx->DR = data; // Load the data that needs to be written

	while(!(SPIx->SR & (1<<0))); // Check Rx buffer flag

	garb = SPIx->DR; // Garbage value

	while(SPIx->SR & (1<<7)); // Check for busy status

	gpiox->ODR |= (1<<pinVAL); // PE3 pin set high Indicating end of communication.

};

//Function to Read data through SPI
//This is a simple function that only read data for 8bit
// The SPI used, Register address, the GPIO port used for clock and the GPIO pin used is passed as arguments

 int8_t spiReaD(spi_regi *SPIx,uint8_t reg,GPIO_TypeDef *gpiox, uint16_t pinVAL){

 	int8_t val = 0; // Variable to read the data
 	reg |= 0x80; // Setting the MSB bit high Indicating Read process to slave
 	gpiox->ODR &= ~(1<<pinVAL); // Enable CS pin
 	while(!(SPIx->SR & (1<<1)));// Check Tx buffer

 	SPIx->DR = reg; // Load Register address
 	while(!(SPIx->SR & (1<<0))); // Check Rx buffer

 	val = SPIx->DR; // Read Garbage Value
 	while(!(SPIx->SR & (1<<1))); // Check Tx buffer

 	SPIx->DR = 0; // Send empty data
 	while(!(SPIx->SR & (1<<0))); // Check Rx buffer
 	while(SPIx->SR & (1<<7)); // Check Busy

 	gpiox->ODR |= (1<<pinVAL); // Disable CS pin
 	val = SPIx->DR; // Read the data from the register
 	return val; // Return the value
 }

