#include QMK_KEYBOARD_H
#include "split_util.h"

#define _BASE 0
#define _NUMS 1
#define _SYMS 2
#define _MOVE 3

#define SS_HYPER(KC) SS_LCMD(SS_LCTL(SS_LOPT(SS_LSFT(KC))))

#define L1_THUMB LT(_NUMS, KC_SPC)
#define R1_THUMB LT(_MOVE, KC_BSPC)
#define R2_THUMB LT(_SYMS, KC_ENT)

#define LHS_WORD LOPT(KC_LEFT)
#define RHS_WORD ROPT(KC_RIGHT)

enum custom_keycodes {
    _____ = SAFE_RANGE,
	SNIPT,  // MacOS screenshot to clipboard snipping tool + Arc browser UI component snipping tool
	NAV1,	// Arc browser fast pinned tab navigation + MacOS fast application navigation...
	NAV2,
	NAV3,
	NAV4,
	NAV5,
	NAV6,
	AMSW,   // Amethyst switch layouts + switch main window 
	AMIN, 	// Amethyst increase main window's horizontal width 
	AMDE,   // Amethyst decrease main window's horizontal width
};

// Comboes 
const uint16_t PROGMEM alfred_search_combo[] = {KC_SPC, KC_BSPC, COMBO_END};
const uint16_t PROGMEM wooshy_search_combo[] = {KC_B, KC_N, COMBO_END};
combo_t key_combos[] = {
    COMBO(alfred_search_combo, LCMD(KC_SPC)), // TODO: NEEDS FIXING
    COMBO(wooshy_search_combo, HYPR(KC_W)),
};

// Key Overrides AKA mod-morphs: 
const key_override_t bspace_override = ko_make_basic(MOD_MASK_SHIFT, KC_BSPC, LOPT(KC_BSPC));
const key_override_t delete_override = ko_make_basic(MOD_MASK_SHIFT, KC_DEL, LOPT(KC_DEL));
const key_override_t grvesc_override = ko_make_basic(MOD_MASK_SHIFT, KC_ESC, KC_GRV);
const key_override_t **key_overrides = (const key_override_t *[]){
    &bspace_override,
	&delete_override,
	&grvesc_override,
    NULL
};

// Macros
void process_nav_key(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        if (get_mods() & MOD_MASK_SHIFT) {
			tap_code16(C(A(G(keycode))));
        } else {
			tap_code16(G(keycode));
        }
    }
}

uint8_t mod_state;
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	mod_state = get_mods();
	static bool v_tapped = false;

    switch (keycode) {
		case NAV1:  
            process_nav_key(KC_1, record);
            break;
        case NAV2:
            process_nav_key(KC_2, record);
            break;
        case NAV3:
            process_nav_key(KC_3, record);
            break;
        case NAV4:
            process_nav_key(KC_4, record);
            break;
        case NAV5:
            process_nav_key(KC_5, record);
            break;
        case NAV6:
            process_nav_key(KC_6, record);
            break;
		case SNIPT: 
			if (record->event.pressed) {
				if (mod_state & MOD_MASK_SHIFT) {
					tap_code16(S(G(KC_2)));
				} else {
					tap_code16(C(S(G(KC_4))));
				}
			} 
			break;
		case AMSW:
			if (record->event.pressed) {
				if (mod_state & MOD_MASK_SHIFT) {
					tap_code(C(A(KC_COMMA)));
				} else {
					tap_code(C(A(KC_ENT)));
				}
			}
			break;
		case AMIN: 
			if (record->event.pressed) {
				tap_code(C(A(KC_RIGHT)));
			}
			break;
		case AMDE:
			if (record->event.pressed) {
				tap_code(C(A(KC_LEFT)));
			}
			break;
		case KC_V:
			if (record->event.pressed) {
				static uint16_t tap_timer = 0;
				if (mod_state & MOD_MASK_GUI) {
					if (v_tapped && !timer_expired(record->event.time, tap_timer)) {
						tap_code16(HYPR(KC_V));
					}
					v_tapped = true;
					tap_timer = record->event.time + TAPPING_TERM;
				} else {
					tap_code(KC_V);
					v_tapped = false; 
				}
			}
			break;
		default:
			v_tapped = false; 
	}
    return true;
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[_BASE] = LAYOUT_split_3x6_3(                                                                                                                       \
  	/* ╭━━━━━━━━━┳━━━━━━━━━┳━━━━━━━━━┳━━━━━━━━━┳━━━━━━━━━┳━━━━━━━━━╮                      ╭━━━━━━━━━┳━━━━━━━━━┳━━━━━━━━━┳━━━━━━━━━┳━━━━━━━━━┳━━━━━━━━━╮
  	*/     KC_ESC,     KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,                            KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,   KC_DEL, \
  	/*  ━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━┫                      ┣━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━┫
  	*/     KC_TAB,     KC_A,     KC_S,     KC_D,     KC_F,     KC_G,                            KC_H,     KC_J,     KC_K,     KC_L,  KC_SCLN, KC_QUOTE, \
  	/*  ━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━┫                      ┣━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━┫
  	*/    KC_LSFT,     KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,                            KC_N,     KC_M, KC_COMMA,   KC_DOT, KC_SLASH,  KC_HYPR, \
  	/* ╰━━━━━━━━━┻━━━━━━━━━┻━━━━━━━━━┻━━━━━━━━━┻━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╮  ╭━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━┻━━━━━━━━━┻━━━━━━━━━┻━━━━━━━━━┻━━━━━━━━━╯
  	*/                                            KC_LOPT,  KC_LCMD, L1_THUMB,    R1_THUMB, R2_THUMB,  KC_RCTL									   	    \
  	//      								   ╰─━━━━━━━━┻━━━━━━━━━┻━━━━━━━━━╯  ╰━━━━━━━━━┻━━━━━━━━━┻━━━━━━━━━╯
 	),
	[_NUMS] = LAYOUT_split_3x6_3(                                                                                                                       \
  	/* ╭━━━━━━━━━┳━━━━━━━━━┳━━━━━━━━━┳━━━━━━━━━┳━━━━━━━━━┳━━━━━━━━━╮                      ╭━━━━━━━━━┳━━━━━━━━━┳━━━━━━━━━┳━━━━━━━━━┳━━━━━━━━━┳━━━━━━━━━╮
  	*/   KC_GRAVE,     KC_1,     KC_2,     KC_3,     KC_4,     KC_5,                            KC_6,     KC_7,     KC_8,     KC_9,     KC_0,   KC_DEL, \
  	/*  ━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━┫                      ┣━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━┫
  	*/    _______,    SNIPT,     AMIN,     NAV1,     NAV2,     NAV3,                         XXXXXXX,  XXXXXXX,  XXXXXXX,  KC_BSLS,  KC_MINS,   KC_EQL, \
  	/*  ━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━┫                      ┣━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━┫
  	*/    _______,     AMSW,     AMDE,     NAV4,     NAV5,     NAV6,                         XXXXXXX,  XXXXXXX, KC_COMMA,   KC_DOT, KC_SLASH,  _______, \
  	/* ╰━━━━━━━━━┻━━━━━━━━━┻━━━━━━━━━┻━━━━━━━━━┻━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╮  ╭━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━┻━━━━━━━━━┻━━━━━━━━━┻━━━━━━━━━┻━━━━━━━━━╯
  	*/                                            _______,  _______,  _______,     _______,  _______,  _______									   	    \
  	//      								   ╰─━━━━━━━━┻━━━━━━━━━┻━━━━━━━━━╯  ╰━━━━━━━━━┻━━━━━━━━━┻━━━━━━━━━╯
 	),
	[_SYMS] = LAYOUT_split_3x6_3(                                                                                                                       \
  	/* ╭━━━━━━━━━┳━━━━━━━━━┳━━━━━━━━━┳━━━━━━━━━┳━━━━━━━━━┳━━━━━━━━━╮                      ╭━━━━━━━━━┳━━━━━━━━━┳━━━━━━━━━┳━━━━━━━━━┳━━━━━━━━━┳━━━━━━━━━╮
  	*/    KC_TILD,  KC_EXLM,    KC_AT,  KC_HASH,   KC_DLR,  KC_PERC,                         KC_CIRC,  KC_AMPR,  KC_ASTR,  KC_LPRN,  KC_RPRN,   KC_DEL, \
  	/*  ━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━┫                      ┣━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━┫
  	*/    _______,  XXXXXXX,  XXXXXXX,  KC_LCBR,  KC_LBRC,  KC_LPRN,                         KC_RPRN,  KC_RBRC,  KC_RCBR,  KC_PIPE,  KC_UNDS,  KC_PLUS, \
  	/*  ━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━┫                      ┣━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━┫
  	*/    _______,  QK_MAKE,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,                         XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  _______, \
  	/* ╰━━━━━━━━━┻━━━━━━━━━┻━━━━━━━━━┻━━━━━━━━━┻━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╮  ╭━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━┻━━━━━━━━━┻━━━━━━━━━┻━━━━━━━━━┻━━━━━━━━━╯
  	*/                                            _______,  _______,  _______,     _______,  _______,  _______									   	    \
  	//      								   ╰─━━━━━━━━┻━━━━━━━━━┻━━━━━━━━━╯  ╰━━━━━━━━━┻━━━━━━━━━┻━━━━━━━━━╯
 	),
	[_MOVE] = LAYOUT_split_3x6_3(                                                                                                                       \
  	/* ╭━━━━━━━━━┳━━━━━━━━━┳━━━━━━━━━┳━━━━━━━━━┳━━━━━━━━━┳━━━━━━━━━╮                      ╭━━━━━━━━━┳━━━━━━━━━┳━━━━━━━━━┳━━━━━━━━━┳━━━━━━━━━┳━━━━━━━━━╮
  	*/    _______,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,                         XXXXXXX, LHS_WORD,    KC_UP, RHS_WORD,  XXXXXXX,   KC_DEL, \
  	/*  ━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━┫                      ┣━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━┫
  	*/    _______,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,                         XXXXXXX,  KC_LEFT,  KC_DOWN, KC_RIGHT,  XXXXXXX,  _______, \
  	/*  ━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━┫                      ┣━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━┫
  	*/    _______,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,                         XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  _______, \
  	/* ╰━━━━━━━━━┻━━━━━━━━━┻━━━━━━━━━┻━━━━━━━━━┻━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╮  ╭━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━┻━━━━━━━━━┻━━━━━━━━━┻━━━━━━━━━┻━━━━━━━━━╯
  	*/                                            _______,  _______,  _______,     _______,  _______,  _______									   	    \
  	//      								   ╰─━━━━━━━━┻━━━━━━━━━┻━━━━━━━━━╯  ╰━━━━━━━━━┻━━━━━━━━━┻━━━━━━━━━╯
 	),
};


