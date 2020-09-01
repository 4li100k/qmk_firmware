// import the core
#include QMK_KEYBOARD_H


// label the layers
#define _base 0
#define _fn 1
#define _pn 2


// declare timers
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


// custom keycodes
enum my_keycodes{ // macro enums
    HOLD_RESET = SAFE_RANGE,
};


// tap dance
enum { // tapdance possible outcomes
    SINGLE_TAP = 1,
    SINGLE_HOLD,
    DOUBLE_TAP,
    DOUBLE_HOLD,
    DOUBLE_SINGLE_TAP, // Send two single taps
};

enum { // tap dance keycodes
    TD_FN_PN,
};

uint8_t cur_dance(qk_tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed) return SINGLE_TAP;
        else return SINGLE_HOLD;
    } else if (state->count == 2) {
        if (state->interrupted) return DOUBLE_SINGLE_TAP;
        else if (state->pressed) return DOUBLE_HOLD;
        else return DOUBLE_TAP;
    } else { // 3 or more
        if (state->interrupted) return DOUBLE_SINGLE_TAP;
        else if (state->pressed) return DOUBLE_HOLD;
        else return DOUBLE_TAP;
    }
};

int fn_pn_tap_state = 0;

void fn_pn_finished(qk_tap_dance_state_t *state, void *user_data) {
    fn_pn_tap_state = cur_dance(state);
    switch (fn_pn_tap_state) {
        case SINGLE_TAP: layer_on(_fn); break;
        case SINGLE_HOLD: layer_on(_fn); break;
        case DOUBLE_TAP: layer_on(_pn); break;
        case DOUBLE_HOLD: layer_on(_pn); break;
        case DOUBLE_SINGLE_TAP: layer_on(_pn); break;
    }
};

void fn_pn_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (fn_pn_tap_state) {
        case SINGLE_TAP: layer_off(_fn); break;
        case SINGLE_HOLD: layer_off(_fn); break;
        case DOUBLE_TAP: layer_off(_pn); break;
        case DOUBLE_HOLD: layer_off(_pn); break;
        case DOUBLE_SINGLE_TAP: layer_off(_pn); break;
    }
    fn_pn_tap_state = 0;
};

qk_tap_dance_action_t tap_dance_actions[] = { // tap dance definitions
    [TD_FN_PN] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, fn_pn_finished, fn_pn_reset),
};









// layout keycodes
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_base] = LAYOUT_60_iso( /* Base */
        KC_ESC,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_BSPC,\
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,          \
        /*TD(TD_FN_PN)*/MO(_fn),KC_A,  KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,  KC_NUHS,  KC_ENT, \
        KC_LSFT,  KC_NUBS,  KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,            KC_RSFT,\
        KC_LCTL,  KC_LGUI,  KC_LALT,                                KC_SPC,                                 KC_RALT,  MO(_fn),  MO(_pn),  KC_RCTL),

    [_fn] = LAYOUT_60_iso( /* FN */
        KC_GRV,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,   KC_DEL ,\
        XXXXXXX, KC_MS_BTN1,KC_MS_UP,KC_MS_BTN2,XXXXXXX,  XXXXXXX,  XXXXXXX,  KC_PGUP,  KC_UP,    KC_PGDN,  KC_PSCR,  XXXXXXX,  KC_VOLU,          \
      KC_TRNS,KC_MS_LEFT,KC_MS_DOWN,KC_MS_RIGHT,XXXXXXX,  XXXXXXX,  KC_HOME,  KC_LEFT,  KC_DOWN,  KC_RGHT,  KC_END,   KC_MUTE,  KC_VOLD,  XXXXXXX,\
        KC_TRNS,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,            KC_TRNS,\
        KC_MPRV,  KC_MPLY,  KC_MNXT,                                XXXXXXX,                                XXXXXXX,  KC_TRNS,  XXXXXXX,  XXXXXXX),

    [_pn] = LAYOUT_60_iso( /* PN */
        HOLD_RESET,XXXXXXX, XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  HOLD_RESET,\
        XXXXXXX,  S1_DEC,   S1_INC,   S2_DEC,   S2_INC,   ES_DEC,   ES_INC,   XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  BR_INC,           \
        XXXXXXX,  EF_DEC,   EF_INC,   H1_DEC,   H1_INC,   H2_DEC,   H2_INC,   XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  BR_DEC,   XXXXXXX,\
        XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,            XXXXXXX,\
        XXXXXXX,  XXXXXXX,  XXXXXXX,                                XXXXXXX,                                XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX),
};


// callback functions
void keyboard_post_init_user(void) { // last step of keyboard init
    timers[0] = &reset;
    for (int i = 0; i < _timers; i++){
        timers[i]->active = false;
        timers[i]->time = timer_read();
    }
};

void matrix_scan_user(void) { // periodically called function
    for (int i = 0; i < _timers; i++){
        if (timers[i]->active){
            if (timer_elapsed(timers[i]->time) > timers[i]->frequency){
                timers[i]->effect();
                timers[i]->time = timer_read();
            }
        }
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) { // keypress handler
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
