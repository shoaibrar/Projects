/*
 * This Code is a Direct implementation of SPI protocol to read data from LIS3DSH only.
 * LIS3DSH is present in STM32F407 Discovery Kit developer board as MEMS.
 * This is a Baremetal code - Modifying and accessing Registers Using the address respectively.
 *
 */



// Including Header files
#include "stm32f407xx.h"
#include "main.h"


// Configuring SPI1 Peripheral to connect with LIS3DSH.
// The Discovery board is set as Master mode.
// PA5 PA6 PA7 and PE3 pins are used. PE3 pins is set as the Chip Select pin.
// Pin Configuration given in STM32f407 Discovery Kit Schematics - MEMS
void spiconfig(void){

	RCC->AHB1ENR |= (1<<0)|(1<<4); //GPIO A and E clock enable

	GPIOA->MODER |= (2<<10)|(2<<12)|(2<<14); //Configuring GPIO A5,A6,A7 and E3 pins as output
	GPIOE->MODER |= (1<<6);

	GPIOA->OSPEEDR |=(3<<10)|(3<<12)|(3<<14); // Setting the pins speed very high
	GPIOE->OSPEEDR |= (3<<7);

	GPIOA->PUPDR |=(1<<10)|(1<<12)|(1<<14);  // Setting the pins as Pull up
	GPIOE->PUPDR |=(1<<6);

	GPIOA->AFR[0] |= (5<<20)|(5<<24)|(5<<28); // GPIOA pins to Alternate function for spi1


	RCC->APB2ENR |= (1<<12); //SPI1 CLK ENABLE

	// SPI1 CPHA and CPOL = 1, Set Master mode, SSI and SSM = 1, Baud control at F/32
	SPI1->CR1 |= (1<<0)|(1<<1)|(1<<2)|(1<<8)|(1<<9)|(4<<3);

	GPIOE->ODR |= (1<<3); // Setting GPIOE 3rd pin High

	SPI1->CR1 |=(1<<6); // Enabling SPI1


}


// Configuring the LED to output mode
void gpioconfig(void){

	RCC->AHB1ENR |=(1<<3);

	GPIOD->MODER |= (1<<24)|(1<<26)|(1<<28)|(1<<30);
	GPIOD->OSPEEDR |= (0XFF<<24);

}


// Function to perform SPI write
// Arguments are the register address for the slave device and data to write at the register
void spiWR(uint8_t reg, uint8_t data){

	uint8_t garb = 0; // Garbage variable to read from buffer

	GPIOE->ODR &= ~(1<<3); // Chip Select pin PE3 set to low. Initializing the Communication
	while(!(SPI1->SR & (1<<1))); // Check if Tx Buffer flag is empty. Send Data only when its empty

	SPI1->DR = reg; // Load the register address to the Buffer.

	while(!(SPI1->SR & (1<<0))); // Check if Rx Buffer flag is not empty. Read data when not empty

	garb = SPI1->DR; // Read garbage value from slave

	while(!(SPI1->SR & (1<<1))); // Again check for Tx buffer flag

	SPI1->DR = data; // Load the data that needs to be written

	while(!(SPI1->SR & (1<<0))); // Check Rx buffer flag

	garb = SPI1->DR; // Garbage value

	while(SPI1->SR & (1<<7)); // Check for busy status

	GPIOE->ODR |= (1<<3); // PE3 pin set high Indicating end of communication.

}

//Function to Read data
// Arguments passed are the register address from where data to be read
int8_t spiRD(uint8_t reg){

	int8_t val = 0; // Variable to read the data
	reg |= 0x80; // Setting the MSB bit high Indicating Read process to slave
	GPIOE->ODR &= ~(1<<3); // Enable CS pin
	while(!(SPI1->SR & (1<<1)));// Check Tx buffer

	SPI1->DR = reg; // Load Register address
	while(!(SPI1->SR & (1<<0))); // Check Rx buffer

	val = SPI1->DR; // Read Garbage Value
	while(!(SPI1->SR & (1<<1))); // Check Tx buffer

	SPI1->DR = 0; // Send empty data
	while(!(SPI1->SR & (1<<0))); // Check Rx buffer
	while(SPI1->SR & (1<<7)); // Check Busy

	GPIOE->ODR |= (1<<3); // Disable CS pin
	val = SPI1->DR; // Read the data from the register
	return val; // Return the value
}

// Clock Configuration to run at desired frequency
void clkconfig(void){

	RCC->APB1ENR |= (1<<28);

	PWR->CR |= (1<<14);

	FLASH->ACR |= (5<<0)|(1<<8)|(1<<9)|(1<<10);

	RCC->CR |= (1<<16);
	while(!(RCC->CR & (1<<17)));

	RCC->PLLCFGR |= (8<<0)|(336<<6)|(7<<24)|(1<<22);
	RCC->CR |= (1<<24);
	while(!(RCC->CR & (1<<25)));

	RCC->CFGR |= (5<<10)|(4<<13)|(2<<0);
	while(!(RCC->CFGR & (2<<2)));

	RCC->CR &= ~(1<<0);

}

// Main function

int main(void){
	// Initializing variable to read the x and y axis data
	int8_t y,x;
	// Calling the Initialization function
	clkconfig();
	gpioconfig();
	spiconfig();

	// Setting the CTRL_REG4 register of LIS3DSH to  1600 Hz frequency and enable X Y Z axis
	spiWR(0x20,0x97);
  
	// Just turning the LED ON and OFF
	GPIOD->ODR |=(0xf<<12);
	for(int i =0; i<1000000;i++);
	GPIOD->ODR &= ~(0xf<<12);

	// Reading the X and Y axis data Indefinitely
	while(1){
		y = spiRD((int8_t)0x29); // X axis data High Register address 0xx29
		x = spiRD((int8_t)0x2B); // Y axis data High Register address 0xx2B


	}

	return 0;
}


