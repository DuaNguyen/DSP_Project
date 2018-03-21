#include "Button.h"


    unsigned char keys[4][3] =
    {
      {14, 24, 34},
      {13, 23, 33},
      {12, 22, 32},
      {11, 21, 31},
    };
    unsigned char rowPins[4] = {23, 25, 27, 29};
unsigned char columnPins[3] = {31, 33, 35};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, columnPins, rows, columns);


/* Định nghĩa các chân kết nối Moudule */

 /********************************************************
 *                            Hàm quét bàn phím
 ********************************************************/
uint8_t scanButton(){
  /* Quét bàn phím */
     char key = 0;                        // lưu giá trị phím bấm
  /* Khi có phím bấm */
  if((int)keypad.getState() == PRESSED){
     key = keypad.getKey();                              //đọc phím bấm
     while ((int)keypad.getState() !=  RELEASED);        //chờ đến khi nhả phím
     return key;
  }
  return 0;
}

