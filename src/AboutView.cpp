#include "AboutView.h"
#include "ViewManager.h"
#include "Application.h"
#include <MD_Parola.h>
#include "Config.h"
#include "MainView.h"

void AboutView::handleEvent(int event)
{
}

void AboutView::onStart()
{
  getDisplay()->displayClear();
}

void AboutView::onUpdate()
{
}

void AboutView::onDisplay()
{
  if (getDisplay()->displayAnimate() && !mIsFinished)
  {
    getDisplay()->displayText("Akhmad Thoriq Afif 5024201028", PA_LEFT, getDisplay()->getSpeed(), 1000, PA_SCROLL_LEFT, PA_NO_EFFECT);
    mIsFinished = true;
  }

  if (!getDisplay()->displayAnimate() && mIsFinished)
  {
    mVm->start<MainView>(false, true);
  }
}

void AboutView::onResume()
{
}

void AboutView::onDestroy()
{
}