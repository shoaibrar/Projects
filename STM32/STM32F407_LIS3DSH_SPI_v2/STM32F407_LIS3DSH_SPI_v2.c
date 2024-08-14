/*
 * This Code is a implementation of SPI protocol by creating a Simple SPI Driver to read data from LIS3DSH only.
 * LIS3DSH is present in STM32F407 Discovery Kit developer board as MEMS.
 * This is a Baremetal code - Header and C file is created for SPI protocol and called in main for SPI communication .
 *
 */



// Including Header files
#include "stm32f407xx.h"
#include "main.h"
#include "SpiDrive.h"


// Configuring the LED to output mode
void gpioconfig(void){
	RCC->AHB1ENR |=(1<<3);
	GPIOD->MODER |= (1<<24)|(1<<26)|(1<<28)|(1<<30);
	GPIOD->OSPEEDR |= (0XFF<<24);

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

// Configuring SPI1 Peripheral to connect with LIS3DSH.
// The Discovery board is set as Master mode.
// PA5 PA6 PA7 and PE3 pins are used. PE3 pins is set as the Chip Select pin.
// Pin Configuration given in STM32f407 Discovery Kit Schematics - MEMS

void SPI1_set(void)
{

	RCC->AHB1ENR |= (1<<0)|(1<<4); //GPIO A and E clock enable

	GPIOA->MODER |= (2<<10)|(2<<12)|(2<<14); //Configuring GPIO A5,A6,A7 and E3 pins as output
	GPIOE->MODER |= (1<<6);

	GPIOA->OSPEEDR |=(3<<10)|(3<<12)|(3<<14); // Setting the pins speed very high
	GPIOE->OSPEEDR |= (3<<7);

	GPIOA->PUPDR |=(1<<10)|(1<<12)|(1<<14);  // Setting the pins as Pull up
	GPIOE->PUPDR |=(1<<6);

	GPIOA->AFR[0] |= (5<<20)|(5<<24)|(5<<28); // GPIOA pins to Alternate function for spi1


	RCC->APB2ENR |= (1<<12); //SPI1 CLK ENABLE

	config_spi Spi1_config;  // calling the struct from SPI header file to configuring the SPI 
	Spi1_config.SpiMode = SpiMode_master;
	Spi1_config.SpiClk = SpiClk_div32;
	Spi1_config.SpiCPHA = SpiCPHA_high;
	Spi1_config.SpiCPOL = SpiCPOL_high;
	Spi1_config.SpiSSI = SpiSSI_enable;
	Spi1_config.SpiSSM = SpiSSM_enable;

	spi_init(SPI1,&Spi1_config,GPIOE,3); // Initializing the desired SPI by passing SPI1 and its configuration
}


// Main function
int main(void){
	// Initializing variable to read the x and y axis data
	int8_t y,x;
	// Calling the Initialization function
	clkconfig();
	gpioconfig();
	SPI1_set();

	// Setting the CTRL_REG4 register of LIS3DSH to  1600 Hz frequency and enable X Y Z axis
	spi_write(SPI1,0x20,0x97,GPIOE,3);

	// Just turning the LED ON and OFF
	GPIOD->ODR |=(0xf<<12);
	for(int i =0; i<1000000;i++);
	GPIOD->ODR &= ~(0xf<<12);

	// Reading the X and Y axis data Indefinitely
	while(1){
		y = spiReaD(SPI1,(int8_t)0x29,GPIOE,3); // X axis data High Register address 0xx29
		x = spiReaD(SPI1,(int8_t)0x2B,GPIOE,3); // Y axis data High Register address 0xx2B
	}

	return 0;
}


