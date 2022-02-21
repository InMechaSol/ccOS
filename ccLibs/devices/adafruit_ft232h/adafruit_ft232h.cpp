#include "adafruit_ft232h.c"

adaFruitUSB_SPI_class::adaFruitUSB_SPI_class(struct adaFruitFT232hstruct* FT232hPtrIn) :SPI_DeviceClass(&FT232hPtrIn->spidev)
{
	FT232hPtr = FT232hPtrIn;
}

int adaFruitUSB_SPI_class::opendevice()
{
	return 0;
}

int adaFruitUSB_SPI_class::closedevice()
{
	return 0;
}

int adaFruitUSB_SPI_class::readdevice()
{
	return 0;
}

int adaFruitUSB_SPI_class::writedevice()
{
	return 0;
}

UI_8  adaFruitUSB_SPI_class::isdeviceopen()
{
	return ui8FALSE;
}