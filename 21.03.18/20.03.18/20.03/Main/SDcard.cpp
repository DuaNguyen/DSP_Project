#include "SDcard.h"
#define   Number_Uno                9
extern char FileName[];// = "30-10-18.txt";
extern unsigned int _error1[Number_Uno];
extern unsigned int _error2[Number_Uno];
byte typeError = 34;

extern int _count;
int sizeHeader = 0;
int sizeLine = 98;
unsigned long sizeFile = 0;

extern byte _turn;
extern byte product;
extern int  SD_count;
extern byte SD_second;
extern byte SD_minute;
extern byte SD_hour;
extern byte SD_typeError;
extern byte SD_lineError;

//byte a[20];
extern byte second, minute, hour, day, wday, month;
extern int year;
extern File MyFile;
//void write_SD(unsigned long Position = 0);
/*--------------------------------------------------------------------------------------------
void setup() {
  Wire.begin();
  Serial.begin(9600);
   setTime(12, 30, 45, 1, 12, 3, 19);
  Serial.print("Initializing SD card..."); 
  
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);
  
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);
  
  pinMode(53, OUTPUT);
  
  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    return;
  }
  else 
    Serial.println("initialization done.");
} 

//----------------------------------------------------------------------------------------------------------
void loop() {
  readDS1307();
  In_man_hinh();
//    pinMode(4, OUTPUT);
//  digitalWrite(4, HIGH);
//  GetFolderName();
//  Serial.println("GetFolderName();");
// CreateFolder();
//  Serial.println("CreateFolder();");
//  multi_char(1,"2",4);
//  delay(3000);
//  GetFileName();
//  Serial.println("GetFileName();");
  CreateFile();
//  headerFile_SD();
  while(1){
    readDS1307();
//    rewriteLastline();
    write_SD();
  //  Serial.println("CreateFile();");
    for(byte i=0; i<=10; i++){
      delay(200);
    }
  }

    pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);
  pinMode(53, OUTPUT);
  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);
  if (SD.begin(4)) 
    Serial.println("initialization done.");
    delay (1000);
}
*/
//--------------------------------------------------------------------------------------------------------------
void GetFileName() {
  readDS1307();
  FileName[0] = day/10 + 48;
  FileName[1] = day%10 + 48;
  FileName[2] = 45;
  FileName[3] = month/10 + 48;
  FileName[4] = month%10 + 48;
  FileName[5] = 45;
  FileName[6] = (year-2000)/10 + 48;
  FileName[7] = (year-2000)%10 + 48;
}
//--------------------------------------------------------------------------------------------------------------
void CreateFile() {
  /* Check file exists */
  GetFileName();
  if (SD.exists(FileName)) {
    Serial.println("exists.");
  }
  /* Creat file*/
  else {
    Serial.print(FileName);
    Serial.println(" doesn't exist.");
    Serial.println("Creating new file");
    CreateHeaderFile();
    delay(100);
    if( !SD.exists(FileName) ) {
      Serial.print("File created fail!");
    }
    else{
      Serial.print("File exists: ");
    }
  }
}
//-------------------------------------------------------------------------------------------------------
void In_man_hinh(){
  Serial.print("Thoi gian: ");
  Serial.print(hour);
  Serial.print(":");
  Serial.print(minute);
  Serial.print(":");
  Serial.print(second);
  Serial.print("  /  ");
  Serial.print(day);
  Serial.print("-");
  Serial.print(month);
  Serial.print("-");
  Serial.println(year);
}
//------------------------------------------------------------------------------------------------------------
void CreateHeaderFile(){
  MyFile = SD.open(FileName, FILE_WRITE);
  
  /* Header */
  MyFile.write("File Save Data");

  multi_char(3,"\r\n",0);

  MyFile.write("Turn: ");
  if(_turn != 0){
    MyFile.write(_turn+48);
  }
  else
    MyFile.write("Unknown");
    
  MyFile.write("\r\n");

  MyFile.write("Product: ");
  switch(product){
    case 0:
            MyFile.write("Unknown-");
            break;
    case 2:
            MyFile.write("ruot phich");
            break;
  }
   
  multi_char(7," ",0);
  MyFile.write("Date: ");
  MyFile.write(day/10+48);
  MyFile.write(day%10+48);
  MyFile.write(".");
  MyFile.write(month/10+48);
  MyFile.write(month%10+48);
  MyFile.write(".");
  MyFile.write("20");
  MyFile.write((year-2000)/10 + 48);
  MyFile.write((year-2000)%10 + 48);
  MyFile.write("\r\n");
  
  /* Tạo bảng */
  multi_char(12,"-",0);
  MyFile.write("\r\n");

  multi_char(2,"Time",4);
  multi_char(1,"Type",4);
  multi_char(1,"line1",5);
  multi_char(1,"line2",5);
  multi_char(1,"line3",5);
  multi_char(1,"line4",5);
  multi_char(1,"line5",5);
  multi_char(1,"line6",5);
  multi_char(1,"line7",5);
  multi_char(1,"line8",5);
  multi_char(1,"line9",5);

  MyFile.write("\r\n");
  multi_char(12,"-",0);
  MyFile.write("\r\n");

  sizeHeader = MyFile.size();
//  
//  MyFile.seek(215);                                           //Set con trỏ trong file SD
//  Serial.print("MyFile.available(): ");                       // = Size - seek
//  Serial.println(MyFile.available());
  Serial.print("MyFile.size(): ");
  Serial.println(MyFile.size());                              // kích thước toàn bộ file
//  Serial.print("MyFile.position(): ");
//  Serial.println(MyFile.position());                          // vị trí hiện tại của con trỏ
//  MyFile.seek(0);
//  for(byte i=0; i<150; i++){
//    MyFile.seek(i);
//    Serial.print("MyFile.peek(): ");
//    Serial.println(char(MyFile.peek()));                      // hiển thị giá trị tại vị trí con trỏ
//  }
  MyFile.close(); 
  
  Serial.println("Write done!");
}
//---------------------------------------------------------------------------------------------------------
/*
 * 
 * x        : viết ở cột nào
 * string[] : mảng ký tự truyền vào
 * sub      : kích thước của mảng ký tự truyền vào
 */
void multi_char(byte x, char string[], byte sub){
  if(string == "\r\n" || string == "*"){
    for(byte i=0; i<x; i++){
      MyFile.write(string);
    }
  }
  else {
    if(string == "-"){
      for(byte i=0; i<(x*8); i++){
        MyFile.write(string);
      }
    }
    else {
      //byte sub = sizeof(string);
      //Serial.println(char(string));
      if(x>=2){
        for(byte i=0; i<(8*(x-1)); i++){
          MyFile.write(" ");
        }
      }
      MyFile.write(string);
      for(byte i=0; i<(8-sub); i++){
        MyFile.write(" ");
      }
    }
  }
}


//--------------------------------------------------------------------------------------------------------------------
/*Chức năng: Viết số lượng lỗi theo từng cột vào file SD
 * 
 */
void multi_digit(int digit1, int digit2){
  byte a[3];
  byte count = 1;
  if(digit1 < 1000 ){
    a[0] = digit1/100;
    a[1] = digit1/10;
    a[2] = digit1%10;
    /* Hiện chữ số trăm, chục nếu # 0 */
    for(byte i=0; i<2; i++){
      if(a[i] != 0){
        MyFile.write(char(a[i]+48));
        count++;
      }
    }
    /* Hiện chữ số đơn vị */
    MyFile.write(char(a[2]+48));
    for(byte i=0; i<(3-count); i++)
      MyFile.write(" ");
  }
  
    
  /* Hiện _error2 */
  if(digit2 < 1000){
    MyFile.write("-");
    count = 1;
    a[0] = digit2/100;
    a[1] = digit2/10;
    a[2] = digit2%10;
    for(byte i=0; i<2; i++){
      if(a[i] != 0){
        MyFile.write(char(a[i]+48));
        count++;
      }
    }
    MyFile.write(char(a[2]+48));
    for(byte i=0; i<(4-count); i++)
      MyFile.write(" ");
  }
  else {
    for(byte i=0; i<5; i++)
      MyFile.write(" ");
  }
}
//-------------------------------------------------------------------------------------------------------------------
/*Chức năng: Viết dữ liệu vào file SD
 * 
 * Biến 'sizeline' có thể đặt là giá trị cố định 
 * kích thước 1 dòng tin là 98 ký tự
 */
void write_SD(long unsigned int Position){
  MyFile = SD.open(FileName, FILE_WRITE);
  sizeFile = MyFile.size();
  Serial.print("size file: ");
  Serial.println(sizeFile);
  /*  */
  MyFile.seek(MyFile.position()-Position);
  /* STT */
  write_digit(_count,1);
  multi_char(0," ",6);
  
  /* Time */
  write_digit(hour,0);
  MyFile.write(":");
  write_digit(minute,0);
  MyFile.write(":");
  write_digit(second,0);
  multi_char(0," ",7);
  
  /* Type error*/
  multi_digit(typeError,1000);
  
  /* Line */
  for(byte i=0; i<9; i++){
    multi_digit(_error1[i], _error2[i]);
  }
  MyFile.write("\r\n");

  if(Position == 0){
    sizeLine = MyFile.size()-sizeFile;
  }
  MyFile.close();
  
  Serial.print("size line: ");
  Serial.println(sizeLine);
   
  _count++;
}

//------------------------------------------------------------------------------------------------------------
/*Chức năng: In số thứ tự trong file .txt
 * digit: số truyền vào
 * type: 1 - dạng hiển thị có số "0" đằng trước
 *       0 - không có số "0"
 */
void write_digit(int digit, byte type){
  //phần này dành riêng cho STT
  if(type == 1){
    if(digit > 99){
      MyFile.write((digit/100)+48);
      digit = digit%100;
      type = 0;
    }
    else 
      MyFile.write(" ");
  }
  //phần chung cho STT và thời gian
  if((digit/10) < 1){
    if(type == 1){
      MyFile.write(" ");                      //Type 1 - hiển thị số TT
    }
    else 
      MyFile.write("0");                      //Type 0 - hiển thị thời gian
  }
  else
    MyFile.write((digit/10)+48);
    
  MyFile.write((digit%10)+48);
}
//-------------------------------------------------------------------------------------------------------------
/*
 * line : dòng cần đọc
 */
void readLastLine(){
  MyFile = SD.open(FileName);
//  getImfor(1,3);
  /* Đọc Số thứ tự */
  SD_count  = getImfor(1,3);
//  Serial.print("SD_count: ");
//  Serial.println(SD_count);
  /* Đọc thời gian */
//  Serial.print("Time:  ");
  SD_hour   = getImfor(7,9);
//  Serial.print(SD_hour);
//  Serial.print(":");
  SD_minute = getImfor(10,12);
//  Serial.print(SD_minute);
//  Serial.print(":");
  SD_second = getImfor(13,15);
//  Serial.print(SD_second);
//  Serial.println();
  /* Đọc kiểu lỗi */
  SD_typeError = getImfor(17,24)/10;
//  Serial.print("SD_typeError: ");
//  Serial.println(SD_typeError);
  SD_lineError = getImfor(17,24)%10;
//  Serial.print("SD_lineError: ");
//  Serial.println(SD_lineError);
  /* Đọc số lượng lỗi từng line */
  for (byte i=0; i<9; i++){
    _error1[i] = getImfor((i+3)*8+1,(i+4)*8);
//    Serial.print("error: line ");
//    Serial.print(i+1);
//    Serial.print("- ");
//    Serial.println(error[i]);
  }

  MyFile.close();
}
//-------------------------------------------------------------------------------------------------------------
int getImfor(byte Start, byte End){
  int a = 0;
  int x = 0;
  byte x1 = 0;
  byte b = 1;
//  Serial.println("ham getImfor -->>");
  for(byte i = End; i >= Start; i--){
    MyFile.seek(MyFile.size()-sizeLine+i-1);
//    Serial.print("MyFile.peek(): ");
//    Serial.print(i);
//    Serial.print(" - ");
//    Serial.println(char(MyFile.peek()));
//    Serial.print("MyFile.peek()/10+48: ");
//    Serial.println(char(MyFile.peek()/10+48));
//    Serial.print("MyFile.peek()%10+48): ");
//    Serial.println(char(MyFile.peek()%10+48));
    if((char(MyFile.peek()) != ' ') && (char(MyFile.peek()) != ':') && (MyFile.peek() >= 48) && (MyFile.peek() <= 58)){
//      Serial.println("(t/m dieu kien IF)");
      a = a+ (MyFile.peek()-48)*b;
      b = b*10;
//      Serial.print("b");
//      Serial.println(char(b+48));
//      Serial.print("a >= 100");
//      Serial.print(char(x/100+48));
//      Serial.print("a/10+48: ");
//      Serial.print("a%10+48: ");
//      Serial.println(char(a%10+48));
    }
    else{
     b = 1;
//       a = 0;
    }
  }
//  Serial.print("a+48: ");
//  x = a;
//  if(x>99){
////    Serial.println("a >= 100");
//    Serial.print(char(x/100+48));
//    x=x%100;
//  }
////  Serial.print(char(x/100+48));
////  x=x/10;
//  Serial.print(char(x/10+48));
//  Serial.println(char((x%10)+48));
  return a;
}
//--------------------------------------------------------------------------------------------------------------------
/*
 * 
 */
void rewriteLastline(){
  if(typeError >= 100){
    typeError = 0;
  }
  _count--;
  Serial.print("Type_Error: "); 
  Serial.print(char(typeError/10+48));
  Serial.println(char((typeError%10)+48));
  write_SD(sizeLine);
  typeError ++;
}
//-------------------------------------------------------------------------------
uint8_t SD_init(){
  if (!SD.begin(4)){
    return 0;
  }
  else {
    GetFileName();
    if(SD.exists(FileName)){
      readLastLine();
    }
    else{
      CreateHeaderFile();
    }
  }
}

