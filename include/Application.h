#ifndef APPLICATION_H
#define APPLICATION_H

#include <MD_Parola.h>
#include "Viewmanager.h"
#include "AlarmSettings.h"

class DateTime;
class RTC_DS3231;
class PS2Keyboard;

class Application
{
public:
  Application(MD_Parola &P);
  virtual ~Application();
  Application(const Application &) = delete;
  Application &operator=(const Application &) = delete;

  void setup();
  void loop();

  RTC_DS3231 *getRtc();
  double getTemperature();
  void setAlarmSettings(int index, DateTime time, int duration);
  void setAlarmSettings(int index, DateTime time, int duration, char *additionalData);
  void resetAlarmSettings();
  AlarmSettings *getAlarmSettings();
  AlarmSettings *getAlarmSettings(int index);
  PS2Keyboard *getKeyboard();

private:
  AlarmSettings alarmSettings[MAX_ALARM_SETTINGS];
  PS2Keyboard *keyboard;
  ViewManager *mVm;
  RTC_DS3231 *rtc;

  void alarmLoop();
  void ldrLoop();

  int readEvent();
};

#endif
