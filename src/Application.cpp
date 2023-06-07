#include "Application.h"
#include "Config.h"
#include "AboutView.h"
#include "MainView.h"
#include "RTClib.h"
#include <EEPROM.h>
#include <PS2Keyboard.h>

Application::Application(MD_Parola &P) : keyboard(new PS2Keyboard()), mVm(new ViewManager(&P)), rtc(new RTC_DS3231()) {}

Application::~Application()
{
  delete mVm;
}

void Application::setup()
{
  keyboard->begin(KEYBOARD_DATA_PIN, KEYBOARD_CLOCK_PIN);
  mVm->getDisplay()->begin();
  mVm->context = this;

  pinMode(UP_BUTTON, INPUT);
  pinMode(DOWN_BUTTON, INPUT);
  pinMode(MENU_BUTTON, INPUT);
  pinMode(ENTER_BUTTON, INPUT);

  pinMode(LM35_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(BUZZER_PIN, HIGH);

  if (!rtc->begin())
  {
#if DEBUG
    Serial.println("Couldn't find RTC");
#endif
    Serial.flush();
    while (1)
      delay(10);
  }

  if (rtc->lostPower())
  {
#if DEBUG
    Serial.println("RTC lost power, let's set the time!");
#endif
    rtc->adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  EEPROM.get(EEPROM_ADDRESS, alarmSettings);

  mVm->getDisplay()->setSpeed(50);
  mVm->start<AboutView>();
}

void Application::loop()
{
  ldrLoop();
  alarmLoop();
  int event = readEvent();
  mVm->handleEvent(event);
  mVm->update();
}

void Application::alarmLoop()
{
  for (int i = 0; i < MAX_ALARM_SETTINGS; i++)
  {
    if (alarmSettings[i].hour == rtc->now().hour() && alarmSettings[i].minute == rtc->now().minute() && alarmSettings[i].enabled)
    {
      mVm->getDisplay()->displayClear();
      unsigned long startTime = millis();
      while (digitalRead(ENTER_BUTTON) == HIGH)
      {
        if (mVm->getDisplay()->displayAnimate())
        {
          const int displaySpeed = 20;
          switch (i)
          {
          case 0:
            mVm->getDisplay()->displayText("5024201028 ", PA_LEFT, displaySpeed, 0, PA_SCROLL_LEFT, PA_NO_EFFECT);
            break;
          case 1:
            mVm->getDisplay()->displayText("5024201028 A Thoriq A ", PA_LEFT, displaySpeed, 0, PA_SCROLL_LEFT, PA_NO_EFFECT);
            break;
          case 2:
            mVm->getDisplay()->displayText(alarmSettings[i].additionalData, PA_LEFT, displaySpeed, 0, PA_SCROLL_LEFT, PA_NO_EFFECT);
            break;
          default:
            break;
          }
        }

        unsigned long elapsedTime = millis() - startTime;
        if (elapsedTime % 1000 < 500)
        {
          digitalWrite(BUZZER_PIN, HIGH);
        }
        else
        {
          digitalWrite(BUZZER_PIN, LOW);
        }
        if (elapsedTime >= alarmSettings[i].duration * 1000)
        {
          break;
        }
      }
      alarmSettings[i].enabled = false;
      EEPROM.put(EEPROM_ADDRESS, alarmSettings);
      digitalWrite(BUZZER_PIN, HIGH); // Make sure buzzer is off

      mVm->getDisplay()->displayClear();
      mVm->getDisplay()->displayReset();
      mVm->start<MainView>(false, true);
    }
  }
}

void Application::ldrLoop()
{
  static unsigned long previousMillis = 0;
  static int targetIntensity = 0;
  const unsigned long interval = 500;
  float smoothness = 0.5;
  static uint8_t currentIntensity = 0;

  auto lerp = [](float start, float end, float t)
  {
    return (1 - t) * start + t * end;
  };

  if (millis() - previousMillis >= interval)
  {
    previousMillis = millis();

    const int ldrValue = analogRead(LDR_PIN);

    targetIntensity = map(ldrValue, 0, 1023, 15, 0);
  }

  uint8_t newIntensity = lerp(currentIntensity, targetIntensity, smoothness);
  currentIntensity = newIntensity;
  mVm->getDisplay()->setIntensity(currentIntensity);
}

int Application::readEvent()
{
  if (digitalRead(UP_BUTTON) == LOW)
  {
    while (digitalRead(UP_BUTTON) == LOW)
      ;
    return UP_BUTTON;
  }
  if (digitalRead(DOWN_BUTTON) == LOW)
  {
    while (digitalRead(DOWN_BUTTON) == LOW)
      ;
    return DOWN_BUTTON;
  }

  if (digitalRead(MENU_BUTTON) == LOW)
  {
    while (digitalRead(MENU_BUTTON) == LOW)
      ;
    return MENU_BUTTON;
  }

  if (digitalRead(ENTER_BUTTON) == LOW)
  {
    while (digitalRead(ENTER_BUTTON) == LOW)
      ;
    return ENTER_BUTTON;
  }

  return 0;
}

RTC_DS3231 *Application::getRtc()
{
  return rtc;
}

double Application::getTemperature()
{
  static double temperatureSum = 0.0;
  static int sampleCount = 0;
  static double previousTemperature = 0.0;
  static unsigned long previousMillis = 0;
  const unsigned long interval = 1000;
  const int numSamples = 5;

  if (millis() - previousMillis >= interval)
  {
    previousMillis = millis();

    double currentTemperature = analogRead(LM35_PIN) * 0.48828125 / 2;
    temperatureSum += currentTemperature;
    sampleCount++;

    if (sampleCount >= numSamples)
    {
      double averageTemperature = temperatureSum / numSamples;
      temperatureSum = 0.0;
      sampleCount = 0;
      previousTemperature = averageTemperature;

      return averageTemperature;
    }
  }

  return previousTemperature;
}

void Application::setAlarmSettings(int index, DateTime time, int duration)
{
  AlarmSettings settings;
  settings.hour = time.hour();
  settings.minute = time.minute();
  settings.duration = duration;
  settings.enabled = true;

  alarmSettings[index] = settings;
  EEPROM.put(EEPROM_ADDRESS, alarmSettings);
}

void Application::setAlarmSettings(int index, DateTime time, int duration, char *additionalData)
{
  AlarmSettings settings;
  settings.hour = time.hour();
  settings.minute = time.minute();
  settings.duration = duration;
  settings.enabled = true;
  strcpy(settings.additionalData, additionalData);

  alarmSettings[index] = settings;
  EEPROM.put(EEPROM_ADDRESS, alarmSettings);
}

void Application::resetAlarmSettings()
{
  for (int i = 0; i < MAX_ALARM_SETTINGS; i++)
  {
    alarmSettings[i].hour = 0;
    alarmSettings[i].minute = 0;
    alarmSettings[i].duration = 0;
    alarmSettings[i].enabled = false;
    strcpy(alarmSettings[i].additionalData, "");
  }
  EEPROM.put(EEPROM_ADDRESS, alarmSettings);
}

AlarmSettings *Application::getAlarmSettings()
{
  return alarmSettings;
}

AlarmSettings *Application::getAlarmSettings(int index)
{
  return &alarmSettings[index];
}

PS2Keyboard *Application::getKeyboard()
{
  return keyboard;
}
