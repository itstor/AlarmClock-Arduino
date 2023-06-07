#ifndef CLOCKSETTING_H
#define CLOCKSETTING_H

#include <View.h>

#include <RTClib.h>

#define BLINK_INTERVAL 500

#define MAX_INDEX 6
#define MAX_TIME_INDEX 3
#define MAX_DATE_INDEX 6

class ClockSettingView : public View
{
public:
  void handleEvent(int event) override;
  void onStart() override;
  void onUpdate() override;
  void onDisplay() override;
  void onResume() override;
  void onDestroy() override;

private:
  void formatDateTime();

  unsigned short mIndex = 0;
  unsigned long mLastBlink = 0;
  DateTime mDateTime;
  char time[9] = {0};
  char date[11] = {0};
};

#endif