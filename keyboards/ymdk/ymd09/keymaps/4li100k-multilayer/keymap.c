#include QMK_KEYBOARD_H

#define _key1 0
#define _key2 1
#define _key3 2
#define _key4 3
#define _key5 4
#define _key6 5
#define _key7 6
#define _key8 7
#define _key9 8

#define _timers 4
struct Timer {
    bool active; // whether to run or not
    uint16_t time; // for time checking
    int frequency; // how often should it fire
    void (*effect)(void); // what should it do
};

void dbd_struggle_function(void){
    tap_code(KC_SPACE);
};
struct Timer dbd_struggle = {.frequency = 100, .effect = dbd_struggle_function};

void dbd_wiggle_function(void){
    tap_code(KC_A);
    tap_code(KC_D);
};
struct Timer dbd_wiggle = {.frequency = 100, .effect = dbd_wiggle_function};

void spam_m1_function(void){
    tap_code(KC_MS_BTN1);
};
struct Timer spam_m1 = {.frequency = 50, .effect = spam_m1_function};

void reset_function(void){
    reset_keyboard();
};
struct Timer reset = {.frequency = 3000, .effect = reset_function};

struct Timer *timers[_timers];

bool is_charging_cutlass = false;

bool reset_is_held = false;

// here enumerate custom keycodes -> name them and assign them a unique number -> use them inside of my keymap
enum my_keycodes{
    DBD_STRUGGLE = SAFE_RANGE,
    DBD_WIGGLE,

    SOT_CUTLASS,

    SPAM_M1,
    RANDOM_KEY,

    HOLD_RESET,
    EXAMPLE,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [_key1] = LAYOUT( // top left
        KC_A,  KC_NO, RANDOM_KEY,
        KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, MO(_key9)
    ),

    [_key2] = LAYOUT( // top
        KC_B,  DBD_WIGGLE, DBD_STRUGGLE,
        KC_NO, KC_NO,      KC_NO,
        KC_NO, KC_NO,      KC_TRNS
    ),

    [_key3] = LAYOUT( // top right
        KC_C,  KC_NO, SPAM_M1,
        KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_TRNS
    ),

    [_key4] = LAYOUT( // middle left
        KC_D,  KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_TRNS
    ),

    [_key5] = LAYOUT( // middle
        KC_E,  KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_TRNS
    ),

    [_key6] = LAYOUT( // middle right
        KC_F,  KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_TRNS
    ),

    [_key7] = LAYOUT( // bottom left
        KC_G,  KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_TRNS
    ),

    [_key8] = LAYOUT( // bottom
        KC_MS_BTN1, KC_MS_UP,   KC_MS_BTN2,
        KC_MS_LEFT, KC_MS_DOWN, KC_MS_RIGHT,
        HOLD_RESET, HOLD_RESET, KC_TRNS
    ),

    [_key9] = LAYOUT(
        TO(_key1), TO(_key2), TO(_key3),
        TO(_key4), TO(_key5), TO(_key6),
        TO(_key7), TO(_key8), KC_TRNS
    ),

};

/* 8
 * ┌──────┬──────┬──────┐
 * │      │      │      │
 * ├──────┼──────┼──────┤
 * │      │      │      │
 * ├──────┼──────┼──────┤
 * │      │      │  Fn  │
 * └──────┴──────┴──────┘
 */



bool process_record_user(uint16_t keycode, keyrecord_t *record) {  // keycode contains whatever is in the keymap, record contains the event
    switch (keycode) {
        case SPAM_M1: // spam mouse1 while held
            if (record->event.pressed) {
                spam_m1.active = true;
            } else {
                spam_m1.active = false;
            }
            return false;

        case HOLD_RESET: // enter boot mode
            if (record->event.pressed) {
                if (reset_is_held) {
                    reset.time = timer_read();
                    reset.active = true;
                } else {
                    reset_is_held = true;
                }
            } else {
                reset_is_held = false;
                reset.active = false;
            }
            return false;

        case RANDOM_KEY: // random key (0–25 is A–Z, 26–51 is a–z, 52–61 is 0–9, 62 is + and 63 is /)
            if (record->event.pressed) {
                tap_random_base64();
            }
            return false;

        case DBD_STRUGGLE: // dead by daylight hook struggle
            if (record->event.pressed) {
                dbd_struggle.active = true;
            } else {
                dbd_struggle.active = false;
            }
            return false;

        case DBD_WIGGLE: // dead by daylight wiggle
            if (record->event.pressed) {
                dbd_wiggle.active = true;
            } else {
                dbd_wiggle.active = false;
            }
            return false;

        case SOT_CUTLASS: // sea of thieves charged leap
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
};

uint32_t set_led_based_on_layer(uint32_t state) {

    rgblight_setrgb(0, 0, 0);

    switch(biton32(state)) {
        case _key1:
            rgblight_setrgb_at(0, 0, 0, _key9); // blank
            break;
        case _key2:
            rgblight_setrgb_at(32, 32, 32, _key9); // poor

            rgblight_setrgb_at(32, 32, 32, _key2);
            rgblight_setrgb_at(32, 32, 32, _key3);
            break;
        case _key3:
            rgblight_setrgb_at(255, 255, 255, _key9); // common

            rgblight_setrgb_at(255, 255, 255, _key3);
            break;
        case _key4:
            rgblight_setrgb_at(30, 255, 0, _key9); // uncommon
        break;
        case _key5:
            rgblight_setrgb_at(0, 112, 221, _key9); // rare
            break;
        case _key6:
            rgblight_setrgb_at(163, 0, 238, _key9); // epic
            break;
        case _key7:
            rgblight_setrgb_at(255, 64, 0, _key9); // legendary
            break;
        case _key8:
            rgblight_setrgb_at(255, 0, 0, _key9); // mythical
            break;
        case _key9:
            rgblight_setrgb_at(32, 32, 32, _key2); // poor
            rgblight_setrgb_at(255, 255, 255, _key3); // common
            rgblight_setrgb_at(30, 255, 0, _key4); // uncommon
            rgblight_setrgb_at(0, 112, 221, _key5); // rare
            rgblight_setrgb_at(163, 0, 238, _key6); // epic
            rgblight_setrgb_at(255, 64, 0, _key7); // legendary
            rgblight_setrgb_at(255, 0, 0, _key8); // mythical
            break;
        default:

            break;
    }
    return state;
};

void keyboard_post_init_user(void) {
    set_led_based_on_layer(layer_state);
    timers[0] = &dbd_struggle;
    timers[1] = &dbd_wiggle;
    timers[2] = &spam_m1;
    timers[3] = &reset;
    for (int i = 0; i < _timers; i++){
        timers[i]->active = false;
        timers[i]->time = timer_read();
    }
};

uint32_t layer_state_set_user(uint32_t state) {
    return set_led_based_on_layer(state);
};

void matrix_scan_user(void) {
    for (int i = 0; i < _timers; i++){
        if (timers[i]->active){
            if (timer_elapsed(timers[i]->time) > timers[i]->frequency){
                timers[i]->effect();
                timers[i]->time = timer_read();
            }
        }
    };
};
