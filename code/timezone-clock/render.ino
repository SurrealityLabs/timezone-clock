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
 
#include <Adafruit_NeoPixel.h>
#include <Time.h>
#include <TimeLib.h>
#include <Timezone.h>

extern Timezone NT, AST, AT, ET, EST, CT, CST, MT, MST, PT;

#include "config_struct.h"

#define NEOPIXEL_PIN D4

Adafruit_NeoPixel strip(10, NEOPIXEL_PIN, NEO_GRBW + NEO_KHZ800);

const uint8_t digit_lut[10] = {187, 17, 188, 157, 23, 143, 175, 145, 191, 159};
const uint8_t colon_val = 64;

extern clockConfig_t clockConfig;

void render_setup()
{
  strip.begin();
  strip.fill(0);
  strip.show();
}

void render_clear()
{
  strip.fill(0);
  strip.show();
}

static void render_time_buffer(tmElements_t *this_time, uint8_t *buff)
{
  uint8_t tmp;
  tmp = this_time->Hour;
  if (tmp > 9)
  {
    buff[2] = digit_lut[tmp / 10];
    buff[3] = digit_lut[tmp % 10];
  }
  else
  {
    buff[2] = 0;
    buff[3] = digit_lut[tmp % 10];
  }
  tmp = this_time->Minute;
  buff[0] = digit_lut[tmp / 10];
  buff[1] = digit_lut[tmp % 10];

  if (this_time->Second % 2 == 0)
  {
    buff[3] += colon_val;
  }
}

void render_time(time_t now, uint8_t bright) {
  tmElements_t times[10]; // nt_time, ast_time, at_time, et_time, est_time, ct_time, cst_time, mt_time, mst_time, pt_time;
  time_t tmp_t;
  uint8_t display_tmp[4];
  uint8_t i;
  
  tmp_t = NT.toLocal(now);
  breakTime(tmp_t, times[0]);
  tmp_t = AST.toLocal(now);
  breakTime(tmp_t, times[1]);
  tmp_t = AT.toLocal(now);
  breakTime(tmp_t, times[2]);
  tmp_t = ET.toLocal(now);
  breakTime(tmp_t, times[3]);
  tmp_t = EST.toLocal(now);
  breakTime(tmp_t, times[4]);
  tmp_t = CT.toLocal(now);
  breakTime(tmp_t, times[5]);
  tmp_t = CST.toLocal(now);
  breakTime(tmp_t, times[6]);
  tmp_t = MT.toLocal(now);
  breakTime(tmp_t, times[7]);
  tmp_t = MST.toLocal(now);
  breakTime(tmp_t, times[8]);
  tmp_t = PT.toLocal(now);
  breakTime(tmp_t, times[9]);
 
  yield();

  for(i = 0; i < 10; i++)
  {
    render_time_buffer(&(times[i]), display_tmp);
    if (bright > 0)
    {
      strip.setPixelColor(i, display_tmp[0], display_tmp[1], display_tmp[2], display_tmp[3]);
    }
    else
    {
      strip.setPixelColor(i, 0, 0, 0, 0);
    }
  }

  yield();
  strip.show();
}
