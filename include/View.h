#ifndef VIEW_H
#define VIEW_H

class MD_Parola;
class Application;
class ViewManager;

class View
{
public:
  View() = default;
  virtual ~View() = default;

  View(const View &) = delete;
  View &operator=(const View &) = delete;

  virtual void handleEvent(int event);
  virtual void onStart();
  virtual void onUpdate();
  virtual void onDisplay();
  virtual void onResume();
  virtual void onDestroy();

  void pause();
  void resume();
  bool isPaused() const;

  MD_Parola *getDisplay();

protected:
  Application *context;
  ViewManager *mVm;

private:
  bool mPaused = false;

  friend class Application;
  friend class ViewManager;
};

#endif
