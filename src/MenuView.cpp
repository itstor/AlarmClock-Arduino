#include "MenuView.h"
#include "AboutView.h"
#include "ClockSettingView.h"
#include "AlarmSettingsView.h"
#include "ViewManager.h"
#include "Application.h"
#include <MD_Parola.h>
#include "Config.h"

void MenuView::handleEvent(int event)
{
  switch (event)
  {
  case MENU_BUTTON:
    mVm->finish();
    break;
  case UP_BUTTON:
    mIndex = mIndex + 1;
    break;
  case DOWN_BUTTON:
    mIndex = mIndex - 1;
    break;
  case ENTER_BUTTON:
    switch (mIndex)
    {
    case ABOUT_INDEX:
      mVm->start<AboutView>();
      break;
    case ALARM_INDEX:
      mVm->start<AlarmSettingsView>();
      break;
    case TIME_INDEX:
      mVm->start<ClockSettingView>();
      break;
    case RESET_ALARM_INDEX:
      context->resetAlarmSettings();
      mVm->finish();
      break;
    default:
      break;
    }
    break;
  default:
    break;
  }
}

void MenuView::onStart()
{
}

void MenuView::onUpdate()
{
  if (mIndex < 0)
    mIndex = SETTINGVIEW_MAX_INDEX;

  if (mIndex > SETTINGVIEW_MAX_INDEX)
    mIndex = 0;
}

void MenuView::onDisplay()
{
  if (getDisplay()->displayAnimate())
    switch (mIndex)
    {
    case TIME_INDEX:
      getDisplay()->displayText(">Time", PA_LEFT, getDisplay()->getSpeed(), getDisplay()->getPause(), PA_NO_EFFECT, PA_NO_EFFECT);
      break;
    case ALARM_INDEX:
      getDisplay()->displayText(">Alarm", PA_LEFT, getDisplay()->getSpeed(), getDisplay()->getPause(), PA_NO_EFFECT, PA_NO_EFFECT);
      break;
    case ABOUT_INDEX:
      getDisplay()->displayText(">About", PA_LEFT, getDisplay()->getSpeed(), getDisplay()->getPause(), PA_NO_EFFECT, PA_NO_EFFECT);
      break;
    case RESET_ALARM_INDEX:
      getDisplay()->displayText(">Reset", PA_LEFT, getDisplay()->getSpeed(), getDisplay()->getPause(), PA_NO_EFFECT, PA_NO_EFFECT);
      break;
    default:
      break;
    }
}

void MenuView::onResume()
{
  getDisplay()->displayClear();
}

void MenuView::onDestroy()
{
}