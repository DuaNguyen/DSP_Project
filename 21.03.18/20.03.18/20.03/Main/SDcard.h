#ifndef SD_Card_H_
#define SD_Card_H_
#include <SD.h>
//#include "RTClib.h"
#include <string.h>
#include "RTC.h"

    void GetFileName();
    void CreateFile();
    void In_man_hinh();
    void CreateHeaderFile();
    void multi_char(byte x, char string[], byte sub);
    void multi_digit(int digit);
    void write_digit(int digit, byte type);
    void write_SD(long unsigned int Position = 0);
    void readLastLine();
    int getImfor(byte Start, byte End);
    void rewriteLastline();
    uint8_t SD_init();


#endif
