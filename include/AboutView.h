#ifndef ABOUTVIEW_H
#define ABOUTVIEW_H

#include <View.h>

class AboutView : public View
{
public:
  void handleEvent(int event) override;
  void onStart() override;
  void onUpdate() override;
  void onDisplay() override;
  void onResume() override;
  void onDestroy() override;

private:
  bool mIsFinished = false;
};

#endif