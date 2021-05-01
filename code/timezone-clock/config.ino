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

#include "config_struct.h"

#include <FS.h>
#include <ArduinoJson.h>

const char *configFile = "/config.json";
const clockConfig_t defaultConfig = { 2330, 0, 800, 40, "", "", "2.pool.ntp.org", 24 };

void loadConfig(clockConfig_t *config)
{
  File f;
  bool loadDefault = false;
  if (!SPIFFS.exists(configFile))
  {
    loadDefault = true;
  }
  else
  {
    f = SPIFFS.open(configFile, "r");

    StaticJsonDocument<1024> doc;

    // Deserialize the JSON document
    DeserializationError error = deserializeJson(doc, f);
    if (error)
    {
      Serial.println(F("Failed to read file, using default configuration"));
      loadDefault = true;
    }
    else
    {
      config->nightModeStart = doc["nightModeStart"];
      config->nightModeBright = doc["nightModeBright"];
      config->dayModeStart = doc["dayModeStart"];
      config->dayModeBright = doc["dayModeBright"];
      strlcpy(config->wifiSSID, doc["wifiSSID"] | "", 32);
      strlcpy(config->wifiKey, doc["wifiKey"] | "", 32);
      strlcpy(config->ntpServer, doc["ntpServer"] | "", 32);
      config->ntpInterval = doc["ntpInterval"];
    }
  
    f.close();
  }

  if (loadDefault)
  {
    /* Load default config and save to file */
    config->nightModeStart = defaultConfig.nightModeStart;
    config->nightModeBright = defaultConfig.nightModeBright;
    config->dayModeStart = defaultConfig.dayModeStart;
    config->dayModeBright = defaultConfig.dayModeBright;
    strncpy(config->wifiSSID, defaultConfig.wifiSSID, 32);
    strncpy(config->wifiKey, defaultConfig.wifiKey, 32);
    strncpy(config->ntpServer, defaultConfig.ntpServer, 32);
    config->ntpInterval = defaultConfig.ntpInterval;
    saveConfig(config);
  }
}

void saveConfig(clockConfig_t *config)
{
  SPIFFS.remove(configFile);
  File f = SPIFFS.open(configFile, "w+");
  
  if (f)
  {
    StaticJsonDocument<1024> doc;
    doc["nightModeStart"] = config->nightModeStart;
    doc["nightModeBright"] = config->nightModeBright;
    doc["dayModeStart"] = config->dayModeStart;
    doc["dayModeBright"] = config->dayModeBright;
    doc["wifiSSID"] = config->wifiSSID;
    doc["wifiKey"] = config->wifiKey;
    doc["ntpServer"] = config->ntpServer;
    doc["ntpInterval"] = config->ntpInterval;

    if (serializeJson(doc, f) == 0) {
      Serial.println(F("Failed to write to file"));
    }

    f.close();
  }
}
