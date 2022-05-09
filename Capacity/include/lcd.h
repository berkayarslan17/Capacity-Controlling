#ifndef LCD_H
#define LCD_H

#include <LiquidCrystal_I2C.h>

#define LCD_I2C_ADDRESS 0x27
#define LCD_SCREEN_WIDTH 16
#define LCD_SCREEN_HEIGHT 2

#include <Arduino.h>
#include <vector>

class lcd_manager
{
public:
    lcd_manager();
    ~lcd_manager();
    void idle_screen();
    void tracking_screen();
    void alarm_screen();
    void counter_up();
    void counter_down();

private:
     int beacon_idx;
    
};

class Command
{
public:
    virtual ~Command() {}
    virtual void execute(lcd_manager &screen) = 0;
};

class IdleCommand : public Command
{
public:
    virtual void execute(lcd_manager &screen)
    {
        Serial.println("IDLE Screen");
        screen.idle_screen();
    }
};

class TrackingCommand : public Command
{
public:
    virtual void execute(lcd_manager &screen)
    {
        Serial.println("Tracking Screen");
        screen.tracking_screen();
    }
};

class AlarmCommand : public Command
{
public:
    virtual void execute(lcd_manager &screen)
    {
        Serial.println("Alarm Screen");
        screen.alarm_screen();
    }
};

class CntUpCommand : public Command
{
public:
    virtual void execute(lcd_manager &screen)
    {
        Serial.println("CNT Up Screen");
        screen.counter_up();
    }
};

class CntDownCommand : public Command
{
public:
    virtual void execute(lcd_manager &screen)
    {
        Serial.println("CNT Down Screen");
        screen.counter_down();
    }
};

#endif
