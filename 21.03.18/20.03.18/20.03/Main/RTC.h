#ifndef RTC_H_
#define RTC_H_
#include <Arduino.h>
#include <Wire.h>

/* Địa chỉ của DS1307 */
#define DS1307            0x68
#define NumberOfFields    7

/* khai báo các biến thời gian */


extern byte second, minute, hour, day, wday, month;
extern int year;
/*************************************************
 * Hàm: readDS1307
 * 
 * Chức năng: Đọc thời gian từ Module DS1307
 *************************************************/
void readDS1307();

/*************************************************
 *
 * Hàm: bcd2dec
 * 
 * Chức năng: Chuyển mã nhị phân sang mã thập phân
 *************************************************/
int bcd2dec(byte num);

/*************************************************
 * 
 *  Hàm: dec2bcd
 *  
 *  Chức năng: Chuyển mã thập phân sang mã nhị phân
 *************************************************/
int dec2bcd(byte num);

/*************************************************
 * 
 * Hàm:
 * 
 * Chức năng: Hiển thị thời gian qua UART
 *************************************************/
void digitalClockDisplay();

/*************************************************
 * 
 * 
 *************************************************/
void printDigits(int digits);

/*************************************************
 * 
 * Hàm: setTime
 * 
 * Chức năng: Cài đặt thời gian Mudule
 *************************************************/
void setTime(byte hr=0, byte min=0, byte sec=0, byte wd=7, byte d=1, byte mth=1, byte yr=0);

#endif
