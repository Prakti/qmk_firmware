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
// Aliases for Mac layers
#define MCQWER   DF(_MAC_QWERTZ)
#define MCSYM    MO(_MAC_SYM)
#define MCNAV    MO(_MAC_NAV)
#define MCFKEY   MO(_MAC_FUNCTION)
#define MCADJU   MO(_MAC_ADJUST)

#define CTL_ESC  MT(MOD_LCTL, KC_ESC)
#define CTL_ADIA MT(MOD_RCTL, DE_ADIA)
#define RALT_0   MT(MOD_RALT, KC_0)
#define S_SZ     TD(TD_S_SZ)
#define LSFT_CAP TD(TD_LSHIFT_CAPS)
#define RSFT_CAP TD(TD_RSHIFT_CAPS)


// OS Detection on USB Init
os_variant_t detected_os = OS_UNSURE;

bool process_detected_host_os_user(os_variant_t os_variant) {
    // locally store detected OS
    detected_os = os_variant;
    return true;
}


// TAP DANCE Configuration

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case DE_S: // make our ß tap dance more efficient
            return 160;
        default:
            return TAPPING_TERM;
    }
}

enum tap_dance_opts {
    TD_S_SZ,
    TD_LSHIFT_CAPS,
    TD_RSHIFT_CAPS,
};

void dance_sz(tap_dance_state_t *state, void *user_data) {
    if (state->count >= 3) {
        tap_code16(DE_SS);
        reset_tap_dance(state);
    } else if (state->count >=2) {
        tap_code16(DE_S);
        tap_code16(DE_S);
        reset_tap_dance(state);
    } else {
        tap_code16(DE_S);
        reset_tap_dance(state);
    }
}

tap_dance_action_t tap_dance_actions[] = {
    // Tap once or twice for 's' and thrice for 'ß'
    [TD_S_SZ] = ACTION_TAP_DANCE_FN(dance_sz),
    [TD_LSHIFT_CAPS] = ACTION_TAP_DANCE_DOUBLE(KC_LSFT, KC_CAPS),
    [TD_RSHIFT_CAPS] = ACTION_TAP_DANCE_DOUBLE(KC_RSFT, KC_CAPS),
};

// KEY OVERRIDES

const key_override_t delete_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_BSPC, KC_DEL);

// This globally defines all key overrides to be used
const key_override_t **key_overrides = (const key_override_t *[]){
	&delete_key_override,
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
    XX_MAPPING(XX_DEG, DE_RABK)
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
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (keycode >= XX_HOME && keycode <= XX_DEG) {
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


// Keymap Layer definition


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
      CTL_ESC , KC_A ,  S_SZ   ,  KC_D  ,   KC_F ,   KC_G ,         KC_LALT,     KC_RALT,          KC_H ,   KC_J ,   KC_K ,   KC_L , DE_ODIA, CTL_ADIA,
      LSFT_CAP, DE_Y ,  KC_X   ,  KC_C  ,   KC_V ,   KC_B , DE_EXLM,KC_CAPS,     FKEYS  , DE_QUES, KC_N ,   KC_M ,   KC_COMM, KC_DOT,KC_SLSH, RSFT_CAP,
                                 ADJUST , KC_LGUI, KC_LALT, KC_ENT , NAV   ,     SYM    , KC_SPC , KC_RALT, KC_RGUI, KC_APP,

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
                                 _______, _______, _______, _______, _______, _______, _______, RALT_0 , _______, _______,

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
 * |        |      |      |      |      |      |      |      |  |      |      |      |      |      |      |      |        |
 * |--------+------+------+------+------+------|      |------|  |------|      |------+------+------+------+------+--------|
 * |        |      |      |      |      |      |      |      |  |      |      | PgUp | Home |   ↑  | End  | VolUp| Delete |
 * |--------+------+------+------+------+------|      |------|  |------|      |------+------+------+------+------+--------|
 * |        |  GUI |  Alt | Ctrl | Shift|      |      |      |  |      |      | PgDn |  ←   |   ↓  |   →  | VolDn| Insert |
 * |--------+------+------+------+------+------+------+------|  |------|------+------+------+------+------+------+--------|
 * |        |      |      |      |      |      |      |ScLck |  |      |      | Pause|M Prev|M Play|M Next|VolMut| PrtSc  |
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
      _______, _______, _______, _______, _______, _______,          _______, _______,          KC_DEL , KC_INS , KC_SCRL, KC_PSCR, _______, _______,
      _______, _______, _______, _______, _______, _______,          _______, _______,          XX_HOME, KC_PGDN, KC_PGUP,  XX_END, KC_VOLU, _______,
      _______, _______, _______, _______, _______, _______,          _______, _______,          KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_VOLD, _______,
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,KC_PAUSE, KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE, _______,
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


#ifdef OLED_ENABLE
bool oled_task_user(void) {
    oled_write_P(PSTR("Elora rev1 \n Prakti's Layout\n\n"), false);

    switch (detected_os) {
        case OS_UNSURE:
            oled_write_P(PSTR("Unsure\n"), false);
            break;
        case OS_LINUX:
            oled_write_P(PSTR("Linux\n"), false);
            break;
        case OS_WINDOWS:
            oled_write_P(PSTR("Windows\n"), false);
            break;
        case OS_MACOS:
            oled_write_P(PSTR("MacOS\n"), false);
            break;
        case OS_IOS:
            oled_write_P(PSTR("IOS\n"), false);
            break;
    }

    // Overlay Layer
    switch (get_highest_layer(layer_state)) {
        case _QWERTZ:
            oled_write_P(PSTR("BASE\n"), false);
            break;
        case _SYM:
            oled_write_P(PSTR("SYMBOLS\n"), false);
            break;
        case _FUNCTION:
            oled_write_P(PSTR("FUNCTIONS\n"), false);
            break;
        case _NAV:
            oled_write_P(PSTR("NAVIGATION\n"), false);
            break;
        case _ADJUST:
            oled_write_P(PSTR("SETTINGS\n"), false);
            break;
        default:
            // Or use the write_ln shortcut over adding '\n' to the end of your string
            oled_write_ln_P(PSTR("Layer?"), false);
    }

    // Host Keyboard LED Status
    led_t led_state = host_keyboard_led_state();
    oled_write_P(led_state.num_lock ? PSTR("NUM ") : PSTR("    "), false);
    oled_write_P(led_state.caps_lock ? PSTR("CAP ") : PSTR("    "), false);
    oled_write_P(led_state.scroll_lock ? PSTR("SCR ") : PSTR("    "), false);

    // QMK Logo
    // clang-format off
    static const char PROGMEM qmk_logo[] = {
        0x81,0x82,0x83,0x84,0x0a,
        0xa1,0xa2,0xa3,0xa4,0x85,0x86,0x87,0x88,0x89,0x0a,
        0xc1,0xc2,0xc3,0xc4,0xa5,0xa6,0xa7,0xa8,0xa9,0x0a,
        0x8a,0x8b,0x8c,0x8d,0xc5,0xc6,0xc7,0xc8,0xc9,0x0a,
        0xaa,0xab,0xac,0xad,0xae,0xaf,0xb0,0xb1,0xb2,0xb3,0x00
    };
    // clang-format on
    oled_set_cursor(0, oled_max_lines()-5);
    oled_write_P(qmk_logo, false);

    return false;
}
#endif

/* DELETE THIS LINE TO UNCOMMENT (1/2)
#ifdef ENCODER_ENABLE
bool encoder_update_user(uint8_t index, bool clockwise) {
  // Your code goes here
}
#endif
DELETE THIS LINE TO UNCOMMENT (2/2) */
