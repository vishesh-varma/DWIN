#include <stdlib.h>
#include <stdint.h>
#include <Arduino.h>

#define WRITE_COMMAND 0x82
#define READ_COMMAND 0x83

struct DisplayData
{
    uint8_t command;
    uint16_t address;
    uint16_t data;
} DWINdata;

class __DWIN
{
protected:
    DisplayData disp_dat;

public:
    void begin(unsigned long baudrate);
    void setPins(int8_t rxPin, int8_t txPin);
    void send(uint16_t address, uint16_t data);
    void send(uint16_t address, uint8_t *byteArray, uint8_t arrayLength);
    void setPage(uint16_t pageNumber);
    bool read();
    DisplayData getDisplayData();
} DWIN;