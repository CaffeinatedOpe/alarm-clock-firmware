#include <SPI.h>
#include <SD.h>

#include <iostream>
#include <vector>

const int chipSelect=13;

void initSD() {
	SD.begin(chipSelect);
}