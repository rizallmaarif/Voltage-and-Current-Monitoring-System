#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_INA219.h>
#include <SD.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

Adafruit_INA219 ina219;

File imsfile;

const int CS = 5;

void setup()
{
  ina219.begin();
  ina219.setCalibration_32V_50A();

  lcd.begin();
  lcd.backlight();

  SD.begin();
}

void loop()
{
  float shuntvoltage  = 0;
  float busvoltage    = 0;
  float current_mA    = 0;
  float loadvoltage   = 0;
  float power_mW      = 0;

  shuntvoltage  = ina219.getShuntVoltage_mV();
  busvoltage    = ina219.getBusVoltage_V();
  current_mA    = ina219.getCurrent_mA();
  power_mW      = ina219.getPower_mW();
  loadvoltage   = busvoltage + (shuntvoltage / 1000);

  lcd.setCursor(0, 0);
  lcd.print(loadvoltage);
  lcd.print(" ");
  lcd.println("Volt");

  lcd.setCursor(0, 1);
  lcd.print((int)current_mA);
  lcd.print(" ");
  lcd.println("mA");

  delay(1000);

  imsfile = SD.open("sdcard.txt", FILE_WRITE);
  if(imsfile)
  {
    imsfile.print(loadvoltage);
    imsfile.print("Volt");
    imsfile.print("    |    ");
    imsfile.print(current_mA);
    imsfile.print("mA");
    imsfile.println();
    imsfile.close();  
  }
}