#include "config.h"
#include "logo.h"



OLed lcd;
VL53L0X tof;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  lcd.begin();


  // Clear the buffer
  lcd.clearDisplay(); 
  lcd.setTextColor(WHITE, BLACK);
    
  if (tof.begin() == true)
  {
    lcd.println("tof : OK");
  }
  else
  {
    lcd.println("tof : Fail");
  }
  
  lcd.display();  

  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  static int cnt = 0;
  uint32_t pre_time;
  static int i = 0;
  static int x = 0;
  static int y = 0;


  tof.update();

  pre_time = micros();
  lcd.clearDisplay();

  lcd.drawBitmap(80-x, 16, &logo[i*48*48/8], 48, 48, 1);
  i++;
  i %= 4;

  //lcd.printf(48+5, 48, "%d", cnt++);

  //lcd.printf(48+5, 16-6, "OROCA");
  //lcd.printf(48+5, 32-6, "EduBot");
  //lcd.printf(48+5, 48-6, "바람");
  lcd.printf(0, 0, "%d mm", tof.distance_mm);

  x = constrain(tof.distance_mm, 0, 150);
  x = map(x, 0, 150, 0, 80);
  
  lcd.display();
  delay(50);
}


