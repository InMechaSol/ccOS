#include "adafruit_ft232h.hpp"

adaFruitUSB_SPI_class::adaFruitUSB_SPI_class(struct adaFruitFT232hstruct* FT232hPtrIn) :SPI_DeviceClass(&FT232hPtrIn->spidev)
{
	FT232hPtr = FT232hPtrIn;
}