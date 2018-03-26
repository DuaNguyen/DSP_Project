//#include "testModule.h"

//#include "SDcard.cpp"
//File MyFile;
//Định nghĩa chân Ethernet + chân SD
#define   ETHERNET    10
#define   SDcard      4
#define   SS_W5100    53

//Đĩnh nghĩa các chân LED hiển thị + chân Configure
#define   LED_1       13
#define   LED_2       15
#define   LED_3       15
#define   Configure   11

#define   LED_1_ON    digitalWrite(LED_1, HIGH)
#define   LED_1_OFF   digitalWrite(LED_1, LOW)
#define   LED_2_ON    digitalWrite(LED_2, HIGH)
#define   LED_2_OFF   digitalWrite(LED_2, LOW)
#define   LED_3_ON    digitalWrite(LED_3, HIGH)
#define   LED_3_OFF   digitalWrite(LED_3, LOW)
#define   READ_CONF  digitalRead(Configure)

//Định nghĩa chân cho phép từng Module Uno
#define   PC          48
#define   Module_1    49
#define   Module_2    50
#define   Module_3    51
#define   Module_4    52
#define   Module_5    53
#define   Module_6    54
#define   Module_7    55
#define   Module_8    56
#define   Module_9    57
#define   Mega        58

//Định nghía các dạng tin
//#define   REF_COMMAND                   @,,A;??!               // bản tin chưa có checksum: 
                                                                  // Header + Source + Destination + Type + Start Data + Data1 +  Data2 + End
                                                                  //  "@"   +   ","  +    ","      +  'A' +     ';'    + '?'   +   '?'  + '!'
//Định nghĩa vị trí các phần trong Frame Serial
#define   FRAME_HEADER                  0                         
#define   FRAME_DESTINATION             1
#define   FRAME_START_DATA              2
#define   FRAME_DATA_1_1                3
#define   FRAME_DATA_1_2                4
#define   FRAME_SEPERATE                5
#define   FRAME_DATA_2_1                6
#define   FRAME_DATA_2_2                7
#define   FRAME_END                     8
#define   FRAME_CHECKSUM_1              9
#define   FRAME_CHECKSUM_2              10

//Định nghĩa vị trí các phần trong Frame Serial
#define   FRAME_HEADER_Ether            0
#define   FRAME_TURN                    1                        
#define   FRAME_SEPERATE_Ether          2
#define   FRAME_HOUR_1                  3           
#define   FRAME_HOUR_2                  4
#define   FRAME_SLASH_1                 5 
#define   FRAME_MINUTE_1                6 
#define   FRAME_MINUTE_2                7
#define   FRAME_SLASH_2                 8   
#define   FRAME_DAY_1                   9            
#define   FRAME_DAY_2                   10
#define   FRAME_SLASH_3                 11 
#define   FRAME_MONTH_1                 12
#define   FRAME_MONTH_2                 13
#define   FRAME_END_Ether               14

#define   SYMBOL_HEADER_Serial          0x40
#define   SYMBOL_HEADER_Ethernet        0x24
#define   SYMBOL_START_DATA             0x3B
#define   SYMBOL_SEPERATE               0x2C
#define   SYMBOL_SLASH                  0x2F
#define   SYMBOL_END                    0x21
#define   SYMBOL_CHECK                  0x3F
//Định nghĩa các Type trong phần gửi lệnh
#define   Send_Ethernet_LastLine        65
#define   Send_UART_LastLine            66
#define   Send_UART_CHECK               67
#define   Send_UART_DATA                68
#define   Send_UART_CONFIG              69

// Các loại Lỗi
#define   ERROR_NHIET_DO                0x01
#define   ERROR_NGOAI_QUAN              0x02
#define   ERROR_USER                    0x03


//Định nghĩa chế độ hoạt động Module
#define   enable_Ethernet   digitalWrite(ETHERNET, LOW)           //cho phép Module Ethernet
#define   disable_Ethernet  digitalWrite(ETHERNET, HIGH)          //không cho phép Module Ethernet

#define   enable_SD         digitalWrite(SDcard, LOW)             //cho phép Module SD
#define   disable_SD        digitalWrite(SDcard, HIGH)            //không cho phép Module SD



//********************************    Thực hiện các hàm   *********************************//
/********************************************************
 *              ``````  Hàm tạo
 *******************************************************/ 
void TestModule(){
// LED
    LED_1_OFF;
    LED_2_OFF;
    LED_3_OFF;
//
    for(byte i=0; i<9; i++){
      _checkUno[i] = 0;
    }
    _checkEthernet = 0;
    _checkSD = 0;
    _state = 0;
    _count  = 0;
//    _typeError = 0;
    _turn = 0;
//    _product = 0;
    
// các biến khi đọc line cuối
    SD_count = 0;
    SD_second = 0;
    SD_minute = 0;
    SD_hour = 0;
    SD_typeError = 0;
    SD_lineError = 0;

// Các biến câu lệnh
    _charCommand[FRAME_HEADER]      = SYMBOL_HEADER_Serial;
    _charCommand[FRAME_DESTINATION] = PC;
    _charCommand[FRAME_START_DATA]  = SYMBOL_START_DATA;
    _charCommand[FRAME_DATA_1_1]    = SYMBOL_CHECK;
    _charCommand[FRAME_DATA_1_2]    = SYMBOL_CHECK;
    _charCommand[FRAME_SEPERATE]    = SYMBOL_SEPERATE;
    _charCommand[FRAME_DATA_2_1]    = SYMBOL_CHECK;
    _charCommand[FRAME_DATA_2_2]    = SYMBOL_CHECK;
    _charCommand[FRAME_END]         = SYMBOL_END;
    _charCommand[FRAME_CHECKSUM_1]  = 0;
    _charCommand[FRAME_CHECKSUM_2]  = 0;
     _countISR = 0;
//     _state = 0;
     _timeISR = 0;
}
/********************************************************
 *                Hàm cài đặt thông số bắt đầu 
 *******************************************************/
uint8_t setupModule(){
  Serial.begin(9600);
  Wire.begin();
  //Server_Mega.begin();
  
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);
  
  pinMode(SDcard, OUTPUT);
  pinMode(ETHERNET, OUTPUT);
  pinMode(SS_W5100, OUTPUT);
  
  disable_SD;
  disable_Ethernet;
  
  _checkSD = SD_init();
//  _checkEthernet = Ethernet_init();
// CheckUno();
//  end_interrupt();
}

/********************************************************
 *                         Hàm truyền kiểm tra Uno
 *******************************************************/
 void CheckUno(){
  Serial.println("Phần kiểm tra checkUno: ");
  _countISR = 0;
  byte type = 0;
  for (byte uno = 0; uno < Number_Uno; uno++){
    _checkUno[uno] = 0;
    /* Tạo Frame gửi đi */
    Serial.println("Tạo bản tin truyền đi");
    createFrameUART(SYMBOL_HEADER_Serial, uno+49, SYMBOL_CHECK, SYMBOL_CHECK, SYMBOL_CHECK, SYMBOL_CHECK);
    /* Gửi bản tin */
    Serial.println("Truyền bản tin");
    sendUART();
    /* Nhấp nháy LED + Kiểm tra bộ đệm Ethernet + Serial  */
    Serial.println("Nhấp nháy LED - hàm waitting: ");
    type = waitting(1,1,1000,2);
    if(type ==1 ){
      if(recieveCommand(type)){
        if( (_charCommand[FRAME_DATA_1_1] == 0x4F)  && (_charCommand[FRAME_DATA_1_2] == 0x4B) && (_charCommand[FRAME_DATA_2_1] == 0x4F)  && (_charCommand[FRAME_DATA_2_2] == 0x4B) )
          _checkUno[uno] = 1;
      }
    }
  }
}
/********************************************************
 *                         Hàm truyền kiểm tra Uno
 *******************************************************/
uint8_t recieveCommand(byte type){
  byte i = 0;
  unsigned int check = 0;
  /* Đọc giá trị truyền đến */
  switch(type){
    case 0:
      return 0;
    case 1: 
      while(Serial.available()){
        _charCommand[i] = Serial.read();
        i++;
      }
       /* Kiểm tra địa chỉ */
      if(_charCommand[FRAME_DESTINATION] != Mega)
        return 0;
      break;
    case 2:
      while(client.available()){
        _charCommand[i] = client.read();
        i++;
      }
      break;
  }

 /* Kiểm tra checkSum */
  check = checkSum(_charCommand, i -2);
  if((_charCommand[i-2] != (check >> 8)) || (_charCommand[i-1] != (check && 0xff)))
    return 0;
  
 /* Kiểm tra cấu trúc câu lệnh */
  switch(_charCommand[FRAME_HEADER]){
    // Header là Serial
    case SYMBOL_HEADER_Serial:
      if(( _charCommand[FRAME_START_DATA] == SYMBOL_START_DATA) && ( _charCommand[FRAME_SEPERATE] == SYMBOL_SEPERATE)&& ( _charCommand[FRAME_END] == SYMBOL_END)){
        return 1;
      }
      else 
        return 0;
      break;
      
    // Header là Ethernet 
    case SYMBOL_HEADER_Ethernet:
//      if(( _charCommand[FRAME_SEPERATE] == SYMBOL_SEPERATE)&& ( _charCommand[FRAME_END] == SYMBOL_END)){
        return 1;
//      }
//      else 
//        return 0;
      break;
    default:  
      return 0;
  }
}
/********************************************************
 *                         Hàm truyền kiểm tra Uno
 *******************************************************/
void createFrameUART(byte header ,byte destination, byte data1_1, byte data1_2, byte data2_1, byte data2_2){
  unsigned int check = checkSum(_charCommand, Size_Command -2);
  _charCommand[FRAME_HEADER] = header;
  _charCommand[FRAME_DESTINATION] = destination;
  _charCommand[FRAME_DATA_1_1] = data1_1;
  _charCommand[FRAME_DATA_1_2] = data1_2;
  _charCommand[FRAME_DATA_2_1] = data2_1;
  _charCommand[FRAME_DATA_2_2] = data2_2;
  _charCommand[FRAME_CHECKSUM_1] = (check >> 8);
  _charCommand[FRAME_CHECKSUM_2] = (check && 0xff);
  
  /* Phần mở rộng:
  Cấu trúc truyền 1 loại lỗi với 2 byte */
//  _charCommand[FRAME_DATA_1] = ((data2 >> 12 | data1);
//  _charCommand[FRAME_DATA_2] = (data2 && 0xff );

}

/********************************************************
 *                         Hàm send UART
 *******************************************************/
 void sendUART(){
  for (byte s = 0; s < Size_Command; s++){
    Serial.print(_charCommand[s]);
  }
 }
 
/********************************************************
 *                         Hàm check SUM
 *******************************************************/
 int checkSum (char checkSum[], byte Size){
  int sum = 0;
  for (byte c = 0; c < Size; c++){
    sum += checkSum[c];
  }
  return sum;
 }
 
/*******************************************************
 *                     Hàm cài đặt thời gian ngắt
 *******************************************************/
boolean init_interrupt(int Time, byte Mode){
  //_state = 0;                                // đảm bảo chắc chắn biến 
  cli();                                    //Không cho phép ngắt toàn cục
  TCCR1A = 0;
  TCCR1B = 0;                               //thanh ghi lựa chọn xung nhịp 
  TIMSK1 = 0;                               //thanh ghi Interrupt Mask
  _timeISR = Time;
  /* Các chế độ Prescale */
  switch(Mode){
    case 0:
      TCCR1B |= (1 << CS10);                 // prescale = 1 (thời gian tối đa (1/16)us x 65536 = 16.384ms)
      TCNT1 = 65536 - (_timeISR*16);          //thanh ghi chứa biến đếm, tính theo us
      break;
    case 1:
      TCCR1B |= (1 << CS11);                 // prescale = 1 (thời gian tối đa (8/16)us x 65536 = 65.536ms)
      TCNT1 = 65536 - (_timeISR*2);           //thanh ghi chứa biến đếm, tính theo us
      break;
    case 2:
      TCCR1B |= (1 << CS11) | (1 << CS10);             // prescale = 64 (thời gian tối đa (64/16)us x 65536 = 0.262144s)
      TCNT1 = 65536-(_timeISR/4);
      break;
    case 3:
      TCCR1B |= (1 << CS12);                          // prescale = 256 (thời gian tối đa (256/16)us x 65536 = 1,048s)
      TCNT1 = 65536-(_timeISR/16)*1000;
      break;                                                 
    case 4:
      TCCR1B |= (1 << CS12) | (1 << CS10);            // thời gian tối đa (1024/16)us x 65536 = 4,192s
      TCNT1 = 65536-(_timeISR/64)*1000;
      break;
    default:
      return 0;                                       // Nếu nhập sai chế độ Mode
  }
  return 1;                                           // Trả về 1 nếu cài đặt thông số thành công
}
/*******************************************************
 *                         Hàm tắt ngắt
 *******************************************************/
void start_interrupt(){
    Serial.println("start interrupt:");
    _state = 0;                              // báo rằng Timer bắt đầu chạy
    TIMSK1 = (1 << TOIE1);                  // cho phép ngắt tràn (Overflow interrupt enable) 
    sei();                                  // cho phép ngắt toàn cục
}
/*******************************************************
 *                         Hàm tắt ngắt
 *******************************************************/
void end_interrupt(){
    Serial.println("end interrupt:");
    TIMSK1 = 0;
    cli();
}

/*******************************************************
 *                          Hàm ngắt  
 *******************************************************/
ISR (TIMER1_OVF_vect) {
    _state = 1;
    _countISR ++;
}

/*******************************************************
 *                          Hàm chờ Timer 
 *******************************************************/
 //nháy LED báo và liên tục kiểm tra bộ đệm Serial + Ethernet
uint8_t waitting(byte times, byte type, int milis, byte mode){
  Serial.print("waiiting:");
  _countISR = 0;                          // Khai báo biến đếm số giây delay
  Serial.print(_state);
  // thời gian phải tự tính toán để phù hợp         
  while(_countISR < times){               // so sánh số lần lỗi
    if (_state == 1){                      // Over time
      Serial.println(_state);
      //_countISR ++;
      init_interrupt(milis, mode);        // khởi tạo các giá trị cho Timer  
      start_interrupt();
      end_interrupt();
    }
    /* Hiển thị nhấp nháp LED báo ko */
    if(type == 1){
      if(( _countISR % 2) == 0){
        LED_1_ON;
      }
      else
        LED_1_OFF;
    }
    
    /* Nếu có dữ liệu đến */
    if(Serial.available()){
      return 1;
    }
    if(client.available()){
      return 2;
    }
    /* Nếu phím Configure bấm */
    if(READ_CONF){
      return 3;
    }
    /* Nếu phím Configure bấm */
    if(scanButton()){
      return scanButton();
    }
  }
  delay(500);
  return 0;
}
/*******************************************************
 *                          Hàm chờ Timer 
 *******************************************************/
void configure(byte Time){
  byte i = 0;
  if (waitting(30,1,1000,2) == 1){
    while(Serial.available()){
      _charCommand[i] = Serial.read();
      i++;
    }
    setIP();
  }
}
/*******************************************************
 *                          Hàm chờ Timer 
 *******************************************************/
 void setIP(){
  
 }
/*******************************************************
 *                          Hàm chờ Timer 
 *******************************************************/
void ethernet(){
  if(/*client.available()*/1){
    _checkEthernet = 1;
    if(recieveEthernet()){
      readLastLine();
      sendEthernet();
      setTime(hour, day, 0, 7, day, month, year);
      GetFileName();
      if((SD.exists(FileName) == 1) && (_turn == 1))
        CreateHeaderFile();
    }
  }
}
/*******************************************************
 *                          Hàm chờ Timer 
 *******************************************************/
void sendEthernet(){
 // client.println("");
}
/*******************************************************
 *                          Hàm chờ Timer 
 *******************************************************/
uint8_t recieveEthernet(){
  /*  Đọc bản tin */
  byte i = 0;
  while(client.available()){
    _charCommandEthernet[i] = client.read();
    i++;
  }
  /* Kiểm tra cấu trúc */
  if(( _charCommandEthernet[FRAME_HEADER_Ether] == SYMBOL_HEADER_Ethernet) && (_charCommandEthernet[FRAME_SEPERATE_Ether] == SYMBOL_SEPERATE)&&( _charCommandEthernet[FRAME_END_Ether] == SYMBOL_END)
   &&(_charCommandEthernet[FRAME_SLASH_1] == SYMBOL_SLASH) && (_charCommandEthernet[FRAME_SLASH_1] == SYMBOL_SLASH) && (_charCommandEthernet[FRAME_SLASH_1] == SYMBOL_SLASH))
  {
    _turn   = _charCommandEthernet[FRAME_TURN];
    hour    = _charCommandEthernet[FRAME_HOUR_1]*10 + _charCommandEthernet[FRAME_HOUR_2];
    minute  = _charCommandEthernet[FRAME_MINUTE_1]*10 + _charCommandEthernet[FRAME_MINUTE_2];
    day     = _charCommandEthernet[FRAME_DAY_1]*10 + _charCommandEthernet[FRAME_DAY_2];
    month   = _charCommandEthernet[FRAME_MONTH_1]*10 + _charCommandEthernet[FRAME_MONTH_2];
    return 1;
  }
  else 
    return 0;
}
/*******************************************************
 *                          Hàm chờ Timer 
 *******************************************************/
 void Button(){
  if(scanButton()){
    _error = scanButton()/10;
    _line  = scanButton()%10;
    switch(_error){
      case ERROR_NHIET_DO:
        _error1[_line] = _error1[_line] + 1;
        Error_Product(_error,_line);
        break;
      case ERROR_NGOAI_QUAN:
        _error2[_line] = _error2[_line] + 1;
        Error_Product(_error,_line);
        break;
      case ERROR_USER:
        Error_USER(_error,_line);
        break;
    }
  }
 }
/*******************************************************
 *                          Hàm chờ Timer 
 *******************************************************/
 void Error_Product(byte error, byte line){
  /* tạo bản tin */
  createFrameUART(SYMBOL_HEADER_Serial, line,(_error1[_line]/10), (_error1[_line]%10), (_error2[_line]/10), (_error2[_line]%10));
  /* Lưu SD */
  write_SD();
  /* truyền data */
  sendUART();
  /* đặt timer */
  switch(waitting(1,0,1000,2)){
    case 0:
      sendEthernet();
      break;
    case 1:
      if(recieveCommand()){
        if( (_charCommand[FRAME_DATA_1_1] == 0x4F)  && (_charCommand[FRAME_DATA_1_2] == 0x4B) && (_charCommand[FRAME_DATA_2_1] == 0x4F)  && (_charCommand[FRAME_DATA_2_2] == 0x4B) );
         // _checkUno[uno] = 1;
      }
     
      break;
    default :
      break;
  }
 }
/*******************************************************
 *                          Hàm chờ Timer 
 *******************************************************/
 void Error_USER(byte error, byte line){
  byte type = 0;
  type = waitting(5,0,1000,2);
  switch(type){
    case 0:
      break;
    case 4:
      error = type/10;
      line = type%10;
      if(error == 1){
        _error1[_line] = _error1[_line] - 1;
        rewriteLastline();
      }
      if(error == 2){
        _error2[_line] = _error2[_line] - 1;
        rewriteLastline();
      }
      break;
    default :
      break;
  }
 }

