#ifndef ALARM_SETTINGS_H
#define ALARM_SETTINGS_H

#include "Config.h"

struct AlarmSettings
{
  unsigned short int hour;
  unsigned short int minute;
  unsigned short int duration;
  bool enabled;
  char additionalData[MAX_ADDITIONAL_DATA_LENGTH];
};

#endif