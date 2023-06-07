#ifndef SETTINGVIEW_H
#define SETTINGVIEW_H

#define SETTINGVIEW_MAX_INDEX 3
#define TIME_INDEX 0
#define ALARM_INDEX 1
#define ABOUT_INDEX 2
#define RESET_ALARM_INDEX 3

#include <View.h>

class MenuView : public View
{
public:
  void handleEvent(int event) override;
  void onStart() override;
  void onUpdate() override;
  void onDisplay() override;
  void onResume() override;
  void onDestroy() override;

private:
  short int mIndex = 0;
};

#endif