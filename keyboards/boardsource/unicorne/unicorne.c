// Copyright 2023 jack (@waffle87)
// SPDX-License-Identifier: GPL-2.0-or-later
#include "unicorne.h"

#ifdef OLED_ENABLE
oled_rotation_t oled_init_kb(oled_rotation_t rotation) {
    return rotation;
}

bool oled_task_kb(void) {
    if (!oled_task_user()) {
        return false;
    }
    switch (get_highest_layer(layer_state)) {
        case 0:
            oled_write_raw(layer_zero, sizeof(layer_zero));
            break;
        case 1:
            oled_write_raw(layer_one, sizeof(layer_one));
            break;
        case 2:
            oled_write_raw(layer_two, sizeof(layer_two));
            break;
        case 3:
            oled_write_raw(layer_three, sizeof(layer_three));
            break;
        }
    return false;
}
#endif
