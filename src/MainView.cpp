#include "MainView.h"
#include "ViewManager.h"
#include "Application.h"
#include <MD_Parola.h>
#include "Config.h"
#include "MenuView.h"
#include <RTClib.h>

void MainView::handleEvent(int event)
{
  switch (event)
  {
  case MENU_BUTTON:
    mVm->start<MenuView>();
    break;

  default:
    break;
  }
}

void MainView::onStart()
{
  getDisplay()->displayClear();
  getDisplay()->displayReset();
}

void MainView::onUpdate()
{
  const unsigned long now = millis();

  // Get temperature
  if (now - mLastUpdate > TEMP_UPDATE_INTERVAL)
  {
    mLastUpdate = now;
    mTemp = context->getTemperature();
  }
}

void MainView::onDisplay()
{
  const DateTime now = context->getRtc()->now();

  char time[9];
  char date[11];
  char temp[6];

  // Primitive time formatting. It's too expensive to use sprintf or dtostrf
  if (now.hour() < 10)
  {
    time[0] = '0';
    itoa(now.hour(), time + 1, 10);
  }
  else
    itoa(now.hour(), time, 10);
  strcat(time, ":");
  if (now.minute() < 10)
    strcat(time, "0");
  itoa(now.minute(), time + strlen(time), 10);
  strcat(time, ":");
  if (now.second() < 10)
    strcat(time, "0");
  itoa(now.second(), time + strlen(time), 10);

  // Primitive date formatting. It's too expensive to use sprintf or dtostrf
  if (now.day() < 10)
    strcat(date, "0");
  itoa(now.day(), date, 10);
  strcat(date, "/");
  if (now.month() < 10)
    strcat(date, "0");
  itoa(now.month(), date + strlen(date), 10);
  strcat(date, "/");
  itoa(now.year() % 100, date + strlen(date), 10); // Display only the last two digits of the year

  itoa(mTemp, temp, 10);
  strcat(temp, "©");

  if ((now.second() >= 10 && now.second() <= 13) || (now.second() >= 40 && now.second() <= 43))
  {
    if (getDisplay()->displayAnimate())
      getDisplay()->displayText(date, PA_CENTER, 0, 0, PA_NO_EFFECT, PA_NO_EFFECT);
  }
  else if ((now.second() >= 13 && now.second() <= 16) || (now.second() >= 43 && now.second() <= 46))
  {
    if (getDisplay()->displayAnimate())
    {
      getDisplay()->displayText(temp, PA_CENTER, 0, 0, PA_NO_EFFECT, PA_NO_EFFECT);
      mAnimateClock = true;
    }
  }
  else
  {
    if (getDisplay()->displayAnimate())
    {
      getDisplay()->displayText(time, PA_CENTER, mAnimateClock ? 50 : 0, mAnimateClock ? 500 : 0, mAnimateClock ? PA_SCROLL_LEFT : PA_NO_EFFECT, PA_NO_EFFECT);
      mAnimateClock = false;
    }
  }
}

void MainView::onResume()
{
  getDisplay()->displayClear();
}

void MainView::onDestroy()
{
}