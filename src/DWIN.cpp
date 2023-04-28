#include "DWIN.h"


#define FRAME_HEADER_H 0x5A
#define FRAME_HEADER_L 0xA5


HardwareSerial dispUART(2);

uint8_t dispRxBuf[50] = {0};

void __DWIN::begin(unsigned long baudrate)
{
    dispUART.begin(baudrate);
}

void __DWIN::setPins(int8_t rxPin, int8_t txPin)
{
    dispUART.setPins(rxPin, txPin);
}

void __DWIN::send(uint16_t address, uint8_t *byteArray, uint8_t arrayLength)
{
    uint8_t Arr[50];
    Arr[0] = FRAME_HEADER_H;
    Arr[1] = FRAME_HEADER_L;
    Arr[2] = arrayLength + 3;
    Arr[3] = WRITE_COMMAND;
    Arr[4] = address >> 8;
    Arr[5] = address;
    for (int i = 0; i < arrayLength; i++)
        Arr[i + 6] = byteArray[i];
    dispUART.write(Arr, arrayLength + 6);
}

bool __DWIN::read()
{
    static uint8_t buffer[256];
    static uint8_t index = 0;
    static uint8_t packetLength = 0;
    static uint8_t checksum = 0;
    static uint8_t state = 0;

    while (dispUART.available() > 0)
    {
        uint8_t data = dispUART.read();

        switch (state)
        {
        case 0:
            if (data == FRAME_HEADER_H)
            {
                state = 1;
                buffer[index++] = data;
            }
            break;
        case 1:
            if (data == FRAME_HEADER_L)
            {
                state = 2;
                buffer[index++] = data;
            }
            break;
        case 2:
            state = 3;
            buffer[index++] = data;
            break;
        case 3:
            buffer[index++] = data;
            if (index == buffer[2] + 3)
            {
                index = 0;
                state = 0;
                disp_dat.command = buffer[3];
                disp_dat.address = (buffer[4] << 8) | (buffer[5]);
                disp_dat.data = (buffer[7] << 8) | buffer[8];
                return true;
            }
            break;
        default:
            index = 0;
            state = 0;
        }
    }
    return false;
}

void __DWIN::setPage(uint16_t pageNumber)
{
    uint8_t pageArray[4] = {0x5A, 0x01, (pageNumber>>8), pageNumber};
    send(0x0084, pageArray, sizeof(pageArray));
}

void __DWIN::send(uint16_t address, uint16_t data)
{
    uint8_t dataArray[2] = {(uint8_t)(data>>8), (uint8_t)(data)};
    send(address, dataArray, 2);
}

DisplayData __DWIN::getDisplayData()
{
    return disp_dat;
}