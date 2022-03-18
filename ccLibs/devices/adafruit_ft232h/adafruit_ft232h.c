#include "adafruit_ft232h.h"

struct adaFruitFT232hstruct createFT232Struct()
{
	struct adaFruitFT232hstruct outstruct;
	outstruct.spidev.chipSelectEnabled = false;
	return outstruct;
}