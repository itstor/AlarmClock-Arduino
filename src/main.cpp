#include "Config.h"
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <View.h>
#include <ViewManager.h>
#include <Application.h>
#include "MainView.h"

MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

Application app(P);

void setup()
{
#if DEBUG
  Serial.begin(9600);
#endif

  app.setup();
}

void loop()
{
  app.loop();
}