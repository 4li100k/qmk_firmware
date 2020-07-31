#include QMK_KEYBOARD_H

#define _base 0
#define _fn 1
#define _pn 2

#define _timers 1
struct Timer {
    bool active; // whether to run or not
    uint16_t time; // for time checking
    int frequency; // how often should it fire
    void (*effect)(void); // what should it do
};

void reset_function(void){
    reset_keyboard();
};
struct Timer reset = {.frequency = 3000, .effect = reset_function};

struct Timer *timers[_timers];

int  resets_held = 0;

enum my_keycodes{
    HOLD_RESET = SAFE_RANGE,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_base] = LAYOUT_60_iso( /* Base */
        KC_GESC,  KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_BSPC,\
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,          \
        MO(_fn),  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,  KC_NUHS,  KC_ENT, \
        KC_LSFT,  KC_NUBS,  KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,            KC_RSFT,\
        KC_LCTL,  KC_LGUI,  KC_LALT,                                KC_SPC,                                 KC_RALT,  MO(_fn),  MO(_pn),  KC_RCTL),

    [_fn] = LAYOUT_60_iso( /* FN */
        KC_GRV,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,   KC_DEL ,\
        XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  KC_PGUP,  KC_UP,    KC_PGDN,  KC_PSCR,  XXXXXXX,  KC_VOLU,          \
        XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  KC_HOME,  KC_LEFT,  KC_DOWN,  KC_RGHT,  KC_END,   KC_MUTE,  KC_VOLD,  XXXXXXX,\
        XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,            XXXXXXX,\
        KC_MPRV,  KC_MPLY,  KC_MNXT,                                XXXXXXX,                                XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX),

    [_pn] = LAYOUT_60_iso( /* FN */
        HOLD_RESET,XXXXXXX, XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  HOLD_RESET,\
        XXXXXXX,  S1_DEC,   S1_INC,   S2_DEC,   S2_INC,   ES_DEC,   ES_INC,   XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  BR_INC,           \
        XXXXXXX,  EF_DEC,   EF_INC,   H1_DEC,   H1_INC,   H2_DEC,   H2_INC,   XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  BR_DEC,   XXXXXXX,\
        XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,            XXXXXXX,\
        XXXXXXX,  XXXXXXX,  XXXXXXX,                                XXXXXXX,                                XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX),

};

void matrix_init_user(void) {
    //user initialization
}

void keyboard_post_init_user(void) { // on end of keyboard init

    timers[0] = &reset;
    for (int i = 0; i < _timers; i++){
        timers[i]->active = false;
        timers[i]->time = timer_read();
    }

};

void matrix_scan_user(void) {
    for (int i = 0; i < _timers; i++){
        if (timers[i]->active){
            if (timer_elapsed(timers[i]->time) > timers[i]->frequency){
                timers[i]->effect();
                timers[i]->time = timer_read();
            }
        }
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case HOLD_RESET: // enter boot mode
            if (record->event.pressed) {
                resets_held++;
                if (resets_held == 2){
                    reset.time = timer_read();
                    reset.active = true;
                }
            } else {
                resets_held--;
                if (resets_held == 1){
                    reset.active = false;
                }
            }
            return false;
        
        default:
            return true;  // Process all other keycodes normally
    }
}
