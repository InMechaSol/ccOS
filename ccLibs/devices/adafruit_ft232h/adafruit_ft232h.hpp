#ifndef __ADAFRUIT_FT232H__
#define __ADAFRUIT_FT232H__

#include "version_config.hpp"

struct adaFruitFT232hstruct
{
	SPIDeviceStruct spidev;
};

class adaFruitUSB_SPI_class : SPI_DeviceClass
{
private:
	struct adaFruitFT232hstruct* FT232hPtr = nullptr;
public:
	adaFruitUSB_SPI_class(struct adaFruitFT232hstruct* FT232hPtrIn);
};

#endif