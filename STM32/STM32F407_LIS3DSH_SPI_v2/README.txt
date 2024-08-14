
STM32F407_LIS3DSH_SPI_v2 :  This is a simple program on creating a SPI driver to communicate with LIS3DSH accelrometer
                            This Driver is not a Full SPI driver, rather only the necessary functions and declaration are written.
                            Comparing to the STM32F407_LIS3DSH_SPI_V1 which implements the SPI directly, this implementation uses header and function files declared in the library.

                            STM32F407_LIS3DSH_SPI_v2.c is the main file
                            SpiDrive.h is the Header file for SPI driver
                            SpiDrive.c is the function definition file for SPI Driver
