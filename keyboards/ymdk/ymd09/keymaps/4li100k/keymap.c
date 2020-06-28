#include QMK_KEYBOARD_H

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [0] = LAYOUT(
        KC_KP_7, KC_KP_8, KC_KP_9,
        KC_KP_4, LT(1, KC_KP_5), KC_KP_6,
        KC_KP_1, STRUGGLE, CUTLASS
    ),

    [1] = LAYOUT(
        RGB_RMOD, RGB_VAI, RGB_MOD,
        RGB_HUI, KC_TRNS, RGB_SAI,
        RGB_HUD, RGB_VAD, RGB_SAD
    ),

};

/* 0
 * ┌──────┬─────────────┬──────┐
 * │  7   │ 8 with beep │  9   │
 * ├──────┼─────────────┼──────┤
 * │  4   │LT(1│KC KP 5)│  6   │
 * ├──────┼─────────────┼──────┤
 * │  1   │  STRUGGLE   │CUTLSS│
 * └──────┴─────────────┴──────┘
 */

/* 1
 * ┌──────┬──────┬──────┐
 * │ Ld M-│ Ld V+│ Ld M+│
 * ├──────┼──────┼──────┤
 * │ Ld H+│ trns │ Ld S+│
 * ├──────┼──────┼──────┤
 * │ Ld H-│ Ld V-│ Ld S-│
 * └──────┴──────┴──────┘
 */

uint16_t struggle_timer = 0; // 0 is arbitrary, will get replaced by timestamp
bool should_struggle = false;
bool is_charging_cutlass = false;

// here enumerate custom keycodes -> name them and assign them a unique number -> use them inside of my keymap
enum my_keycodes(
    STRUGGLE = SAFE_RANGE,
    CUTLASS,
    EXAMPLE,

);

// to my understanding, this is an override of a function that is called before handling a keypress
bool process_record_user(uint16_t keycode, keyrecord_t *record) {  // keycode contains whatever is in the keymap, record contains the event
    switch (keycode) {
        case STRUGGLE: // dead by daylight hook struggle
            if (record->event.pressed) {
                if (should_struggle) {
                    should_struggle = false;
                } else {
                    struggle_timer = timer_read();
                    should_struggle = true;
                    PLAY_SONG(tone_qwerty);
                }
            }
            return false;

        case CUTLASS: // sea of thieves charged leap
            if (record->event.pressed) {
                if (is_charging_cutlass) return false;
                is_charging_cutlass = true;
                register_code(KC_MS_BTN1);
                register_code(KC_MS_BTN2);
                SEND_STRING(SS_DELAY(2000));
                tap_code(KC_SPACE);
                unregister_code(KC_MS_BTN1);
                unregister_code(KC_MS_BTN2);
                is_charging_cutlass = false;
            }
            return false;

        case KC_KP_8: // Play a tone when enter is pressed
            if (record->event.pressed) {
                PLAY_SONG(tone_qwerty);
            }
            return true;

        case EXAMPLE:
            if (record->event.pressed) {  // Do something when pressed
                
                register_code(KC_F22); //this means to send F22 down
                unregister_code(KC_F22); //this means to send F22 up
                tap_code(KC_F22); // registers and unregisters code
                clear_keyboard(); // unpress everything

                SEND_STRING(SS_TAP(X_F22)); // presses and releases a key
                SEND_STRING(SS_DOWN(X_F22)); // presses (but does not release) a key
                SEND_STRING(SS_UP(X_F22)); // releases a key
                SEND_STRING("minh"SS_TAP(X_LEFT)"i"SS_TAP(X_RIGHT)"ydra");
                SEND_STRING(SS_DELAY(1000)); // waits 1 second

                char str[2] = ":D";
                send_string(str); // must be lowercase for vars
                SEND_STRING(":D"); // must be uppercase for literals




            } else {  // Do something else when release
            
            }
            return false;  // Skip all further processing of this key
        
        default:
            return true;  // Process all other keycodes normally
    }
}


void matrix_scan_user(void) { // handles timer
  if (should_struggle) {
    if (timer_elapsed(struggle_timer) > 100) {
        tap_code(KC_SPACE);
        struggle_timer = timer_read();
    }
  }
}


