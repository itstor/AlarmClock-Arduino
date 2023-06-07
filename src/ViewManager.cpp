#include "ViewManager.h"
#include "View.h"
#include <ArxTypeTraits.h>

ViewManager::ViewManager(MD_Parola *p) : mDisplay(p), mCurrentView(nullptr) {}

ViewManager::~ViewManager()
{
  while (!mStack.isEmpty())
  {
    View *view = mStack.pop();
    view->onDestroy();
    delete view;
  }
}

void ViewManager::finish()
{
  if (!mStack.isEmpty())
  {
    View *view = mStack.pop();
    view->onDestroy();
    delete view;
    if (!mStack.isEmpty())
    {
      mCurrentView = mStack.peek();
      mCurrentView->resume();
    }
    else
    {
      mCurrentView = nullptr;
    }
  }
}

void ViewManager::update()
{
  if (mCurrentView != nullptr && !mCurrentView->isPaused())
  {
    mCurrentView->onUpdate();
    mCurrentView->onDisplay();
  }
}

void ViewManager::handleEvent(int event)
{
  if (mCurrentView != nullptr && !mCurrentView->isPaused())
  {
    mCurrentView->handleEvent(event);
  }
}

MD_Parola *ViewManager::getDisplay()
{
  return mDisplay;
}