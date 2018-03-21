#ifndef Button_H_
#define Button_H_
#include <Arduino.h>
#include <Keypad.h>

#define   rows          4               
#define   columns       3  
#define   holdDelay     700                                 //Khai báo biến thời gian nhằm tránh nhiễu
#define   state         4                                   //Khai báo biến trạng thái nút bấm state = (0-không nhấn/ 1-nhấn thời gian nhỏ/ 2-giữ lâu)

    uint8_t scanButton();

 #endif
