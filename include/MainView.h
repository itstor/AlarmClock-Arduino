#ifndef MAINVIEW_H
#define MAINVIEW_H

#define TEMP_UPDATE_INTERVAL 1000

#include <View.h>

class MainView : public View
{
public:
  void handleEvent(int event) override;
  void onStart() override;
  void onUpdate() override;
  void onDisplay() override;
  void onResume() override;
  void onDestroy() override;

private:
  unsigned long mLastUpdate = 0;
  double mTemp = 0;
  bool mAnimateClock = false;
  bool mShowDate = false;
  bool mShowTemp = false;
};

#endif