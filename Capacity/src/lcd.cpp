#include <iostream>

#include "system.h"

LiquidCrystal_I2C lcd(LCD_I2C_ADDRESS, LCD_SCREEN_WIDTH, LCD_SCREEN_HEIGHT);

lcd_manager::lcd_manager()
{
  beacon_idx = 0;

  lcd.begin();
  lcd.backlight();
  lcd.print("Starting...");
  delay(1000);
  idle_screen();
}

lcd_manager::~lcd_manager()
{
}

void lcd_manager::idle_screen()
{
  lcd.clear();
  lcd.print("Capacity Control");
  lcd.setCursor(0, 1);
  lcd.print("    System");
}

void lcd_manager::tracking_screen()
{
  Serial.println("In tracking screen");
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print(beacon_idx);
  lcd.print("/");
  lcd.print(system_manager::max_device);
}

void lcd_manager::alarm_screen()
{
  lcd.clear();
  lcd.print("!!!ALARM!!!");
  lcd.setCursor(0, 1);
  lcd.print("Buzzer Triggered!");
}

void lcd_manager::counter_up()
{
  beacon_idx++;
  tracking_screen();
}

void lcd_manager::counter_down()
{
  beacon_idx--;
  tracking_screen();
}
