#include "AlarmSettingsView.h"
#include "ViewManager.h"
#include "Application.h"
#include "MainView.h"
#include <MD_Parola.h>
#include "Config.h"
#include <PS2Keyboard.h>

void AlarmSettingsView::handleEvent(int event)
{
  switch (event)
  {
  case MENU_BUTTON:
    handleMenuButton();
    break;
  case UP_BUTTON:
    handleUpButton();
    break;
  case DOWN_BUTTON:
    handleDownButton();
    break;
  case ENTER_BUTTON:
    handleEnterButton();
    break;
  default:
    break;
  }
}

void AlarmSettingsView::handleMenuButton()
{
  if (mAlarmPointer == -1)
  {
    mVm->finish();
  }
  else
  {
    resetAlarmSettings();
  }
}

void AlarmSettingsView::handleUpButton()
{
  if (mAlarmPointer == -1)
  {
    mSelectedIndex++;
  }
  else
  {
    adjustAlarmTime(1);
  }
}

void AlarmSettingsView::handleDownButton()
{
  if (mAlarmPointer == -1)
  {
    mSelectedIndex--;
  }
  else
  {
    adjustAlarmTime(-1);
  }
}

void AlarmSettingsView::handleEnterButton()
{
  if (mAlarmPointer == -1)
  {
    initAlarmSettings();
  }
  else
  {
    confirmAlarmSettings();
  }
}

void AlarmSettingsView::resetAlarmSettings()
{
  mAlarmPointer = -1;
  mDateTime = DateTime();
  mAlarmCustomText[0] = '\0';
  mAlarmDuration = 0;
}

void AlarmSettingsView::adjustAlarmTime(int delta)
{
  if (mAlarmPointer == 0)
  {
    mDateTime = mDateTime + TimeSpan(0, delta, 0, 0);
  }
  else if (mAlarmPointer == 1)
  {
    mDateTime = mDateTime + TimeSpan(0, 0, delta, 0);
  }
  else if (mAlarmPointer == 2)
  {
    mAlarmDuration = constrain(mAlarmDuration + delta, 0, 60);
  }
  formatAlarmTime();
}

void AlarmSettingsView::initAlarmSettings()
{
  mAlarmPointer = 0;
  mAlarmDuration = context->getAlarmSettings(mSelectedIndex)->duration;
  mDateTime = DateTime(1, 1, 1, context->getAlarmSettings(mSelectedIndex)->hour, context->getAlarmSettings(mSelectedIndex)->minute, 0);
  strcpy(mAlarmCustomText, context->getAlarmSettings(mSelectedIndex)->additionalData);
}

void AlarmSettingsView::confirmAlarmSettings()
{
  if (mAlarmPointer < 2)
  {
    mAlarmPointer++;
    formatAlarmTime();
  }
  else if (mAlarmPointer == 2)
  {
    if (mSelectedIndex != 2)
    {
      saveAndExit();
    }
    else
    {
      mAlarmPointer++;
      formatAlarmTime();
    }
  }
  else if (mAlarmPointer == 3 && mSelectedIndex == 2)
  {
    saveAndExit();
  }
}

void AlarmSettingsView::saveAndExit()
{
  context->setAlarmSettings(mSelectedIndex, mDateTime, mAlarmDuration, mAlarmCustomText);
  mVm->start<MainView>(false, true);
}

void AlarmSettingsView::onStart()
{
  getDisplay()->displayClear();
  mDateTime = DateTime();
  formatAlarmTime();
}

void AlarmSettingsView::onUpdate()
{
  if (mSelectedIndex < 0)
    mSelectedIndex = MAX_ALARM_CHOICES;

  if (mSelectedIndex > MAX_ALARM_CHOICES)
    mSelectedIndex = 0;

  if (mSelectedIndex == 2 && mAlarmPointer == 3)
  {
    if (context->getKeyboard()->available())
    {
      char c = context->getKeyboard()->read();
      // append to the custom text, ignore enter and tab. use backspace
      if (c != PS2_ENTER && c != PS2_TAB && c != PS2_BACKSPACE)
      {
        if (strlen(mAlarmCustomText) < MAX_ADDITIONAL_DATA_LENGTH)
        {
          mAlarmCustomText[strlen(mAlarmCustomText)] = c;
        }
      }
      else if (c == PS2_BACKSPACE)
      {
        if (strlen(mAlarmCustomText) > 0)
        {
          mAlarmCustomText[strlen(mAlarmCustomText) - 1] = '\0';
        }
      }
    }
  }
}

void AlarmSettingsView::onDisplay()
{
  if (mAlarmPointer == -1) // Not Selected
  {
    if (getDisplay()->displayAnimate())
    {
      switch (mSelectedIndex)
      {
      case ALARM_1_INDEX:
        getDisplay()->displayText("> 1", PA_LEFT, getDisplay()->getSpeed(), getDisplay()->getPause(), PA_NO_EFFECT, PA_NO_EFFECT);
        break;
      case ALARM_2_INDEX:
        getDisplay()->displayText("> 2", PA_LEFT, getDisplay()->getSpeed(), getDisplay()->getPause(), PA_NO_EFFECT, PA_NO_EFFECT);
        break;
      case ALARM_3_INDEX:
        getDisplay()->displayText("> 3", PA_LEFT, getDisplay()->getSpeed(), getDisplay()->getPause(), PA_NO_EFFECT, PA_NO_EFFECT);
        break;
      default:
        break;
      }
    }
  }
  else // Selected
  {
    // Blink the selected index
    unsigned long currentMillis = millis();
    if (currentMillis - mLastBlink >= 500)
    {
      mLastBlink = currentMillis;
      if (mAlarmPointer == 0)
      {
        if (mAlarmTime[0] == '_' && mAlarmTime[1] == '_')
        {
          mAlarmTime[0] = '0' + mDateTime.hour() / 10;
          mAlarmTime[1] = '0' + mDateTime.hour() % 10;
        }
        else
        {
          mAlarmTime[0] = '_';
          mAlarmTime[1] = '_';
        }
      }
      else if (mAlarmPointer == 1)
      {
        if (mAlarmTime[3] == '_' && mAlarmTime[4] == '_')
        {
          mAlarmTime[3] = '0' + mDateTime.minute() / 10;
          mAlarmTime[4] = '0' + mDateTime.minute() % 10;
        }
        else
        {
          mAlarmTime[3] = '_';
          mAlarmTime[4] = '_';
        }
      }
    }

    if (getDisplay()->displayAnimate())
    {
      if (mAlarmPointer < 2)
      {
        getDisplay()->displayText(mAlarmTime, PA_CENTER, getDisplay()->getSpeed(), 0, PA_NO_EFFECT, PA_NO_EFFECT);
      }
      else if (mAlarmPointer == 2)
      {
        itoa(mAlarmDuration, mDuration, 10);
        strcat(mDuration, " s");
        getDisplay()->displayText(mDuration, PA_CENTER, getDisplay()->getSpeed(), 0, PA_NO_EFFECT, PA_NO_EFFECT);
      }
      else if (mAlarmPointer == 3)
      {
        getDisplay()->print(mAlarmCustomText);
      }
    }
  }
}

void AlarmSettingsView::onResume()
{
  getDisplay()->displayClear();
}

void AlarmSettingsView::onDestroy()
{
}

void AlarmSettingsView::formatAlarmTime()
{
  if (mDateTime.hour() < 10)
  {
    mAlarmTime[0] = '0';
    itoa(mDateTime.hour(), mAlarmTime + 1, 10);
  }
  else
    itoa(mDateTime.hour(), mAlarmTime, 10);
  strcat(mAlarmTime, ":");
  if (mDateTime.minute() < 10)
    strcat(mAlarmTime, "0");
  itoa(mDateTime.minute(), mAlarmTime + strlen(mAlarmTime), 10);
}
