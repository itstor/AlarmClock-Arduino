#ifndef VIEWMANAGER_H
#define VIEWMANAGER_H

#include <StackArray.h>
#include "View.h"
#include <ArxTypeTraits.h>

class MD_Parola;
class Application;

class ViewManager
{
public:
  explicit ViewManager(MD_Parola *p);
  ~ViewManager();

  ViewManager(const ViewManager &) = delete;
  ViewManager &operator=(const ViewManager &) = delete;

  template <class T>
  void start(bool replace = false, bool startOver = false);

  void finish();
  void update();
  void handleEvent(int event);

  MD_Parola *getDisplay();

private:
  MD_Parola *mDisplay;
  View *mCurrentView;
  Application *context;
  StackArray<View *> mStack;

  friend class Application;
};

template <class T>
void ViewManager::start(bool replace, bool startOver)
{
  if (!mStack.isEmpty())
  {
    View *currentView = mStack.peek();
    if (std::is_same<T, decltype(*currentView)>::value)
    {
      return;
    }

    if (replace)
    {
      currentView->onDestroy();
      delete currentView;
      mStack.pop();
    }
  }

  if (startOver)
  {
    for (int i = 0; i < mStack.count(); i++)
    {
      View *currentView = mStack.peek();
      currentView->onDestroy();
      delete currentView;
      mStack.pop();
    }
  }

  View *newView = new T();
  newView->context = context;
  newView->mVm = this;
  mStack.push(newView);
  newView->onStart();
  newView->onResume();
  mCurrentView = newView;
}

#endif