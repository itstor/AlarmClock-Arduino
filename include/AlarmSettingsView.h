#ifndef ALARMSETTINGVIEW_H
#define ALARMSETTINGVIEW_H

#include <View.h>
#include <RTClib.h>
#include "Config.h"

#define MAX_ALARM_CHOICES 2

#define ALARM_1_INDEX 0
#define ALARM_2_INDEX 1
#define ALARM_3_INDEX 2

#define MAX_ALARM_POINTER 2

class AlarmSettingsView : public View
{
public:
  void handleEvent(int event) override;
  void onStart() override;
  void onUpdate() override;
  void onDisplay() override;
  void onResume() override;
  void onDestroy() override;

  void formatAlarmTime();

private:
  unsigned short mSelectedIndex = 0;
  short mAlarmPointer = -1;
  unsigned short mAlarmDuration = 0;
  unsigned long mLastBlink = 0;
  char mAlarmTime[6] = {0};
  char mDuration[6] = {0};
  char mAlarmCustomText[MAX_ADDITIONAL_DATA_LENGTH + 1] = "";

  DateTime mDateTime;
  void handleMenuButton();
  void handleUpButton();
  void handleDownButton();
  void handleEnterButton();
  void resetAlarmSettings();
  void adjustAlarmTime(int delta);
  void initAlarmSettings();
  void confirmAlarmSettings();
  void saveAndExit();
};

#endif