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
 
#pragma once

typedef struct {
  uint16_t nightModeStart;
  uint8_t nightModeBright;
  uint16_t dayModeStart;
  uint8_t dayModeBright;
  char wifiSSID[33];
  char wifiKey[33];
  char ntpServer[33];
  uint32_t ntpInterval;
} clockConfig_t;
