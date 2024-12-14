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

#include QMK_KEYBOARD_H
#include "keymap_german.h"


// OS Detection on USB Init
os_variant_t detected_os = OS_UNSURE;

bool process_detected_host_os_user(os_variant_t os_variant) {
    // locally store detected OS
    detected_os = os_variant;
    return true;
}


enum tap_dance_opts {
    TD_LSHIFT_CAPS,
    TD_RSHIFT_CAPS,
};

tap_dance_action_t tap_dance_actions[] = {
    [TD_LSHIFT_CAPS] = ACTION_TAP_DANCE_DOUBLE(KC_LSFT, KC_CAPS),
    [TD_RSHIFT_CAPS] = ACTION_TAP_DANCE_DOUBLE(KC_RSFT, KC_CAPS),
};

// KEY OVERRIDES

const key_override_t delete_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_BSPC, KC_DEL);
const key_override_t sas_sz_override = ko_make_basic(MOD_MASK_ALT, KC_S, DE_SS);

// This globally defines all key overrides to be used
const key_override_t **key_overrides = (const key_override_t *[]){
	&delete_key_override,
    &sas_sz_override,
	NULL // Null terminate the array of overrides!
};

// Custom Keycodes

enum custom_keycodes {
    XX_HOME = SAFE_RANGE,
    XX_END,
    XX_LCBR,
    XX_RCBR,
    XX_LBRC,
    XX_RBRC,
    XX_BSLS,
    XX_AT,
    XX_TILD,
    XX_PIPE,
    XX_LABK,
    XX_RABK,
    XX_CIRC,
    XX_DEG,
    XX_RGUI,
    XX_LGUI,
    CTL_ADIA,
    CTL_ESC,
    XX_OST, // custom keycode to toggle the OS manually
};

// Two custom macros to increase readability in mapping and lookup
#define XX_MAPPING(from_code, to_code) [from_code - SAFE_RANGE] = to_code
#define XX_LOOKUP(table, keycode) table[keycode - SAFE_RANGE]

// We do the mapping of keycodes via lookup tables
// the macros help us create zero indexed arrays without
// too much verbosity.
const uint16_t PROGMEM mac_keycodes[] = {
    XX_MAPPING(XX_HOME, LGUI(KC_LEFT)),
    XX_MAPPING(XX_END,  LGUI(KC_RIGHT)),
    XX_MAPPING(XX_LCBR, RALT(KC_8)),
    XX_MAPPING(XX_RCBR, RALT(KC_9)),
    XX_MAPPING(XX_LBRC, RALT(KC_5)),
    XX_MAPPING(XX_RBRC, RALT(KC_6)),
    XX_MAPPING(XX_BSLS, RALT(S(KC_7))),
    XX_MAPPING(XX_AT, RALT(KC_L)),
    XX_MAPPING(XX_TILD, RALT(KC_N)),
    XX_MAPPING(XX_PIPE, RALT(KC_7)),
    XX_MAPPING(XX_LABK, DE_CIRC),
    XX_MAPPING(XX_RABK, DE_DEG),
    XX_MAPPING(XX_CIRC, DE_LABK),
    XX_MAPPING(XX_DEG, DE_RABK),
    XX_MAPPING(XX_RGUI, KC_RCTL),
    XX_MAPPING(XX_LGUI, KC_LCTL),
};

const uint16_t PROGMEM other_keycodes[] = {
    XX_MAPPING(XX_HOME, KC_HOME),
    XX_MAPPING(XX_END, KC_END),
    XX_MAPPING(XX_LCBR, DE_LCBR),
    XX_MAPPING(XX_RCBR, DE_RCBR),
    XX_MAPPING(XX_LBRC, DE_LBRC),
    XX_MAPPING(XX_RBRC, DE_RBRC),
    XX_MAPPING(XX_BSLS, DE_BSLS),
    XX_MAPPING(XX_AT, DE_AT),
    XX_MAPPING(XX_TILD, DE_TILD),
    XX_MAPPING(XX_PIPE, DE_PIPE),
    XX_MAPPING(XX_LABK, DE_LABK),
    XX_MAPPING(XX_RABK, DE_RABK),
    XX_MAPPING(XX_CIRC, DE_CIRC),
    XX_MAPPING(XX_DEG, DE_DEG),
    XX_MAPPING(XX_RGUI, KC_RGUI),
    XX_MAPPING(XX_LGUI, KC_LGUI),
};

bool rctl_active = false;
uint16_t rctl_timer;

bool process_ctl_adia(keyrecord_t *record) {
    if (record->event.pressed) {
        rctl_timer = timer_read();
        rctl_active = true;
    } else {
        rctl_active = false;
        if (timer_elapsed(rctl_timer) < TAPPING_TERM) {
            tap_code16(DE_ADIA);
        } else {
             // Only handle key release here
            switch (detected_os) {
                case OS_MACOS:
                case OS_IOS:
                    unregister_code16(KC_RGUI);
                    break;
                default:
                    unregister_code16(KC_RCTL);
            }
        }
    }
    return false;
}

void check_ctl_adia_timer(void) {
    // Needs to be called on matrix scan
    if (rctl_active && timer_elapsed(rctl_timer) >= TAPPING_TERM) {
        // Detect a hold on the RCTL_ADIA keybinding
        rctl_active = false;
        switch (detected_os) {
            case OS_MACOS:
            case OS_IOS:
                register_code16(KC_RGUI);
                break;
            default:
                register_code16(KC_RCTL);
        }
    }
}

bool lctl_active = false;
uint16_t lctl_timer;

bool process_ctl_esc(keyrecord_t *record) {
    if (record->event.pressed) {
        lctl_timer = timer_read();
        lctl_active = true;
    } else {
        lctl_active = false;
        if (timer_elapsed(lctl_timer) < TAPPING_TERM) {
            tap_code16(KC_ESC);
        } else {
             // Only handle key release here
            switch (detected_os) {
                case OS_MACOS:
                case OS_IOS:
                    unregister_code16(KC_LGUI);
                    break;
                default:
                    unregister_code16(KC_LCTL);
            }
        }
    }
    return false;
}

void check_ctl_esc_timer(void) {
    // Needs to be called on matrix scan
    if (lctl_active && timer_elapsed(lctl_timer) >= TAPPING_TERM) {
        // Detect a hold on the RCTL_ADIA keybinding
        lctl_active = false;
        switch (detected_os) {
            case OS_MACOS:
            case OS_IOS:
                register_code16(KC_LGUI);
                break;
            default:
                register_code16(KC_LCTL);
        }
    }
}

bool process_mapped_key(uint16_t keycode, keyrecord_t  *record) {
    uint16_t translated_keycode;
    switch (detected_os) {
        case OS_MACOS:
        case OS_IOS:
            translated_keycode = XX_LOOKUP(mac_keycodes, keycode);
            break;
        default:
            translated_keycode = XX_LOOKUP(other_keycodes, keycode);
            break;
    }

    if (record->event.pressed) {
        register_code16(translated_keycode);
    } else {
        unregister_code16(translated_keycode);
    }

    return false;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (keycode >= XX_HOME && keycode <= XX_LGUI) {
        return process_mapped_key(keycode, record);
    } else if (keycode == CTL_ADIA) {
        return process_ctl_adia(record);
    } else if (keycode == CTL_ESC) {
        return process_ctl_esc(record);
    } else if (keycode == XX_OST && record->event.pressed) {
        switch (detected_os) {
            case OS_MACOS:
            case OS_IOS:
                detected_os = OS_LINUX;
                break;
            default:
                detected_os = OS_MACOS;
        }
        return false;
    }

    return true;
}

void matrix_scan_user(void) {
    check_ctl_adia_timer();
    check_ctl_esc_timer();
}

// LAYER Configuration

enum layers {
    _QWERTZ = 0,
    _SYM,
    _FUNCTION,
    _NAV,
    _ADJUST,
};

// Aliases for readability
#define QWERTZ   DF(_QWERTZ)
#define SYM      MO(_SYM)
#define NAV      MO(_NAV)
#define FKEYS    MO(_FUNCTION)
#define ADJUST   MO(_ADJUST)

#define LSFT_CAP TD(TD_LSHIFT_CAPS)
#define RSFT_CAP TD(TD_RSHIFT_CAPS)

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/*
 * Base Layer: QWERTY
 *
 * ,-------------------------------------------.      ,------.  ,------.      ,------------------------------------------------.
 * |  Esc   |   1  |   2  |   3  |   4  |   5  |      |LShift|  |RShift|      |   6  |   7  |   8  |   9  |   0  |  Bksp DEL?  |
 * |--------+------+------+------+------+------|      |------|  |------|      |------+------+------+------+------+-------------|
 * |  Tab   |   Q  |   W  |   E  |   R  |   T  |      |LCtrl |  | RCtrl|      |   Z  |   U  |   I  |   O  |   P  |  Ü          |
 * |--------+------+------+------+------+------|      |------|  |------|      |------+------+------+------+------+-------------|
 * |Ctrl/Esc|   A  |   S  |   D  |   F  |   G  |      | LAlt |  | RAlt |      |   H  |   J  |   K  |   L  |   Ö  |Ctrl/Ä       |
 * |--------+------+------+------+------+------+------+------|  |------|------+------+------+------+------+------+-------------|
 * | LShift |   Z  |   X  |   C  |   V  |   B  |  !   |CapsLk|  |F-keys|   ?  |   N  |   M  | ,  ; | . :  | - _  | RShift      |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+---------------------------'
 *                        |Adjust| LGUI | LAlt | Enter| Nav  |  | Sym  | Space| AltGr| RGUI | Menu |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 *
 * ,----------------------------.      ,------.                 ,D----------------------------.      ,------.
 * | Prev | Next | Pause | Stop |      | Mute |                 | Prev | Next | Pause | Stop |      | Mute |
 * `----------------------------'      `------'                 `----------------------------'      '------'
 */
    [_QWERTZ] = LAYOUT_myr(
      KC_ESC  , KC_1 ,  KC_2   ,  KC_3  ,   KC_4 ,   KC_5 ,         KC_LSFT,     KC_RSFT,          KC_6 ,   KC_7 ,   KC_8 ,   KC_9 ,   KC_0 , KC_BSPC,
      KC_TAB  , KC_Q ,  KC_W   ,  KC_E  ,   KC_R ,   KC_T ,         KC_LCTL,     KC_RCTL,          DE_Z ,   KC_U ,   KC_I ,   KC_O ,   KC_P , DE_UDIA,
      CTL_ESC , KC_A ,  KC_S   ,  KC_D  ,   KC_F ,   KC_G ,         KC_LALT,     KC_RALT,          KC_H ,   KC_J ,   KC_K ,   KC_L , DE_ODIA, CTL_ADIA,
      LSFT_CAP, DE_Y ,  KC_X   ,  KC_C  ,   KC_V ,   KC_B , DE_EXLM,KC_CAPS,     FKEYS  , DE_QUES, KC_N ,   KC_M ,   KC_COMM, KC_DOT,KC_SLSH, RSFT_CAP,
                                 ADJUST , XX_LGUI, KC_LALT, KC_ENT , NAV   ,     SYM    , KC_SPC , KC_RALT, XX_RGUI, KC_APP,

      KC_MPRV, KC_MNXT, KC_MPLY, KC_MSTP,    KC_MUTE,                            KC_MPRV, KC_MNXT, KC_MPLY, KC_MSTP,    KC_MUTE
    ),


/*
 * Sym Layer: Numbers and symbols
 *
 * ,-------------------------------------------.      ,------.  ,------.      ,-------------------------------------------.
 * |        |  ^   |  °   |  ~   |  ,   |  .   |      |      |  |      |      |   $  |   €  |  %   |  ´   |   `  |        |
 * |--------+------+------+------+------+------|      |------|  |------|      |------+------+------+------+------+--------|
 * |    "   |  |   |  @   |  {   |  }   |  #   |      |      |  |      |      |   +  |  1   |  2   |  3   |  /   |   *    |
 * |--------+------+------+------+------+------|      |------|  |------|      |------+------+------+------+------+--------|
 * | Ctrl/' |  \   |  !   |  (   |  )   |  :   |      |      |  |      |      |   -  |  4   |  5   |  6   |  0   |Ctrl/&  |
 * |--------+------+------+------+------+------+------+------|  |------|------+------+------+------+------+------+--------|
 * |        |  §   |  ?   |  [   |  ]   |  ;   |  <   |      |  |      |   >  |   =  |  7   |  8   |  9   |  _   |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 *
 * ,-----------------------------.      ,------.                ,---------------------------.      ,------.
 * |        |      |      |      |      |      |                |      |      |      |      |      |      |
 * `-----------------------------'      `------'                `---------------------------'      '------'
 */
    [_SYM] = LAYOUT_myr(
      _______, XX_CIRC,  XX_DEG, XX_TILD, DE_COMM,  DE_DOT,          _______, _______,          DE_DLR , DE_EURO, DE_PERC, DE_ACUT, DE_GRV , _______,
      DE_DQUO, XX_PIPE,  XX_AT , XX_LCBR, XX_RCBR, DE_HASH,          _______, _______,          DE_PLUS, KC_7   , KC_8   ,  KC_9  , DE_SLSH, DE_ASTR,
      DE_QUOT, XX_BSLS, XX_LABK, DE_LPRN, DE_RPRN, DE_COLN,          _______, _______,          DE_MINS, KC_4   , KC_5   ,  KC_6  , KC_0   , DE_AMPR,
      _______, DE_SECT, XX_RABK, XX_LBRC, XX_RBRC, DE_SCLN, _______, _______, _______, _______, DE_EQL , KC_1   , KC_2   ,  KC_3  , DE_UNDS, _______,
                                 _______, _______, _______, _______, _______, _______, _______, KC_RALT , _______, _______,

      _______, _______, _______, _______,          _______,                   _______, _______, _______, _______,          _______
    ),

/*
 * Function Layer: Function keys
 *
 * ,-------------------------------------------.      ,------.  ,------.      ,-------------------------------------------.
 * |        |      |      |      |      |      |      |      |  |      |      |      |      |      |      |      |        |
 * |--------+------+------+------+------+------|      |------|  |------|      |------+------+------+------+------+--------|
 * |        |  F9  | F10  | F11  | F12  |      |      |      |  |      |      |      |      |      |      |      |        |
 * |--------+------+------+------+------+------|      |------|  |------|      |------+------+------+------+------+--------|
 * |        |  F5  |  F6  |  F7  |  F8  |      |      |      |  |      |      |      | Shift| Ctrl |  Alt |  GUI |        |
 * |--------+------+------+------+------+------+------+------|  |------|------+------+------+------+------+------+--------|
 * |        |  F1  |  F2  |  F3  |  F4  |      |      |      |  |      |      |      |      |      |      |      |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 *
 * ,-----------------------------.      ,------.                ,---------------------------.      ,------.
 * |        |      |      |      |      |      |                |      |      |      |      |      |      |
 * `-----------------------------'      `------'                `---------------------------'      '------'
 */
    [_FUNCTION] = LAYOUT_myr(
      _______, _______, _______, _______, _______, _______,          _______, _______,          _______, _______, _______, _______, _______, _______,
      _______,  KC_F9 ,  KC_F10,  KC_F11,  KC_F12, _______,          _______, _______,          _______, _______, _______, _______, _______, _______,
      _______,  KC_F5 ,  KC_F6 ,  KC_F7 ,  KC_F8 , _______,          _______, _______,          _______, KC_RSFT, KC_RCTL, KC_LALT, KC_RGUI, _______,
      _______,  KC_F1 ,  KC_F2 ,  KC_F3 ,  KC_F4 , _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
                                 _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,

      _______, _______, _______, _______,          _______,                   _______, _______, _______, _______,          _______
    ),

/*
 * Nav Layer: Media, navigation
 *
 * ,-------------------------------------------.      ,------.  ,------.      ,-------------------------------------------.
 * |        |  F1  |  F2  |  F3  | F4   |  F5  |      |      |  |      |      | Del  | Ins  | Scrl | PrSc |      |        |
 * |--------+------+------+------+------+------|      |------|  |------|      |------+------+------+------+------+--------|
 * |        |  F6  | End  |   ↑  | Home | PgUp |      |      |  |      |      | Home | PgDn | PgUp | End  | VolUp|        |
 * |--------+------+------+------+------+------|      |------|  |------|      |------+------+------+------+------+--------|
 * |        | Del  |  ←   |   ↓  |  →   | PgDn |      |      |  |      |      |  ←   |   ↓  |   ↑  |  →   | VolDn| Insert |
 * |--------+------+------+------+------+------+------+------|  |------|------+------+------+------+------+------+--------|
 * |        | Ins  |  F7  |  F8  |  F9  |  F10 |  F11 | F12  |  |      |      | Pause|M Prev|M Play|M Next|VolMut| PrtSc  |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 *
 * ,----------------------------.      ,------.                 ,----------------------------.      ,------.
 * | Prev | Next | Pause | Stop |      | Mute |                 | Prev | Next | Pause | Stop |      | Mute |
 * `----------------------------'      `------'                 `----------------------------'      '------'
 */
    [_NAV] = LAYOUT_myr(
      _______,  KC_F1 ,  KC_F2 ,  KC_F3 ,  KC_F4 ,  KC_F5 ,          _______, _______,          KC_DEL , KC_INS , KC_SCRL, KC_PSCR, _______, _______,
      _______,  KC_F6 , XX_END ,  KC_UP , XX_HOME, KC_PGUP,          _______, _______,          XX_HOME, KC_PGDN, KC_PGUP,  XX_END, KC_VOLU, _______,
      _______, KC_DEL , KC_LEFT, KC_DOWN, KC_RGHT, KC_PGDN,          _______, _______,          KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_VOLD, _______,
      _______, KC_INS ,  KC_F7 ,  KC_F8 ,  KC_F9 , KC_F10 , KC_F11 , KC_F12 , _______, _______,KC_PAUSE, KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE, _______,
                                 _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,

      _______, _______, _______, _______,          _______,                   _______, _______, _______, _______,          _______
    ),

/*
 * Adjust Layer: Default layer settings, RGB
 *
 * ,-------------------------------------------.      ,------.  ,------.      ,-------------------------------------------.
 * |        |      |      |      |      |      |      |      |  |      |      |      |      |      |      |      |        |
 * |--------+------+------+------+------+------|      |------|  |------|      |------+------+------+------+------+--------|
 * |        |      |      |      |      |      |      |      |  |      |      |      |      |      |      |      |        |
 * |--------+------+------+------+------+------|      |------|  |------|      |------+------+------+------+------+--------|
 * |        |      |      |      | OS   |      |      |      |  |      |      | TOG  | SAI  | HUI  | VAI  | MOD  |        |
 * |--------+------+------+------+------+------+------+------|  |------|------+------+------+------+------+------+--------|
 * |        |      |      |      |      |      |      |      |  |      |      |      | SAD  | HUD  | VAD  | RMOD |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 *
 * ,-----------------------------.      ,------.                ,---------------------------.      ,------.
 * |        |      |      |      |      |      |                |      |      |      |      |      |      |
 * `-----------------------------'      `------'                `---------------------------'      '------'
 */
    [_ADJUST] = LAYOUT_myr(
      _______, _______, _______, _______, _______, _______,         _______, _______,          _______, _______, _______, _______, _______, _______,
      _______, _______, _______, _______, _______, _______,         _______, _______,          _______, _______, _______, _______, _______, _______,
      _______, _______, _______, _______, XX_OST , _______,         _______, _______,          RGB_TOG, RGB_SAI, RGB_HUI, RGB_VAI, RGB_SPI, RGB_MOD,
      _______, _______, _______, _______, _______, _______,_______, _______, _______, _______, _______, RGB_SAD, RGB_HUD, RGB_VAD, RGB_SPD, RGB_RMOD,
                                 _______, _______, _______,_______, _______, _______, _______, _______, _______, _______,

      _______, _______, _______, _______,          _______,                   _______, _______, _______, _______,          _______

    ),

// /*
//  * Layer template - LAYOUT
//  *
//  * ,-------------------------------------------.      ,------.  ,------.      ,-------------------------------------------.
//  * |        |      |      |      |      |      |      |      |  |      |      |      |      |      |      |      |        |
//  * |--------+------+------+------+------+------|      |------|  |------|      |------+------+------+------+------+--------|
//  * |        |      |      |      |      |      |      |      |  |      |      |      |      |      |      |      |        |
//  * |--------+------+------+------+------+------|      |------|  |------|      |------+------+------+------+------+--------|
//  * |        |      |      |      |      |      |      |      |  |      |      |      |      |      |      |      |        |
//  * |--------+------+------+------+------+------+------+------|  |------|------+------+------+------+------+------+--------|
//  * |        |      |      |      |      |      |      |      |  |      |      |      |      |      |      |      |        |
//  * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
//  *                        |      |      |      |      |      |  |      |      |      |      |      |
//  *                        |      |      |      |      |      |  |      |      |      |      |      |
//  *                        `----------------------------------'  `----------------------------------'
//  */
//     [_LAYERINDEX] = LAYOUT(
//       _______, _______, _______, _______, _______, _______,          _______, _______,          _______, _______, _______, _______, _______, _______,
//       _______, _______, _______, _______, _______, _______,          _______, _______,          _______, _______, _______, _______, _______, _______,
//       _______, _______, _______, _______, _______, _______,          _______, _______,          _______, _______, _______, _______, _______, _______,
//       _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
//                                  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
//     ),

// /*
//  * Layer template - LAYOUT_myr
//  *
//  * ,-------------------------------------------.      ,------.  ,------.      ,-------------------------------------------.
//  * |        |      |      |      |      |      |      |      |  |      |      |      |      |      |      |      |        |
//  * |--------+------+------+------+------+------|      |------|  |------|      |------+------+------+------+------+--------|
//  * |        |      |      |      |      |      |      |      |  |      |      |      |      |      |      |      |        |
//  * |--------+------+------+------+------+------|      |------|  |------|      |------+------+------+------+------+--------|
//  * |        |      |      |      |      |      |      |      |  |      |      |      |      |      |      |      |        |
//  * |--------+------+------+------+------+------+------+------|  |------|------+------+------+------+------+------+--------|
//  * |        |      |      |      |      |      |      |      |  |      |      |      |      |      |      |      |        |
//  * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
//  *                        |      |      |      |      |      |  |      |      |      |      |      |
//  *                        |      |      |      |      |      |  |      |      |      |      |      |
//  *                        `----------------------------------'  `----------------------------------'
//  *
//  * ,-----------------------------.      ,------.                ,---------------------------.      ,------.
//  * |        |      |      |      |      |      |                |      |      |      |      |      |      |
//  * `-----------------------------'      `------'                `---------------------------'      '------'
//  */
//     [_LAYERINDEX] = LAYOUT_myr(
//       _______, _______, _______, _______, _______, _______,          _______, _______,          _______, _______, _______, _______, _______, _______,
//       _______, _______, _______, _______, _______, _______,          _______, _______,          _______, _______, _______, _______, _______, _______,
//       _______, _______, _______, _______, _______, _______,          _______, _______,          _______, _______, _______, _______, _______, _______,
//       _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
//                                  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
//
//       _______, _______, _______, _______,          _______,                   _______, _______, _______, _______,          _______
//     ),
};



/* The default OLED and rotary encoder code can be found at the bottom of qmk_firmware/keyboards/splitkb/elora/rev1/rev1.c
 * These default settings can be overriden by your own settings in your keymap.c
 * DO NOT edit the rev1.c file; instead override the weakly defined default functions by your own.
 */

// clang-format off
static const char PROGMEM linux_logo[][3] = {
    {0x9C,0x9D, 0x00},
    {0xBC,0xBD, 0x00},
};

static const char PROGMEM apple_logo[][3] = {
    {0x98,0x99,0x00},
    {0xB8,0xB9,0x00},
};

static const char PROGMEM windows_logo[][3] = {
    {0x9A,0x9B,0x00},
    {0xBA,0xBB,0x00},
};

static const char PROGMEM layer_glyph[][3] = {
    {0x96,0x97,0x00},
    {0xB6,0xB7,0x00},
};

static const char PROGMEM scrl_glyph[][3] = {
    {0x8E,0x8F,0x00},
    {0xAE,0xAF,0x00},
};

static const char PROGMEM num_glyph[][3] = {
    {0x90,0x91,0x00},
    {0xB0,0xB1,0x00},
};

static const char PROGMEM caps_glyph[][3] = {
    {0x8C,0x8D,0x00},
    {0xAC,0xAD,0x00},
};

static const char PROGMEM gui_glyph[][3] = {
    {0x8A,0x8B,0x00},
    {0xAA,0xAB,0x00},
};

static const char PROGMEM ctrl_glyph[][3] = {
    {0x84,0x85,0x00},
    {0xA4,0xA5,0x00},
};

static const char PROGMEM alt_glyph[][3] = {
    {0x86,0x87,0x00},
    {0xA6,0xA7,0x00},
};

static const char PROGMEM sft_glyph[][3] = {
    {0x88,0x89,0x00},
    {0xA8,0xA9,0x00},
};

static const char PROGMEM qmk_logo[][5] = {
    {0x80,0x81,0x82,0x83,0x00},
    {0xA0,0xA1,0xA2,0xA3,0x00},
    {0xC0,0xC1,0xc2,0xC3,0x00},
};
// clang-format on

static const char PROGMEM title[] = PSTR("Elora rev1 \nPrakti's\nLayout\n\n");

#ifdef OLED_ENABLE

void modkey_symbol(uint8_t mod, uint8_t key, const char glyph[][3], uint8_t x, uint8_t y) {
    if (mod & MOD_BIT(key)) {
        oled_set_cursor(x, y);
        oled_write_P(glyph[0], false);
        oled_set_cursor(x, y+1);
        oled_write_P(glyph[1], false);
    } else {
        oled_set_cursor(x, y);
        oled_write_P(PSTR("  "), false);
        oled_set_cursor(x, y+1);
        oled_write_P(PSTR("  "), false);
    }
}

void lock_key_symbol(bool led_on, const char glyph[][3], uint8_t x, uint8_t y) {
    if (led_on) {
        oled_set_cursor(x, y);
        oled_write_P(glyph[0], false);
        oled_set_cursor(x, y+1);
        oled_write_P(glyph[1], false);
    } else {
        oled_set_cursor(x, y);
        oled_write_P(PSTR("  "), false);
        oled_set_cursor(x, y+1);
        oled_write_P(PSTR("  "), false);
    }
}

bool oled_task_user(void) {
    oled_write_P(title, false);

    uint8_t mods = get_mods();

    if (is_keyboard_left()) {
        modkey_symbol(mods, KC_LGUI, gui_glyph, 1, 5);
        modkey_symbol(mods, KC_LCTL, ctrl_glyph, 3, 5);
        modkey_symbol(mods, KC_LALT, alt_glyph, 5, 5);
        modkey_symbol(mods, KC_LSFT, sft_glyph, 7, 5);
    } else {
        modkey_symbol(mods, KC_RGUI, gui_glyph, 1, 5);
        modkey_symbol(mods, KC_RCTL, ctrl_glyph, 3, 5);
        modkey_symbol(mods, KC_RALT, alt_glyph, 5, 5);
        modkey_symbol(mods, KC_RSFT, sft_glyph, 7, 5);
    }

    // Host Keyboard LED Status
    led_t led_state = host_keyboard_led_state();

    lock_key_symbol(led_state.num_lock, num_glyph, 1, 7);
    lock_key_symbol(led_state.caps_lock, caps_glyph, 3, 7);
    lock_key_symbol(led_state.scroll_lock, scrl_glyph, 5, 7);

    // Overlay Layer
    oled_set_cursor(0,10);
    oled_write_P(layer_glyph[0], false); oled_advance_page(true);
    switch (get_highest_layer(layer_state)) {
        case _QWERTZ:
            oled_write_P(layer_glyph[1], false);
            oled_write_P(PSTR(" BASE\n"), false);
            break;
        case _SYM:
            oled_write_P(layer_glyph[1], false);
            oled_write_P(PSTR(" SYMBOL\n"), false);
            break;
        case _FUNCTION:
            oled_write_P(layer_glyph[1], false);
            oled_write_P(PSTR(" FUNC\n"), false);
            break;
        case _NAV:
            oled_write_P(layer_glyph[1], false);
            oled_write_P(PSTR(" NAVI\n"), false);
            break;
        case _ADJUST:
            oled_write_P(layer_glyph[1], false);
            oled_write_P(PSTR(" CONF\n"), false);
            break;
        default:
            oled_write_P(layer_glyph[1], false);
            oled_write_ln_P(PSTR("???"), false);
    }

    if (is_keyboard_master()) {
        // QMK Logo
        oled_set_cursor(0, oled_max_lines()-3);
        oled_write_P(qmk_logo[0], false); oled_advance_page(false);
        oled_write_P(qmk_logo[1], false); oled_advance_page(false);
        oled_write_P(qmk_logo[2], false);

        switch (detected_os) {
            case OS_UNSURE:
                oled_set_cursor(oled_max_chars()-4, oled_max_lines()-2);
                oled_write_P(PSTR("??"), false);
                oled_set_cursor(oled_max_chars()-4, oled_max_lines()-1);
                oled_write_P(PSTR("??"), false);
                break;
            case OS_LINUX:
                oled_set_cursor(oled_max_chars()-4, oled_max_lines()-2);
                oled_write_P(linux_logo[0], false);
                oled_set_cursor(oled_max_chars()-4, oled_max_lines()-1);
                oled_write_P(linux_logo[1], false);
                break;
            case OS_WINDOWS:
                oled_set_cursor(oled_max_chars()-4, oled_max_lines()-2);
                oled_write_P(windows_logo[0], false); oled_advance_page(true);
                oled_set_cursor(oled_max_chars()-4, oled_max_lines()-1);
                oled_write_P(windows_logo[1], false); oled_advance_page(true);
                break;
            case OS_MACOS:
            case OS_IOS:
                oled_set_cursor(oled_max_chars()-4, oled_max_lines()-2);
                oled_write_P(apple_logo[0], false); oled_advance_page(true);
                oled_set_cursor(oled_max_chars()-4, oled_max_lines()-1);
                oled_write_P(apple_logo[1], false); oled_advance_page(true);
                break;
        }
    } else {
        static const char PROGMEM sleepy_cat[] = {
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,128,128, 64, 64, 64, 64, 64, 64,
            64, 64, 64, 64, 64, 64, 64,128,128,128,  0,  0,  0,  0,  0,  0,224,
            24, 68,136,144, 32, 64,192, 64,192, 64, 64,128,128, 64, 32,160,
            16,240,  0,  0,  0,  0,  0,  0,  0,  0, 0,  0,  0,  0,192, 32, 16,
            8,  4,  2, 66,129,  1,  0,  0,  0, 16, 34,  4,  4, 32,192,  6,
            24,224,  0,  6,216,  0,224, 24,  1,  1,  1,  1,241, 14,  1,  0,  0,
            0, 32, 32, 32,  3,128,  7,  0, 71, 64, 64,  0,  1,240, 14,  1,  0,
            0,  0,  0,  0,  0,  0,  0, 32, 32,248,  7,  0,  0,  0,  0,224, 32,
            32, 32, 35, 40, 40, 40, 40, 40, 41, 41, 40, 40, 40, 40, 50, 32, 32,
            32, 32, 32, 32, 32, 33, 34, 44, 32, 33, 34, 34, 34, 36, 36, 36, 40,
            41, 41, 41, 40, 40, 40, 36, 42, 42, 41, 42, 50, 42, 42, 42, 42, 42,
            44, 32, 32,
        };
        oled_set_cursor(0, 12);
        oled_write_raw_P(sleepy_cat, sizeof(sleepy_cat));
    }

    return false;
}
#endif

/* DELETE THIS LINE TO UNCOMMENT (1/2)
#ifdef ENCODER_ENABLE
bool encoder_update_user(uint8_t index, bool clockwise) {
  // Your code goes here
} #endif
DELETE THIS LINE TO UNCOMMENT (2/2) */
