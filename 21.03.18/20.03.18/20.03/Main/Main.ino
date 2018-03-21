 #include "Test_Module.h"
    byte second, minute, hour, day, wday, month;
    int year;
    File MyFile;
void setup() {
  // put your setup code here, to run once:
  TestModule();
  setupModule();
  //setTime(15, 31, 0, 4, 21, 3, 18);
//  Serial.print("Initializing SD card..."); 
//  
//  pinMode(10, OUTPUT);
//  digitalWrite(10, HIGH);
//  
//  pinMode(4, OUTPUT);
//  digitalWrite(4, HIGH);
//  
//  pinMode(53, OUTPUT);
//  
//  if (!SD.begin(4)) {
//    Serial.println("initialization failed!");
//    return;
//  }
//  else 
//    Serial.println("initialization done.");
}

void loop() {
  // put your main code here, to run repeatedly:
  /* kiểm tra phím CONFIGURE */
//configure();
  /* Chạy chương trình */
//ethernet();

  /* quét bàn phím */
//Button();


  readDS1307();
  In_man_hinh();
  CreateFile();
  while(1){
    readDS1307();
//    rewriteLastline();
    write_SD();
  //  Serial.println("CreateFile();");
    for(byte i=0; i<=10; i++){
      delay(200);
    }
  }
}
