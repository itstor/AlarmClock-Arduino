#include "View.h"
#include "ViewManager.h"

void View::pause()
{
  mPaused = true;
}

void View::resume()
{
  mPaused = false;
  onResume();
}

bool View::isPaused() const
{
  return mPaused;
}

MD_Parola *View::getDisplay()
{
  return mVm->getDisplay();
}
