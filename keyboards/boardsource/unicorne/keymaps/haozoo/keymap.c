#include QMK_KEYBOARD_H
#include "split_util.h"

#define _BASE 0
#define _NUMS 1
#define _SYMS 2
#define _MOVE 3
#define _ALFR 4

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
	NAVU, 	// Amethyst increase main window's horizontal width + go to above tab
	NAVD,   // Amethyst decrease main window's horizontal width + go to below tab
	TD_V,   // Alfred paste tap dance
	JIGG,   // Jiggles the mouse 
	ACV1,   // Alfred CMD + V navigation
	ACV2,
	ACV3,
	ACV4,
	ACV5,
	ACV6,
	ACV7,
	ACV8,
	ACV9,
};

// Comboes 
const uint16_t PROGMEM alfred_search_combo[] = {L1_THUMB, R1_THUMB, COMBO_END};
const uint16_t PROGMEM wooshy_search_combo[] = {KC_B, KC_N, COMBO_END};
combo_t key_combos[] = {
    COMBO(alfred_search_combo, LCMD(KC_SPC)), // TODO: NEEDS FIXING
    COMBO(wooshy_search_combo, HYPR(KC_W)),
};

// Key Overrides AKA mod-morphs: 
const key_override_t bspace_override = ko_make_basic(MOD_MASK_SHIFT, R1_THUMB, LOPT(KC_BSPC));
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
	static deferred_token token = INVALID_DEFERRED_TOKEN; 
	static bool v_tapped = false;

    switch (keycode) 
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
					del_mods(MOD_MASK_SHIFT);
					tap_code16(C(A(KC_COMMA)));
					set_mods(mod_state);
				} else {
					tap_code16(C(A(KC_ENT)));
				}
			}
			break;
		case NAVU: 
			if (record->event.pressed) {
				if (mod_state & MOD_MASK_SHIFT) {
					del_mods(MOD_MASK_SHIFT);
					tap_code16(C(A(KC_RIGHT)));
					set_mods(mod_state);
				} else {
					tap_code16(A(G(KC_UP)));
				}
			}
			break;
		case NAVD:
			if (record->event.pressed) {
				if (mod_state & MOD_MASK_SHIFT) {
					del_mods(MOD_MASK_SHIFT);
					tap_code16(C(A(KC_LEFT)));
					set_mods(mod_state);
				} else {
					tap_code16(A(G(KC_DOWN)));
				}
			}
			break;
		case TD_V:
			if (record->event.pressed) {
				static uint16_t tap_timer = 0;
				if (mod_state & MOD_MASK_GUI) {
					if (v_tapped && !timer_expired(record->event.time, tap_timer)) {
						tap_code16(HYPR(KC_V));
						if (token) {
							cancel_deferred_exec(token);
							token = INVALID_DEFERRED_TOKEN; 
						}
						set_oneshot_layer(_ALFR, ONESHOT_START); 						
						v_tapped = false;
					} else {
						uint32_t paste_callback(uint32_t trigger_time, void *cb_arg) {
							tap_code16(G(KC_V));
							return 0;
						}
						token = defer_exec(TAPPING_TERM, paste_callback, NULL);
						v_tapped = true;
					}
					tap_timer = record->event.time + TAPPING_TERM;
				} else {
					tap_code(KC_V);
					v_tapped = false; 
				}
			}
			break;
		case JIGG:
			if (record->event.pressed) {
				static deferred_token token = INVALID_DEFERRED_TOKEN;
				static report_mouse_t report = {0};
				if (token) {
					cancel_deferred_exec(token);
					token = INVALID_DEFERRED_TOKEN;
					report = (report_mouse_t){};  
					host_mouse_send(&report);
				} else if (keycode == JIGG) {
					uint32_t jiggler_callback(uint32_t trigger_time, void* cb_arg) {
						static const int8_t deltas[32] = {
							0, -1, -2, -2, -3, -3, -4, -4, -4, -4, -3, -3, -2, -2, -1, 0,
							0, 1, 2, 2, 3, 3, 4, 4, 4, 4, 3, 3, 2, 2, 1, 0};
						static uint8_t phase = 0;
						report.x = deltas[phase];
						report.y = deltas[(phase + 8) & 31];
						phase = (phase + 1) & 31;
						host_mouse_send(&report);
						return 16;  
					}
				token = defer_exec(1, jiggler_callback, NULL); 
				}
			}
			break;
		case ACV1:
        case ACV2:
        case ACV3:
        case ACV4:
        case ACV5:
        case ACV6:
        case ACV7:
        case ACV8:
        case ACV9:
            if (record->event.pressed) {
                tap_code16(G(keycode - ACV1 + KC_1));
            } else {
                clear_oneshot_layer_state(ONESHOT_PRESSED);
            }
            break;
        case KC_ESC:
            if (IS_LAYER_ON(_ALFR)) {
                clear_oneshot_layer_state(ONESHOT_PRESSED); 
            }
            break;
		default:
			v_tapped = false; 

	if (IS_LAYER_ON(_ALFR) && !record->event.pressed && keycode != TD_V) {
		clear_oneshot_layer_state(ONESHOT_PRESSED); 
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
  	*/    KC_LSFT,     KC_Z,     KC_X,     KC_C,     TD_V,     KC_B,                            KC_N,     KC_M, KC_COMMA,   KC_DOT, KC_SLASH,  KC_HYPR, \
  	/* ╰━━━━━━━━━┻━━━━━━━━━┻━━━━━━━━━┻━━━━━━━━━┻━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╮  ╭━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━┻━━━━━━━━━┻━━━━━━━━━┻━━━━━━━━━┻━━━━━━━━━╯
  	*/                                            KC_LOPT,  KC_LCMD, L1_THUMB,    R1_THUMB, R2_THUMB,  KC_RCTL									   	    \
  	//      								   ╰─━━━━━━━━┻━━━━━━━━━┻━━━━━━━━━╯  ╰━━━━━━━━━┻━━━━━━━━━┻━━━━━━━━━╯
 	),
	[_NUMS] = LAYOUT_split_3x6_3(                                                                                                                       \
  	/* ╭━━━━━━━━━┳━━━━━━━━━┳━━━━━━━━━┳━━━━━━━━━┳━━━━━━━━━┳━━━━━━━━━╮                      ╭━━━━━━━━━┳━━━━━━━━━┳━━━━━━━━━┳━━━━━━━━━┳━━━━━━━━━┳━━━━━━━━━╮
  	*/   KC_GRAVE,     KC_1,     KC_2,     KC_3,     KC_4,     KC_5,                            KC_6,     KC_7,     KC_8,     KC_9,     KC_0,   KC_DEL, \
  	/*  ━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━┫                      ┣━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━┫
  	*/    _______,    SNIPT,     NAVU,     NAV1,     NAV2,     NAV3,                         XXXXXXX,  XXXXXXX,  XXXXXXX,  KC_BSLS,  KC_MINS,   KC_EQL, \
  	/*  ━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━┫                      ┣━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━┫
  	*/    _______,     AMSW,     NAVD,     NAV4,     NAV5,     NAV6,                         XXXXXXX,  XXXXXXX, KC_COMMA,   KC_DOT, KC_SLASH,  _______, \
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
  	*/    _______,     JIGG,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,                         XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  _______, \
  	/* ╰━━━━━━━━━┻━━━━━━━━━┻━━━━━━━━━┻━━━━━━━━━┻━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╮  ╭━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━┻━━━━━━━━━┻━━━━━━━━━┻━━━━━━━━━┻━━━━━━━━━╯
  	*/                                            _______,  _______,  _______,     _______,  _______,  _______									   	    \
  	//      								   ╰─━━━━━━━━┻━━━━━━━━━┻━━━━━━━━━╯  ╰━━━━━━━━━┻━━━━━━━━━┻━━━━━━━━━╯
 	),
	[_ALFR] = LAYOUT_split_3x6_3(                                                                                                                       \
  	/* ╭━━━━━━━━━┳━━━━━━━━━┳━━━━━━━━━┳━━━━━━━━━┳━━━━━━━━━┳━━━━━━━━━╮                      ╭━━━━━━━━━┳━━━━━━━━━┳━━━━━━━━━┳━━━━━━━━━┳━━━━━━━━━┳━━━━━━━━━╮
  	*/    _______,  G(KC_1),  G(KC_2),  G(KC_3),  G(KC_4),  G(KC_5),                         G(KC_6),  G(KC_7),  G(KC_8),  G(KC_9),  XXXXXXX,  _______, \
  	/*  ━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━┫                      ┣━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━┫
  	*/    _______,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,                         XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  _______, \
  	/*  ━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━┫                      ┣━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━┫
  	*/    _______,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,                         XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  _______, \
  	/* ╰━━━━━━━━━┻━━━━━━━━━┻━━━━━━━━━┻━━━━━━━━━┻━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━╮  ╭━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━┻━━━━━━━━━┻━━━━━━━━━┻━━━━━━━━━┻━━━━━━━━━╯
  	*/                                            _______,  _______,  _______,     _______,  _______,  _______									   	    \
  	//      								   ╰─━━━━━━━━┻━━━━━━━━━┻━━━━━━━━━╯  ╰━━━━━━━━━┻━━━━━━━━━┻━━━━━━━━━╯
 	),
};


