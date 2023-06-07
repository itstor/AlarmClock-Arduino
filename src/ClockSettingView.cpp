#include "ClockSettingView.h"
#include "ViewManager.h"
#include "Application.h"
#include <MD_Parola.h>
#include "Config.h"
#include <RTClib.h>
#include "MainView.h"

void ClockSettingView::handleEvent(int event)
{
  switch (event)
  {
  case MENU_BUTTON:
    mVm->finish();
    break;
  case ENTER_BUTTON:
    formatDateTime();
    mIndex++;
    if (mIndex > 5)
    {
      context->getRtc()->adjust(mDateTime);
      mVm->start<MainView>(false, true);
    }
    break;
  case UP_BUTTON:
    if (mIndex == 0)
    {
      mDateTime = mDateTime + TimeSpan(0, 1, 0, 0);
    }
    else if (mIndex == 1)
    {
      mDateTime = mDateTime + TimeSpan(0, 0, 1, 0);
    }
    else if (mIndex == 2)
    {
      mDateTime = mDateTime + TimeSpan(0, 0, 0, 1);
    }
    else if (mIndex == 3)
    {
      mDateTime = mDateTime + TimeSpan(1, 0, 0, 0);
    }
    else if (mIndex == 4)
    {
      bool isLeapYear = (mDateTime.year() % 4 == 0) && ((mDateTime.year() % 100 != 0) || (mDateTime.year() % 400 == 0));

      if (mDateTime.month() == 2)
      {
        if (isLeapYear)
        {
          mDateTime = mDateTime + TimeSpan(29, 0, 0, 0);
        }
        else
        {
          mDateTime = mDateTime + TimeSpan(28, 0, 0, 0);
        }
      }
      else if (mDateTime.month() == 4 || mDateTime.month() == 6 || mDateTime.month() == 9 || mDateTime.month() == 11)
      {
        mDateTime = mDateTime + TimeSpan(30, 0, 0, 0);
      }
      else
      {
        mDateTime = mDateTime + TimeSpan(31, 0, 0, 0);
      }
    }
    else if (mIndex == 5)
    {
      bool isLeapYear = (mDateTime.year() % 4 == 0) && ((mDateTime.year() % 100 != 0) || (mDateTime.year() % 400 == 0));
      bool isNextLeapYear = ((mDateTime.year() + 1) % 4 == 0) && (((mDateTime.year() + 1) % 100 != 0) || ((mDateTime.year() + 1) % 400 == 0));

      if (isLeapYear && !isNextLeapYear && mDateTime.month() == 2 && mDateTime.day() == 29)
      {
        mDateTime = DateTime(mDateTime.year() + 1, 2, 28, mDateTime.hour(), mDateTime.minute(), mDateTime.second());
      }
      else
      {
        mDateTime = mDateTime + TimeSpan(365, 0, 0, 0);
      }
    }
    formatDateTime();
    break;
  case DOWN_BUTTON:
    if (mIndex == 0)
    {
      mDateTime = mDateTime - TimeSpan(0, 1, 0, 0);
    }
    else if (mIndex == 1)
    {
      mDateTime = mDateTime - TimeSpan(0, 0, 1, 0);
    }
    else if (mIndex == 2)
    {
      mDateTime = mDateTime - TimeSpan(0, 0, 0, 1);
    }
    else if (mIndex == 3)
    {
      mDateTime = mDateTime - TimeSpan(1, 0, 0, 0);
    }
    else if (mIndex == 4)
    {
      bool isLeapYear = (mDateTime.year() % 4 == 0) && ((mDateTime.year() % 100 != 0) || (mDateTime.year() % 400 == 0));

      if (mDateTime.month() == 3)
      {
        if (isLeapYear)
        {
          mDateTime = mDateTime - TimeSpan(29, 0, 0, 0);
        }
        else
        {
          mDateTime = mDateTime - TimeSpan(28, 0, 0, 0);
        }
      }
      else if (mDateTime.month() == 5 || mDateTime.month() == 7 || mDateTime.month() == 10 || mDateTime.month() == 12)
      {
        mDateTime = mDateTime - TimeSpan(30, 0, 0, 0);
      }
      else
      {
        mDateTime = mDateTime - TimeSpan(31, 0, 0, 0);
      }
    }
    else if (mIndex == 5)
    {
      bool isLeapYear = (mDateTime.year() % 4 == 0) && ((mDateTime.year() % 100 != 0) || (mDateTime.year() % 400 == 0));
      bool isPrevLeapYear = ((mDateTime.year() - 1) % 4 == 0) && (((mDateTime.year() - 1) % 100 != 0) || ((mDateTime.year() - 1) % 400 == 0));

      if (!isLeapYear && isPrevLeapYear && mDateTime.month() == 2 && mDateTime.day() == 29)
      {
        mDateTime
            = DateTime(mDateTime.year() - 1, 2, 28, mDateTime.hour(), mDateTime.minute(), mDateTime.second());
      }
      else
      {
        mDateTime = mDateTime - TimeSpan(365, 0, 0, 0);
      }
    }

  default:
    break;
  }
}

void ClockSettingView::onStart()
{
  getDisplay()->displayClear();
  mDateTime = context->getRtc()->now();
  formatDateTime();
}

void ClockSettingView::onUpdate()
{
  if (mIndex > MAX_INDEX)
    mIndex = MAX_INDEX;
  else if (mIndex < 0)
    mIndex = 0;
}

void ClockSettingView::onDisplay()
{
  // Blink the selected index
  unsigned long currentMillis = millis();
  if (currentMillis - mLastBlink >= 500)
  {
    mLastBlink = currentMillis;
    if (mIndex == 0)
    {
      if (time[0] == '_' && time[1] == '_')
      {
        time[0] = '0' + mDateTime.hour() / 10;
        time[1] = '0' + mDateTime.hour() % 10;
      }
      else
      {
        time[0] = '_';
        time[1] = '_';
      }
    }
    else if (mIndex == 1)
    {
      if (time[3] == '_' && time[4] == '_')
      {
        time[3] = '0' + mDateTime.minute() / 10;
        time[4] = '0' + mDateTime.minute() % 10;
      }
      else
      {
        time[3] = '_';
        time[4] = '_';
      }
    }
    else if (mIndex == 2)
    {
      if (time[6] == '_' && time[7] == '_')
      {
        time[6] = '0' + mDateTime.second() / 10;
        time[7] = '0' + mDateTime.second() % 10;
      }
      else
      {
        time[6] = '_';
        time[7] = '_';
      }
    }
    else if (mIndex == 3)
    {
      if (date[0] == '_' && date[1] == '_')
      {
        date[0] = '0' + mDateTime.day() / 10;
        date[1] = '0' + mDateTime.day() % 10;
      }
      else
      {
        date[0] = '_';
        date[1] = '_';
      }
    }
    else if (mIndex == 4)
    {
      if (date[3] == '_' && date[4] == '_')
      {
        date[3] = '0' + mDateTime.month() / 10;
        date[4] = '0' + mDateTime.month() % 10;
      }
      else
      {
        date[3] = '_';
        date[4] = '_';
      }
    }
    else if (mIndex == 5)
    {
      if (date[6] == '_' && date[7] == '_')
      {
        date[6] = '0' + mDateTime.year() / 10 % 10;
        date[7] = '0' + mDateTime.year() % 10;
      }
      else
      {
        date[6] = '_';
        date[7] = '_';
      }
    }
  }

  if (getDisplay()->displayAnimate())
  {
    if (mIndex < MAX_TIME_INDEX)
      getDisplay()->displayText(time, PA_LEFT, getDisplay()->getSpeed(), 0, PA_NO_EFFECT, PA_NO_EFFECT);
    else
      getDisplay()->displayText(date, PA_LEFT, getDisplay()->getSpeed(), 0, PA_NO_EFFECT, PA_NO_EFFECT);
  }
}

void ClockSettingView::onResume()
{
  getDisplay()->displayClear();
}

void ClockSettingView::onDestroy()
{
}

void ClockSettingView::formatDateTime()
{
  // Primitive time formatting. It's too expensive to use sprintf or dtostrf
  if (mDateTime.hour() < 10)
  {
    time[0] = '0';
    itoa(mDateTime.hour(), time + 1, 10);
  }
  else
    itoa(mDateTime.hour(), time, 10);
  strcat(time, ":");
  if (mDateTime.minute() < 10)
    strcat(time, "0");
  itoa(mDateTime.minute(), time + strlen(time), 10);
  strcat(time, ":");
  if (mDateTime.second() < 10)
    strcat(time, "0");
  itoa(mDateTime.second(), time + strlen(time), 10);

  // Primitive date formatting. It's too expensive to use sprintf or dtostrf
  if (mDateTime.day() < 10)
  {
    date[0] = '0';
    itoa(mDateTime.day(), date + 1, 10);
  }
  else
    itoa(mDateTime.day(), date, 10);
  strcat(date, "/");
  if (mDateTime.month() < 10)
    strcat(date, "0");
  itoa(mDateTime.month(), date + strlen(date), 10);
  strcat(date, "/");
  itoa(mDateTime.year() % 100, date + strlen(date), 10); // Display only the last two digits of the year
}
