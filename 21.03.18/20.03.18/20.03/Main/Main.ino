 #include "Test_Module.h"
    byte second, minute, hour, day, wday, month;
    int year;
    
void setup() {
  // put your setup code here, to run once:
  TestModule();
  setupModule();
}

void loop() {
  // put your main code here, to run repeatedly:
  /* kiểm tra phím CONFIGURE */
//configure();
  /* Chạy chương trình */
//ethernet();

  /* quét bàn phím */
//Button();

/*    //Phần test SD card
  readDS1307();
  In_man_hinh();
  CreateFile();
  while(1){
    readDS1307();
  //rewriteLastline();
    write_SD();
    readLastLine();
  //  Serial.println("CreateFile();");
    for(byte i=0; i<=10; i++){
      delay(200);
    }
  }
 */

        //Phần test CheckUno
        waitting(1,1,1000,2);
}
