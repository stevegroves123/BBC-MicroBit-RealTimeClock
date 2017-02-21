/*
*   RTC for BBC MicroBit
*   Jan 2017 by Steven Groves
*
*   i2C address  = 0x68 but left shift (<<1) 
*   by 1 to give 0xD0
*   This programme displays the HH:MM in BCD Binary
*   on the 5x5 LED display
*   Due to using the DS3231 Pixnor Tiny RTC the time does not hold 
*   when power is removed, a 3500mF cap on the 3v3 & Gnd lines allows 
*   for a 20 second backup.
*/

#include "MicroBit.h"
#include "MicroBitI2C.h"

MicroBit uBit;

// set hours and minutes to the decimal value
unsigned int hours = 19;
unsigned int minutes = 57;

unsigned int bcdHours = 0;;
unsigned int bcdMinutes = 0;
unsigned int returnValue = 0;
unsigned int seconds = 00;
unsigned int secondsUnits = 0;
unsigned int secondsTens = 0;
unsigned int minutesUnits = 0;
unsigned int minutesTens = 0;
unsigned int hoursUnits = 0;
unsigned int hoursTens = 0;
unsigned int dim = 200;

void setUpButton();
void writei2c(MicroBitEvent e);
void flashLEDS(MicroBitEvent e);
void increaseMinutes(MicroBitEvent e);
void readi2c();
void displayTime(int time, int location);
int convert2Hex(int value);

int main()
{
    uBit.init();
    uBit.display.setDisplayMode(DISPLAY_MODE_GREYSCALE);
    setUpButton();
    bcdHours = convert2Hex(hours);
    bcdMinutes = convert2Hex(minutes);
    while (1) {
// After 10pm and before 7am the display is dimmed
        dim = 200;
        if (hoursTens > 1 || hoursUnits < 7)
        {
            dim = 75;
            }
        readi2c();
    }
    release_fiber();
}

void setUpButton()
{
    uBit.buttonA.setEventConfiguration(MICROBIT_BUTTON_ALL_EVENTS);
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_A,
                           MICROBIT_BUTTON_EVT_CLICK,
                           writei2c);
    
    uBit.buttonB.setEventConfiguration(MICROBIT_BUTTON_ALL_EVENTS);
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_B,
                           MICROBIT_BUTTON_EVT_LONG_CLICK,
                           flashLEDS);
}

void flashLEDS(MicroBitEvent e)
{
    for (int i = 0; i < 10; i++)
    {
        uBit.sleep(250);
        readi2c();
        }
    }

void writei2c(MicroBitEvent e)
{
// bufwrite is the pointer for 
// i2c.write(addr_register, data_seconds, data_minutes, data_hours)
    char bufWrite[] = {0, seconds, bcdMinutes, bcdHours};
    // write bufWrite to memory location 0,1,2,3 
    uBit.i2c.write(0xd0,bufWrite,4,true);  
}

void readi2c()
{
    char buf[6] = {0};
    uBit.i2c.write(0xd0,buf,1);
    uBit.i2c.read(0xd0,buf,6,true);
    secondsUnits = ((int)buf[0] & 0x0f);
    secondsTens = ((int)buf[0]>>4); 
    minutesUnits = ((int)buf[1] & 0x0f);
    minutesTens = ((int)buf[1]>>4);
    hoursUnits = ((int)buf[2] & 0x0f);
    hoursTens = (((int)buf[2]>>4) & 0x03);
    displayTime(hoursTens, 0);
    displayTime(hoursUnits, 1);
    displayTime(minutesTens, 3);
    displayTime(minutesUnits, 4);
    uBit.sleep(500);
    uBit.display.image.clear();
}

void displayTime(int time, int location)
{
    if (time == 0)
    {
        
    }
    if (time == 1)
    {
        uBit.display.image.setPixelValue(location,4,dim);
    }
    if (time == 2)
    {
        uBit.display.image.setPixelValue(location,3,dim);
    }
    if (time == 3)
    {
        uBit.display.image.setPixelValue(location,4,dim);
        uBit.display.image.setPixelValue(location,3,dim);
    }
    if (time == 4)
    {
        uBit.display.image.setPixelValue(location,2,dim);
    }
    if (time == 5)
    {
        uBit.display.image.setPixelValue(location,4,dim);
        uBit.display.image.setPixelValue(location,2,dim);
    }
    if (time == 6)
    {
        uBit.display.image.setPixelValue(location,3,dim);
        uBit.display.image.setPixelValue(location,2,dim);
    }
    if (time == 7)
    {
        uBit.display.image.setPixelValue(location,4,dim);
        uBit.display.image.setPixelValue(location,3,dim);
        uBit.display.image.setPixelValue(location,2,dim);
    }
    if (time == 8)
    {
        uBit.display.image.setPixelValue(location,1,dim);
    }
    if (time == 9)
    {
        uBit.display.image.setPixelValue(location,4,dim);
        uBit.display.image.setPixelValue(location,1,dim);
    }
}

int convert2Hex(int value)
{
    returnValue = (value / 10 * 16)+(value % 10);
    return returnValue;
}
