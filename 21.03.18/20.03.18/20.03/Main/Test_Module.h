#ifndef Test_Module_H_
#define Test_Module_H_
#include <Arduino.h>
#include <Ethernet.h>
#include <avr/interrupt.h>
#include <SPI.h>
#include <SD.h>
#include "SDcard.h"
//#include "RTC.h"
#include "Button.h"

#define   Size_Command_Ethernet    15
#define   Size_Command             11
#define   Number_Uno                9

EthernetClient client;
EthernetServer Server_Mega(80);
  void TestModule();
  uint8_t setupModule();                            //cài đặt các thông số ban đầu
  void CheckUno();        //kiểm tra Module Uno
  uint8_t recieveCommand(byte type = 1);
  void createFrameUART(byte header ,byte destination, byte data1_1, byte data1_2, byte data2_1, byte data2_2);
  void sendUART();
  int checkSum (char checkSum[], byte Size);
  boolean init_interrupt(int Time, byte Mode);
  void start_interrupt();
  void end_interrupt();
  uint8_t waitting(byte times, byte type, int milis = 1000, byte mode = 2);
  void configure(byte Time = 30);
  void setIP();
  void ethernet();
  void sendEthernet();
  uint8_t recieveEthernet();
  void Button();
  void Error_Product(byte error, byte line);
  void Error_USER(byte error, byte line);
        
  byte _checkUno[Number_Uno];
  byte _checkEthernet;
  byte _checkSD;
  int _count = 90;
  byte _typeError = 34;
  byte _turn = 0;
//      byte _product;
// các biến khi đọc line cuối
  int SD_count;
  byte SD_second;
  byte SD_minute;
  byte SD_hour;
  byte SD_typeError;
  byte SD_lineError;
  
  volatile int _countISR;
  volatile int _timeISR;
  unsigned int _error1[Number_Uno] = {0,0,0,0,0,0,0,0,0};
  unsigned int _error2[Number_Uno] = {0,0,0,0,0,0,0,0,0};
//      EthernetServer Server_Mega(80);
//      EthernetClient client;
//      extern File MyFile;
   

  char _charCommand[Size_Command];
  char _charCommandEthernet[Size_Command_Ethernet];
  byte _error = 0;
  byte _line = 0;
  
  byte product = 0;
  volatile byte _state;
  char FileName[] = "30-10-18.txt";
  File MyFile;
#endif
