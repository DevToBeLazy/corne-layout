#include "quantum.h"

#ifndef LAYER_INDICATOR_BRIGHTNESS_INC
#    define LAYER_INDICATOR_BRIGHTNESS_INC 22
#endif

#ifndef CAPSLOCK_INDICATOR_BRIGHTNESS_INC
#    define CAPSLOCK_INDICATOR_BRIGHTNESS_INC 76
#endif

#ifndef RGB_MATRIX_MAXIMUM_BRIGHTNESS
#    define RGB_MATRIX_MAXIMUM_BRIGHTNESS 80
#endif

#ifdef RGB_MATRIX_ENABLE
static HSV _get_hsv_for_layer_index(uint8_t layer) {
    switch (layer) {
        case 1:
            return (HSV){HSV_BLUE};
        case 2:
            return (HSV){HSV_AZURE};
        case 3:
            return (HSV){HSV_ORANGE};
        case 4:
            return (HSV){HSV_GREEN};
        case 5:
            return (HSV){HSV_TEAL};
        case 6:
            return (HSV){HSV_PURPLE};
        default:
            return (HSV){HSV_RED};
    };
}

// Layer state indicator
bool rgb_matrix_indicators_advanced_kb(uint8_t led_min, uint8_t led_max) {
    if (!rgb_matrix_indicators_advanced_user(led_min, led_max)) {
        return false;
    }
    // Set indicator LEDs to red if caps lock is enabled
    if (host_keyboard_led_state().caps_lock) {
        for (int i = led_min; i <= led_max; i++) {
            if (HAS_FLAGS(g_led_config.flags[i], LED_FLAG_MODIFIER)) {
                // set modifier-flagged LEDs to a pure a configured interval brighter than the current brightness, clamped to 255 (ie. uint8_t max value).
                rgb_matrix_set_color(i, MIN(rgb_matrix_get_val() + CAPSLOCK_INDICATOR_BRIGHTNESS_INC, RGB_MATRIX_MAXIMUM_BRIGHTNESS), 0, 0);
            }
        }
    }

    // Set underglow to a solid color for highest active layer apart from the base layer.
    const uint8_t layer = get_highest_layer(layer_state);
    if (layer > 0) {
        HSV hsv = _get_hsv_for_layer_index(layer);

        // Set brightness to the configured interval brighter than current brightness, clamped to 255 (ie. uint8_t max value). This compensates for the dimmer appearance of the underglow LEDs.
        hsv.v         = MIN(rgb_matrix_get_val() + LAYER_INDICATOR_BRIGHTNESS_INC, RGB_MATRIX_MAXIMUM_BRIGHTNESS);
        const RGB rgb = hsv_to_rgb(hsv);

        for (int i = led_min; i < led_max; i++) {
            if (HAS_FLAGS(g_led_config.flags[i], LED_FLAG_UNDERGLOW)) {
                rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
            }
        }
    }
    return true;
};
#endif // RGB_MATRIX_ENABLE
