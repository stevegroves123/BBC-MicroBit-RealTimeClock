# BBC-MicroBit-RealTimeClock
i2c,RTC,MicroBit,BBC
This uses the BBC MicroBit and a RTC (DS3231 chip) on the i2c lines to show the time in BCD
Hours and Minutes are displayed in four columns as in HH MM  = H(t)H(u)<space>M9(t)M(u)
The time is entered as decimal values H = 0 to 23 and M = 0 to 59; the code breaks this down to hours(tens), hours(units),
minutes(tens) and minutes(units) then converts the values into hex.
