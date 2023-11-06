#include<Wire.h>
#include<Adafruit_GFX.h>
#include<Adafruit_SSD1306.h>
#include<Adafruit_BMP280.h>
#include"dht.h"


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define ADC_RES 0.0048828125

#define LDR_PIN A0
#define DHT_PIN 7


Adafruit_SSD1306 display(SCREEN_WIDTH,SCREEN_HEIGHT,&Wire,-1);
Adafruit_BMP280 bmp;

int humidity = 0;
float temperature = 0.0;
float altitude = 0.0;
float LUX = 0.0;

float measure_lux(int ldr_pin);

void setup() {
  Serial.begin(9600);

  if(!display.begin(SSD1306_SWITCHCAPVCC,0x3c)){
    Serial.println("Display failed!");
    for(;;);
  }

  if(!bmp.begin(0x76)){
    Serial.println("BMP280 not found!");
  }
  
  delay(2000);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(3);
  display.setCursor(0,0);
  display.println("HIKERS-KIT_69");
  display.display();
  delay(2000);

}

void loop() {
  LUX = measure_lux(LDR_PIN);
  temperature = bmp.readTemperature();
  altitude = bmp.readAltitude();
  int i = dht_read(DHT_PIN);
  humidity = dht_humidity;
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.drawLine(0,0,128,0,WHITE);
  display.setCursor(2,1);
  display.print("Humidity  ");
  display.print(humidity);
  display.println(" %");
  display.drawLine(1,11,128,11,WHITE);
  display.setCursor(2,12);
  display.print("Temperature  ");
  display.print(temperature);
  display.println(" C");
  display.drawLine(1,22,128,22,WHITE);
  display.setCursor(1,23);
  display.print("Altitude  ");
  display.print(altitude);
  display.println("m");
  display.drawLine(1,33,128,33,WHITE);
  display.setCursor(2,34);
  display.print("LUX  ");
  display.println(LUX);
  display.drawLine(1,44,128,44,WHITE);
  display.setCursor(2,45);
  display.print("Compass  ");
  display.println();
  display.drawLine(0,63,128,63,WHITE);
  display.drawLine(0,0,0,64,WHITE);
  display.drawLine(127,0,127,64,WHITE);

  display.drawLine(53,0,53,10,WHITE);
  display.drawLine(71,11,71,22,WHITE);
  display.drawLine(53,23,53,33,WHITE);
  display.drawLine(23,34,23,44,WHITE);
  display.drawLine(47,45,47,63,WHITE);
  display.display();
  delay(500);
}
float measure_lux(int ldr_pin){
  float lux;
  int LDR_val = analogRead(ldr_pin);
  lux = (250.0/(LDR_val*ADC_RES))-50.0;
  Serial.println(lux);
  return lux;
}
