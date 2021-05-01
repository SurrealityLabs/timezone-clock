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
 
#ifndef SURREALITYLABS_DATETIME_VALIDATOR
#define SURREALITYLABS_DATETIME_VALIDATOR

#include <Arduino.h>

uint8_t validateDate(uint16_t yearNum, uint8_t monthNum, uint8_t dayNum);
uint8_t validateTime(uint8_t hourNum, uint8_t minNum, uint8_t secNum);

#endif
