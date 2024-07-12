/* Copyright 2023 splitkb.com <support@splitkb.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

// Myriad boilerplate
#define MYRIAD_ENABLE

// Debounce reduces chatter (unintended double-presses) - set 0 if debouncing is not needed
#define DEBOUNCE 5

// Mirror event and infos to the slave for RGB and OLED
#define SPLIT_TRANSPORT_MIRROR
#define SPLIT_LED_STATE_ENABLE
#define SPLIT_MODS_ENABLE
#define SPLIT_LAYER_STATE_ENABLE

// Tap Dance Options
#define TAPPING_TERM 175
#define TAPPING_TERM_PER_KEY

// OS Detection Options
#define OS_DETECTION_KEYBOARD_RESET

// Custom OLED Font
#define OLED_FONT_H "glcdfont.h"
