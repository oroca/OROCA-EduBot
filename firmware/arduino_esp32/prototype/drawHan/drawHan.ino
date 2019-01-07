#include "config.h"
#include "logo.h"



OLed lcd;



void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  lcd.begin();

  lcd.display();

  // Clear the buffer
  lcd.clearDisplay();
  lcd.setTextColor(WHITE, BLACK);
  lcd.display();  
}

void loop() {
  // put your main code here, to run repeatedly:
  static int cnt = 0;
  uint32_t pre_time;
  static int i = 0;
  int x = 0;
  int y = 0;


  pre_time = micros();
  lcd.clearDisplay();

  lcd.drawBitmap(0, 8, &logo[i*48*48/8], 48, 48, 1);
  i++;
  i %= 4;

  //lcd.printf(48+5, 48, "%d", cnt++);

  lcd.printf(48+5, 16-6, "OROCA");
  lcd.printf(48+5, 32-6, "EduBot");
  lcd.printf(48+5, 48-6, "바람");
  

  lcd.display();
  Serial.println(micros()-pre_time);
  delay(50);
}


