'use strict';

goog.require('Blockly.JavaScript');

Blockly.JavaScript['edubot_is_moving'] = function(block) {
    return ['is_moving()', Blockly.JavaScript.ORDER_FUNCTION_CALL];
};

Blockly.JavaScript['edubot_set_max_velocity'] = function(block) {
    var max_vel = Blockly.JavaScript.valueToCode(block, 'MAX_VEL', Blockly.JavaScript.ORDER_ATOMIC);
    return 'set_max_velocity(' + max_vel + ');\n';
};

Blockly.JavaScript['edubot_set_accel'] = function(block) {
    var accel = Blockly.JavaScript.valueToCode(block, 'ACCEL', Blockly.JavaScript.ORDER_ATOMIC);
    return 'set_acceleration(' + accel + ');\n';
};

Blockly.JavaScript['edubot_motor_set_step'] = function(block) {
    var l_step = Blockly.JavaScript.valueToCode(block, 'L_STEP', Blockly.JavaScript.ORDER_ATOMIC);
    var r_step = Blockly.JavaScript.valueToCode(block, 'R_STEP', Blockly.JavaScript.ORDER_ATOMIC);
    return 'move_by_step(' + l_step + ', ' + r_step + ');\n';
};

Blockly.JavaScript['edubot_motor_set_velocity'] = function(block) {
    var l_vel = Blockly.JavaScript.valueToCode(block, 'L_VEL', Blockly.JavaScript.ORDER_ATOMIC);
    var r_vel = Blockly.JavaScript.valueToCode(block, 'R_VEL', Blockly.JavaScript.ORDER_ATOMIC);
    return 'move_by_velocity(' + l_vel + ', ' + r_vel + ');\n';
};

Blockly.JavaScript['edubot_motor_set_distance'] = function(block) {
    var l_dist = Blockly.JavaScript.valueToCode(block, 'L_DIST', Blockly.JavaScript.ORDER_ATOMIC);
    var r_dist = Blockly.JavaScript.valueToCode(block, 'R_DIST', Blockly.JavaScript.ORDER_ATOMIC);
    return 'move_by_distance(' + l_dist + ', ' + r_dist + ');\n';
};

Blockly.JavaScript['edubot_motor_move_forward'] = function(block) {
    return 'move_forward();\n';
};

Blockly.JavaScript['edubot_motor_move_backward'] = function(block) {
    return 'move_backward();\n';
};

Blockly.JavaScript['edubot_motor_stop'] = function(block) {
    return 'stop();\n';
};

Blockly.JavaScript['edubot_motor_turn_left'] = function(block) {
    return 'turn_left();\n';
};

Blockly.JavaScript['edubot_motor_turn_right'] = function(block) {
    return 'turn_right();\n';
};

Blockly.JavaScript['edubot_misc_set_colour_led'] = function(block) {
    var l_color = Blockly.JavaScript.valueToCode(block, 'L_COLOR', Blockly.JavaScript.ORDER_NONE) || '\'\'';
    var r_color = Blockly.JavaScript.valueToCode(block, 'R_COLOR', Blockly.JavaScript.ORDER_NONE) || '\'\'';
    return 'set_color_led(' + l_color + ', ' + r_color + ');\n';
};

Blockly.JavaScript['edubot_misc_turn_off_leds'] = function(block) {
    return 'turn_off_leds();\n';
};

Blockly.JavaScript['edubot_sleep'] = function(block) {
    var msec = Blockly.JavaScript.valueToCode(block, 'TIME', Blockly.JavaScript.ORDER_ATOMIC);
    return 'sleep_ms(' + msec + ');\n';
};

Blockly.JavaScript['edubot_misc_set_text'] = function(block) {
    var msg = Blockly.JavaScript.valueToCode(block, 'TEXT', Blockly.JavaScript.ORDER_NONE) || '\'\'';
    return 'set_text_to_lcd(' + msg + ');\n';
};

Blockly.JavaScript['edubot_misc_set_image'] = function(block) {
    var index = Blockly.JavaScript.valueToCode(block, 'INDEX', Blockly.JavaScript.ORDER_ATOMIC);
    return 'set_image_to_lcd(' + index + ');\n';
};

Blockly.JavaScript['edubot_misc_clear_display'] = function(block) {
    return 'clear_display();\n';
};

Blockly.JavaScript['edubot_misc_button_state'] = function(block) {
    return ['get_button_state()', Blockly.JavaScript.ORDER_FUNCTION_CALL];
};

Blockly.JavaScript['edubot_misc_is_button_pressed'] = function(block) {
    return ['is_button_pressed()', Blockly.JavaScript.ORDER_FUNCTION_CALL];
};

Blockly.JavaScript['edubot_misc_get_batt_voltage'] = function(block) {
    return ['get_battery_level()', Blockly.JavaScript.ORDER_FUNCTION_CALL];
};

Blockly.JavaScript['edubot_misc_is_need_charging'] = function(block) {
    return ['is_need_charging()', Blockly.JavaScript.ORDER_FUNCTION_CALL];
};

Blockly.JavaScript['edubot_sensor_floor_sensor'] = function(block) {
    return ['get_floor_sensor_value(\'' + block.getFieldValue('DIRECTION') + '\')', Blockly.JavaScript.ORDER_FUNCTION_CALL];
};

Blockly.JavaScript['edubot_sensor_distance_sensor'] = function(block) {
    return ['get_distance_sensor_value(\'' + block.getFieldValue('DIRECTION') + '\')', Blockly.JavaScript.ORDER_FUNCTION_CALL];
};

Blockly.JavaScript['edubot_sensor_imu'] = function(block) {
    return ['get_imu_sensor_value(\'' + block.getFieldValue('AXIS') + '\')', Blockly.JavaScript.ORDER_FUNCTION_CALL];
};

Blockly.JavaScript['edubot_sensor_accel'] = function(block) {
    return ['get_accelerometer_sensor_value(\'' + block.getFieldValue('AXIS') + '\')', Blockly.JavaScript.ORDER_FUNCTION_CALL];
};

Blockly.JavaScript['edubot_sensor_gyro'] = function(block) {
    return ['get_gyro_sensor_value(\'' + block.getFieldValue('AXIS') + '\')', Blockly.JavaScript.ORDER_FUNCTION_CALL];
};





