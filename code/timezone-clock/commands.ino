/*
 * This file is part of Timezone Clock.
 *
 * Timezone Clock is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * Timezone Clock is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with Timezone Clock. If not, see <https://www.gnu.org/licenses/>.
 */
 
#include <Commander.h>
#include "dateTimeValidator.h"
#include <Time.h>
#include <TimeLib.h>
#include <Timezone.h>

#include "config_struct.h"

Commander cmd;
extern Timezone ET;

extern time_t lastRebootTime;
extern clockConfig_t clockConfig;
extern void setHwTime(time_t epochTime);

const commandList_t masterCommands[] = {
  {"setDate",               setDateHandler,             "setDate [day] [month] [year]"},
  {"setTime",               setTimeHandler,             "setTime [hours] [minutes] [seconds]"},
  {"printTime",             printTimeHandler,           "printTime"},
  {"setBright",             setBrightHandler,           "setBright [night hour] [night minute] [night brightness] [day hour] [day minute] [day brightness]"},
  {"printBright",           printBrightHandler,         "printBright"},
  {"setWifi",               setWifiHandler,             "setWifi [ssid] [psk]"},
  {"printWifi",             printWifiHandler,           "printWifi"},
  {"printIP",               printIPHandler,             "printIP"},
  {"setNTP",                setNTPHandler,              "setNTP [server] [interval]"},
  {"printNTP",              printNTPHandler,            "printNTP"},
  {"printLastReboot",       printLastRebootHandler,     "printLastReboot"},
};

void command_setup()
{
  cmd.begin(&Serial, masterCommands, sizeof(masterCommands));
  cmd.commandPrompt(ON); //enable the command prompt
  cmd.echo(true);     //Echo incoming characters to theoutput port
  cmd.errorMessages(ON); //error messages are enabled - it will tell us if we issue any unrecognised commands
  cmd.autoChain(ON);
  cmd.delimiters("= ,\t\\|");
  cmd.setBuffer(384);
  cmd.printCommandPrompt();
}

void command_loop()
{
  cmd.update();
}

bool setDateHandler(Commander &Cmdr) {
  if(3 != Cmdr.countItems()) {
    Cmdr.println(F("Insufficient arguments!"));
    return 1;
  }

  uint8_t dayNum;
  uint8_t monthNum;
  uint16_t yearNum;
  Cmdr.getInt(dayNum);
  Cmdr.getInt(monthNum);
  Cmdr.getInt(yearNum);

  uint8_t tmp = validateDate(yearNum, monthNum, dayNum);

  if(tmp == 2) {
    Cmdr.println(F("Invalid year!"));
    return true;
  } else if(tmp == 3) {
    Cmdr.println(F("Invalid month!"));
    return true;
  } else if(tmp == 4) {
    Cmdr.println(F("Invalid day!"));
    return true;
  }
  
  tmElements_t newTime;
  time_t tmp_t;
  tmp_t = now();
  tmp_t = ET.toLocal(tmp_t);
  breakTime(tmp_t, newTime);
  newTime.Year = CalendarYrToTm(yearNum);
  newTime.Month = monthNum;
  newTime.Day = dayNum;
  
  tmp_t = makeTime(newTime);
  tmp_t = ET.toUTC(tmp_t);
  setHwTime(tmp_t);
  setTime(tmp_t);

  Cmdr.print(F("Setting date to "));
  Cmdr.print(dayNum);
  Cmdr.print('/');
  Cmdr.print(monthNum);
  Cmdr.print('/');
  Cmdr.println(yearNum);
  return 0;  
}

bool setTimeHandler(Commander &Cmdr) {
  if(3 != Cmdr.countItems()) {
    Cmdr.println(F("Insufficient arguments!"));
    return 1;
  }

  uint8_t hourNum;
  uint8_t minNum;
  uint16_t secNum;
  Cmdr.getInt(hourNum);
  Cmdr.getInt(minNum);
  Cmdr.getInt(secNum);

  uint8_t tmp = validateTime(hourNum, minNum, secNum);

  if(tmp == 2) {
    Cmdr.println(F("Invalid hours!"));
    return true;
  } else if(tmp == 3) {
    Cmdr.println(F("Invalid minutes!"));
    return true;
  } else if(tmp == 4) {
    Cmdr.println(F("Invalid seconds!"));
    return true;
  }

  tmElements_t newTime;
  time_t tmp_t;
  tmp_t = now();
  tmp_t = ET.toLocal(tmp_t);
  breakTime(tmp_t, newTime);
  newTime.Hour = hourNum;
  newTime.Minute = minNum;
  newTime.Second = secNum;

  tmp_t = makeTime(newTime);
  tmp_t = ET.toUTC(tmp_t);
  setHwTime(tmp_t);
  setTime(tmp_t);


  Cmdr.print(F("Setting time to "));
  Cmdr.print(hourNum);
  Cmdr.print(F(":"));
  Cmdr.print(minNum);
  Cmdr.print(F(":"));
  Cmdr.println(secNum);
  return 0;  
}

bool printTimeHandler(Commander &Cmdr) {
  Cmdr.print(F("The current time is: "));
  char strbuf[32];
  tmElements_t newTime;
  time_t tmp_t;
  tmp_t = now();
  tmp_t = ET.toLocal(tmp_t);
  breakTime(tmp_t, newTime);
  snprintf(strbuf, 32, "%04d-%02d-%02d %02d:%02d:%02d ", tmYearToCalendar(newTime.Year), newTime.Month, newTime.Day, newTime.Hour, newTime.Minute, newTime.Second);
  Cmdr.println(strbuf);
  return 0;
}

bool setBrightHandler(Commander &Cmdr) {
  if(6 != Cmdr.countItems()) {
    Cmdr.println(F("Insufficient arguments!"));
    return 1;
  }

  uint8_t hourNum;
  uint8_t minNum;
  uint8_t secNum = 0;

  Cmdr.getInt(hourNum);
  Cmdr.getInt(minNum);

  uint8_t tmp = validateTime(hourNum, minNum, secNum);

  if(tmp == 2) {
    Cmdr.println(F("Invalid hours!"));
    return true;
  } else if(tmp == 3) {
    Cmdr.println(F("Invalid minutes!"));
    return true;
  }

  clockConfig.nightModeStart = (hourNum * 100) + minNum;

  Cmdr.getInt(tmp);

  clockConfig.nightModeBright = tmp;

  Cmdr.getInt(hourNum);
  Cmdr.getInt(minNum);

  tmp = validateTime(hourNum, minNum, secNum);

  if(tmp == 2) {
    Cmdr.println(F("Invalid hours!"));
    return true;
  } else if(tmp == 3) {
    Cmdr.println(F("Invalid minutes!"));
    return true;
  }

  clockConfig.dayModeStart = (hourNum * 100) + minNum;

  Cmdr.getInt(tmp);

  clockConfig.dayModeBright = tmp;

  saveConfig(&clockConfig);
}

bool printBrightHandler(Commander &Cmdr) {
  Cmdr.print(F("Night mode begins at "));
  Cmdr.print(clockConfig.nightModeStart);
  Cmdr.print(F(" with a brightness of "));
  Cmdr.println(clockConfig.nightModeBright);
  Cmdr.print(F("Day mode begins at "));
  Cmdr.print(clockConfig.dayModeStart);
  Cmdr.print(F(" with a brightness of "));
  Cmdr.println(clockConfig.dayModeBright);
}

bool setWifiHandler(Commander &Cmdr) {
  String tmp = "";
  int itms = Cmdr.countItems();
  
  if(1 > itms) {
    Cmdr.println(F("Insufficient arguments!"));
    return 1;
  }

  Cmdr.getString(tmp);
  itms--;
  strlcpy(clockConfig.wifiSSID, tmp.c_str(), 33);
  if (itms >= 1)
  {
    Cmdr.getString(tmp);
    strlcpy(clockConfig.wifiKey, tmp.c_str(), 33);
  }
  else
  {
    memset(clockConfig.wifiKey, 0, 33);
  }

  saveConfig(&clockConfig);

  if (strlen(clockConfig.wifiSSID) > 0)
  {
    Cmdr.println(F("Connecting to WiFi..."));
    WiFi.mode(WIFI_STA);
    WiFi.begin(clockConfig.wifiSSID, clockConfig.wifiKey);
  }

  return 0;
}

bool printWifiHandler(Commander &Cmdr) {
  Cmdr.print(F("Saved SSID is \""));
  Cmdr.print(clockConfig.wifiSSID);
  Cmdr.println(F("\""));
  Cmdr.print(F("Saved key is \""));
  Cmdr.print(clockConfig.wifiKey);
  Cmdr.println(F("\""));
}

bool printIPHandler(Commander &Cmdr) {
  if (WiFi.status() == WL_CONNECTED)
  {
    Cmdr.print(F("WiFi is connected. IP address is "));
    Cmdr.println(WiFi.localIP());
  }
  else
  {
    Cmdr.println(F("WiFi is not connected."));
  }
}

bool setNTPHandler(Commander &Cmdr) {
  String tmp = "";
  uint32_t tmpInt;
  int itms = Cmdr.countItems();
  
  if(1 > itms) {
    Cmdr.println(F("Insufficient arguments!"));
    return 1;
  }

  Cmdr.getString(tmp);
  itms--;
  strlcpy(clockConfig.ntpServer, tmp.c_str(), 33);
  if (itms >= 1)
  {
    Cmdr.getInt(tmpInt);
    clockConfig.ntpInterval = tmpInt;
  }
  else
  {
    clockConfig.ntpInterval = 86400000ul;
  }

  ntp_config(clockConfig.ntpServer, clockConfig.ntpInterval);

  saveConfig(&clockConfig);

  return 0;
}

bool printNTPHandler(Commander &Cmdr) {
  Cmdr.print(F("Saved server is \""));
  Cmdr.print(clockConfig.ntpServer);
  Cmdr.println(F("\""));
  Cmdr.print(F("Saved interval is "));
  Cmdr.print(clockConfig.ntpInterval);
  Cmdr.println(F(" hours"));
}

bool printLastRebootHandler(Commander &Cmdr) {
  char strbuf[32];
  time_t tmp_t;
  tmElements_t newTime;
  
  Cmdr.print(F("The time of the last reboot was: "));
  tmp_t = ET.toLocal(lastRebootTime);
  breakTime(tmp_t, newTime);
  snprintf(strbuf, 32, "%04d-%02d-%02d %02d:%02d:%02d ", tmYearToCalendar(newTime.Year), newTime.Month, newTime.Day, newTime.Hour, newTime.Minute, newTime.Second);
  Cmdr.println(strbuf);
  return 0;  
}
