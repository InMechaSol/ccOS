#ifndef __ADAFRUIT_FT232H_H__
#define __ADAFRUIT_FT232H_H__

#include "io_device.h"

struct adaFruitFT232hstruct
{
    struct SPIDeviceStruct spidev;
};
struct adaFruitFT232hstruct createFT232Struct();

#ifdef __cplusplus

class adaFruitUSB_SPI_class : public SPI_DeviceClass
{
private:
	struct adaFruitFT232hstruct* FT232hPtr = nullptr;
protected:
	int opendevice();
	int closedevice();
	int readdevice();
	int writedevice();
	UI_8 isdeviceopen();
public:
	adaFruitUSB_SPI_class(struct adaFruitFT232hstruct* FT232hPtrIn);
};

#endif
#endif //!__ADAFRUIT_FT232H_H__
