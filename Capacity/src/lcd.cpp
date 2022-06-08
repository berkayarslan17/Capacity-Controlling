#include <iostream>

#include "system.h"

LiquidCrystal_I2C lcd(LCD_I2C_ADDRESS, LCD_SCREEN_WIDTH, LCD_SCREEN_HEIGHT);

static bool control_alarm(int beacon_idx)
{
  if (system_manager::max_device < beacon_idx)
  {
    Serial.println("flag true");
    digitalWrite(BUZZER_PIN, LOW); // turn buzzer on
    return true;
  }
  else
  {
    Serial.println("flag false");
    digitalWrite(BUZZER_PIN, HIGH); // turn buzzer off
    return false;
  }
}

lcd_manager::lcd_manager()
{
  beacon_idx = 0;

  lcd.begin();
  lcd.backlight();
  lcd.home();
  delay(100);
  idle_screen();
}

lcd_manager::~lcd_manager()
{
}

void lcd_manager::idle_screen()
{
  delay(100);
  lcd.clear();
  lcd.print("Capacity Control");
  lcd.setCursor(0, 1);
  lcd.print("System");
}

void lcd_manager::tracking_screen()
{
  delay(100);
  lcd.clear();
  lcd.print("Tracking:");
  lcd.setCursor(0, 1);
  lcd.print(beacon_idx);
  lcd.print("/");
  lcd.print(system_manager::max_device);

  if (control_alarm(beacon_idx))
  {
    alarm_screen();
  }
}

void lcd_manager::alarm_screen()
{
  delay(100);
  lcd.clear();
  lcd.print("!!!ALARM!!!");
  lcd.setCursor(0, 1);
  lcd.print("Buzzer Triggered!");
}

void lcd_manager::counter_up()
{
  beacon_idx++;
  tracking_screen();
  Serial.print("beacon_idx = ");
  Serial.print(beacon_idx);
  Serial.println("max_device = ");
  Serial.print(system_manager::max_device);
}

void lcd_manager::counter_down()
{
  beacon_idx--;
  tracking_screen();
}
