#include "orocaboy_go.h"
#include <WiFi.h>
#include <WiFiMulti.h>

WiFiMulti WiFiMulti;
WiFiClient client;

const uint16_t port = 8881;
const char * host = "192.168.1.100"; 


  
void setup() {
  gb.begin();
  gb.display.clear();


  // We start by connecting to a WiFi network
  WiFiMulti.addAP("canDynamix", "canDynamix");
  
  gb.display.setColor(GREEN);
  gb.display.println("canDynamix mini");
  gb.display.setColor(WHITE);
  gb.display.println();
  gb.display.print("Wait for WiFi... ");
  gb.updateDisplay();
  while(WiFiMulti.run() != WL_CONNECTED) 
  {
    if (gb.update())
    {
      gb.display.print(".");
      delay(500);
    }
  }

  gb.display.println("OK");
  gb.updateDisplay();

  gb.display.println("Wait for Connection.. ");
  if (!client.connect(host, port)) 
  {
     gb.display.println("connection failed");
     gb.updateDisplay();
  }
  else
  {
    gb.display.println("OK");
    gb.updateDisplay();  
    client.setNoDelay(true);
    client.print("Send this data to server");
  }
      
  delay(1000);  
}

void loop() {
  // put your main code here, to run repeatedly:
  static int cnt = 0;
  
  if (gb.update())
  {      
    gb.display.clear();

    gb.display.setColor(BLUE);

    gb.display.setCursor(0, 0);
    gb.display.setColor(WHITE);
    gb.display.println("Arduino Games Buttons");
    gb.display.println();
    gb.display.print("Cnt : ");
    gb.display.println(cnt++);

    

    gb.display.setColor(BLUE);
    gb.display.drawCircle(gb.display.width()/2    + 10, gb.display.height()/2, 10);        // A
    gb.display.drawCircle(gb.display.width()/2+20 + 10, gb.display.height()/2, 10);        // B

    gb.display.drawCircle(gb.display.width()/2    + 10, gb.display.height()/2 - 20, 10);   // MENU
    gb.display.drawCircle(gb.display.width()/2+20 + 10, gb.display.height()/2 - 20, 10);   // HOME

    gb.display.drawCircle(gb.display.width()/2/2-5 , gb.display.height()/2, 10);      // left
    gb.display.drawCircle(gb.display.width()/2/2+25, gb.display.height()/2, 10);      // right
    gb.display.drawCircle(gb.display.width()/2/2+10, gb.display.height()/2-15, 10);   // up
    gb.display.drawCircle(gb.display.width()/2/2+10, gb.display.height()/2+15, 10);   // down



    if (gb.buttons.pressed(Button::a))
    {
      client.print("A");
      gb.display.println("Key : A");
      gb.display.fillCircle(gb.display.width()/2 + 10, gb.display.height()/2, 10);
    }
    if (gb.buttons.pressed(Button::b))
    {
      client.print("B");
      gb.display.println("Key : B");
      gb.display.fillCircle(gb.display.width()/2+20 + 10, gb.display.height()/2, 10);
    }
    if (gb.buttons.repeat(Button::menu,  1))
    {
      gb.display.println("Key : MENU");
      gb.display.fillCircle(gb.display.width()/2    + 10, gb.display.height()/2 - 20, 10);
    }
    if (gb.buttons.repeat(Button::home,  1))
    {
      gb.display.println("Key : HOME");
      gb.display.fillCircle(gb.display.width()/2+20 + 10, gb.display.height()/2 - 20, 10);
    }
    if (gb.buttons.repeat(Button::left, 1))
    {
      client.print("L");
      gb.display.println("Key : LEFT");
      gb.display.fillCircle(gb.display.width()/2/2-5 , gb.display.height()/2, 10);
    }
    if (gb.buttons.repeat(Button::right, 1))
    {
      client.print("R");
      gb.display.println("Key : RIGHT");
      gb.display.fillCircle(gb.display.width()/2/2+25, gb.display.height()/2, 10);
    }
    if (gb.buttons.repeat(Button::up, 1))
    {
      client.print("U");
      gb.display.println("Key : UP");
      gb.display.fillCircle(gb.display.width()/2/2+10, gb.display.height()/2-15, 10);
    }
    if (gb.buttons.repeat(Button::down, 1))
    {
      client.print("D");
      gb.display.println("Key : DOWN");
      gb.display.fillCircle(gb.display.width()/2/2+10, gb.display.height()/2+15, 10);
    }
  }  
}

