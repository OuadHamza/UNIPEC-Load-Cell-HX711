/*
* https://unipec.ma
* 8 December 2020
* Load Cell HX711 Module Interface with minimum alert for Arduino
*  
* HX711 pins: 
*  5 -> HX711 sck
*  4 -> DOUT
*  5V -> VCC
*  GND -> GND

* buzzer pin : 2
* relais pin : 7
* LCD pins A4 = SDA // A5 =  SCL
* 
* Copyright (c) 2020, Ouad Hamza @UNIPEC
*/


#include <HX711_ADC.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//Pins
const int HX711_dout = 4;
const int HX711_sck = 5;
const int relais = 7;


LiquidCrystal_I2C lcd(0x27, 16, 2);
HX711_ADC LoadCell(HX711_dout, HX711_sck);
long t;


// min KG value (to edit)

int minvalue = 9;


void setup() {

  //lcd setup code 

  lcd.begin();
  lcd.backlight();
  lcd.println("Starting...");

  ///////////////////////////////

  pinMode(relais, OUTPUT);
  LoadCell.begin();
  float calibrationValue;
  calibrationValue = -14521.00;

  long stabilizingtime = 2000;
  boolean _tare = false;
  LoadCell.start(stabilizingtime, _tare);

  //check errors
  if (LoadCell.getTareTimeoutFlag()) {

    lcd.println("Timeout, error");
    while (1);
  }
  else {
    LoadCell.setCalFactor(calibrationValue);
    lcd.println("Startup is complete");
  }

}



void loop() {

  static boolean newDataReady = 0;
  const int serialPrintInterval = 500;

  if (LoadCell.update()) newDataReady = true;

  if (newDataReady) {

    if (millis() > t + serialPrintInterval) {
      float i = LoadCell.getData();


      // code pour LCD
      lcd.setCursor(1,0);
      lcd.clear();
      lcd.print("Poid: ");
      lcd.print(i);
      lcd.print("kg");

      lcd.setCursor(0,1);
      lcd.print("min Poid : ");

      lcd.print(minvalue);
      lcd.print("kg");

      relaisAlarm(i);
      
      newDataReady = 0;
      t = millis();
    }
  }
}

void relaisAlarm(float i) {

  if (i>=minvalue ){
    digitalWrite(relais,0);


  } else{
    digitalWrite(relais,1);
  }
}
