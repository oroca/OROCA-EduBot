'use strict';

goog.require('Blockly.Blocks');
goog.require('Blockly');

Blockly.defineBlocksWithJsonArray([
{
    "type": "edubot_is_moving",
    "message0": "%{BKY_LABEL_IS_MOVING}",
    "output": "Boolean",
    "style": "move_blocks",
    "tooltip": "is moving?",
    "helpUrl": "https://github.com/oroca/OROCA-EduBot"
},
{
    "type": "edubot_set_max_velocity",
    "message0": "%{BKY_LABEL_SET_MAX_VELOCITY}",
    "args0": [
                {
                    "type": "input_value",
                    "name": "MAX_VEL",
                    "check": "Number"
                },
    ],
    "previousStatement": null,
    "nextStatement": null,
    "inputsInline": true,
    "style": "move_blocks",
    "tooltip": "set maximum velocity",
    "helpUrl": "https://github.com/oroca/OROCA-EduBot"
},
{
    "type": "edubot_set_accel",
    "message0": "%{BKY_LABEL_SET_ACCELERATION}",
    "args0": [
                {
                    "type": "input_value",
                    "name": "ACCEL",
                    "check": "Number"
                },
    ],
    "previousStatement": null,
    "nextStatement": null,
    "inputsInline": true,
    "style": "move_blocks",
    "tooltip": "set acceleration",
    "helpUrl": "https://github.com/oroca/OROCA-EduBot"
},
{
    "type": "edubot_motor_set_step",
    "message0": "%{BKY_LABEL_MOVE_SET_STEP}",
    "args0": [
                {
                    "type": "input_value",
                    "name": "L_STEP",
                    "check": "Number"
                },
                {
                    "type": "input_value",
                    "name": "R_STEP",
                    "check": "Number"
                }
    ],
    "previousStatement": null,
    "nextStatement": null,
    "inputsInline": true,
    "style": "move_blocks",
    "tooltip": "move by step",
    "helpUrl": "https://github.com/oroca/OROCA-EduBot"
},
{
    "type": "edubot_motor_set_velocity",
    "message0": "%{BKY_LABEL_MOVE_SET_VELOCITY}",
    "args0": [
                {
                    "type": "input_value",
                    "name": "L_VEL",
                    "check": "Number"
                },
                {
                    "type": "input_value",
                    "name": "R_VEL",
                    "check": "Number"
                }
    ],
    "previousStatement": null,
    "nextStatement": null,
    "inputsInline": true,
    "style": "move_blocks",
    "tooltip": "move by velocity",
    "helpUrl": "https://github.com/oroca/OROCA-EduBot"
},
{
    "type": "edubot_motor_set_distance",
    "message0": "%{BKY_LABEL_MOVE_SET_DISTANCE}",
    "args0": [
                {
                    "type": "input_value",
                    "name": "L_DIST",
                    "check": "Number"
                },
                {
                    "type": "input_value",
                    "name": "R_DIST",
                    "check": "Number"
                }
    ],
    "previousStatement": null,
    "nextStatement": null,
    "inputsInline": true,
    "style": "move_blocks",
    "tooltip": "move by distance",
    "helpUrl": "https://github.com/oroca/OROCA-EduBot"
},
{
    "type": "edubot_motor_move_forward",
    "message0": "%{BKY_LABEL_MOVE_FORWARD}",
    "previousStatement": null,
    "nextStatement": null,
    "inputsInline": true,
    "style": "move_blocks",
    "tooltip": "move forward",
    "helpUrl": "https://github.com/oroca/OROCA-EduBot"
},
{
    "type": "edubot_motor_move_backward",
    "message0": "%{BKY_LABEL_MOVE_BACKWARD}",
    "previousStatement": null,
    "nextStatement": null,
    "inputsInline": true,
    "style": "move_blocks",
    "tooltip": "move backward",
    "helpUrl": "https://github.com/oroca/OROCA-EduBot"
},
{
    "type": "edubot_motor_stop",
    "message0": "%{BKY_LABEL_MOVE_STOP}",
    "previousStatement": null,
    "nextStatement": null,
    "inputsInline": true,
    "style": "move_blocks",
    "tooltip": "stop",
    "helpUrl": "https://github.com/oroca/OROCA-EduBot"
},
{
    "type": "edubot_motor_turn_left",
    "message0": "%{BKY_LABEL_TURN_LEFT}",
    "previousStatement": null,
    "nextStatement": null,
    "inputsInline": true,
    "style": "move_blocks",
    "tooltip": "turn left",
    "helpUrl": "https://github.com/oroca/OROCA-EduBot"
},
{
    "type": "edubot_motor_turn_right",
    "message0": "%{BKY_LABEL_TURN_RIGHT}",
    "previousStatement": null,
    "nextStatement": null,
    "inputsInline": true,
    "style": "move_blocks",
    "tooltip": "turn right",
    "helpUrl": "https://github.com/oroca/OROCA-EduBot"
},
{
    "type": "edubot_sleep",
    "message0": "%{BKY_LABEL_SLEEP}",
    "args0": [
             {
             "type": "input_value",
             "name": "TIME",
             "check": "Number"
             }
             ],
    "previousStatement": null,
    "nextStatement": null,
    "style": "move_blocks",
    "tooltip": "sleep for desired milliseconds",
    "helpUrl": "https://github.com/oroca/OROCA-EduBot"
},
{
    "type": "edubot_misc_set_colour_led",
    "message0": "%{BKY_LABEL_MISC_SET_COLOR}",
    "args0": [
                {
                    "type": "input_value",
                    "name": "L_COLOR",
                    "check": "Colour"
                },
                {
                    "type": "input_value",
                    "name": "R_COLOR",
                    "check": "Colour"
                }
        ],
    "previousStatement": null,
    "nextStatement": null,
    "inputsInline": true,
    "style": "output_blocks",
    "tooltip": "set color rgb led",
    "helpUrl": "https://github.com/oroca/OROCA-EduBot"
},
{
    "type": "edubot_misc_turn_off_leds",
    "message0": "%{BKY_LABEL_MISC_TURN_OFF_LEDS}",
    "previousStatement": null,
    "nextStatement": null,
    "inputsInline": true,
    "style": "output_blocks",
    "tooltip": "turn off LEDs",
    "helpUrl": "https://github.com/oroca/OROCA-EduBot"
},
{
    "type": "edubot_misc_set_text",
    "message0": "%{BKY_LABEL_MISC_SET_TEXT}",
    "args0": [
                {
                    "type": "input_value",
                    "name": "TEXT",
                    "check": "String"
                }
        ],
    "previousStatement": null,
    "nextStatement": null,
    "inputsInline": true,
    "style": "output_blocks",
    "tooltip": "set text to lcd",
    "helpUrl": "https://github.com/oroca/OROCA-EduBot"
},
{
    "type": "edubot_misc_set_image",
    "message0": "%{BKY_LABEL_MISC_SET_IMAGE}",
    "args0": [
                {
                    "type": "input_value",
                    "name": "INDEX",
                    "check": "Number"
                }
        ],
    "previousStatement": null,
    "nextStatement": null,
    "inputsInline": true,
    "style": "output_blocks",
    "tooltip": "set image to lcd",
    "helpUrl": "https://github.com/oroca/OROCA-EduBot"
},
{
    "type": "edubot_misc_clear_display",
    "message0": "%{BKY_LABEL_MISC_CLEAR_DISPLAY}",
    "previousStatement": null,
    "nextStatement": null,
    "inputsInline": true,
    "style": "output_blocks",
    "tooltip": "clear display",
    "helpUrl": "https://github.com/oroca/OROCA-EduBot"
},
{
    "type": "edubot_misc_button_state",
    "message0": "%{BKY_LABEL_MISC_GET_BUTTON_STATE}",
    "output": "Number",
    "style": "input_blocks",
    "tooltip": "get button state",
    "helpUrl": "https://github.com/oroca/OROCA-EduBot"
},
{
    "type": "edubot_misc_is_button_pressed",
    "message0": "%{BKY_LABEL_MISC_IS_BUTTON_PRESSED}",
    "output": "Boolean",
    "style": "input_blocks",
    "tooltip": "is button pressed?",
    "helpUrl": "https://github.com/oroca/OROCA-EduBot"
},
{
    "type": "edubot_misc_get_batt_voltage",
    "message0": "%{BKY_LABEL_MISC_GET_BATTERY_LEVEL}",
    "output": "Number",
    "style": "input_blocks",
    "tooltip": "get button state",
    "helpUrl": "https://github.com/oroca/OROCA-EduBot"
},
{
    "type": "edubot_misc_is_need_charging",
    "message0": "%{BKY_LABEL_MISC_NEED_CHARGING}",
    "output": "Boolean",
    "style": "input_blocks",
    "tooltip": "need charging?",
    "helpUrl": "https://github.com/oroca/OROCA-EduBot"
},
{
    "type": "edubot_sensor_floor_sensor",
    "message0": "%{BKY_LABEL_SENSOR_FLOOR_SENSOR}",
    "args0": [
         {
             "type": "field_dropdown",
             "name": "DIRECTION",
             "options": [
                     [
                      "%{BKY_FIELD_LABEL_LEFT_OUTER}",
                      "LEFT_OUTER"
                      ],
                     [
                      "%{BKY_FIELD_LABEL_LEFT_INNER}",
                      "LEFT_INNER"
                      ],
                     [
                      "%{BKY_FIELD_LABEL_RIGHT_INNER}",
                      "RIGHT_INNER"
                      ],
                      [
                      "%{BKY_FIELD_LABEL_RIGHT_OUTER}",
                      "RIGHT_OUTER"
                      ]
            ]
         }
     ],
    "output": "Number",
    "style": "sensor_blocks",
    "tooltip": "get imu sensor",
    "helpUrl": "https://github.com/oroca/OROCA-EduBot"
},
{
    "type": "edubot_sensor_distance_sensor",
    "message0": "%{BKY_LABEL_SENSOR_DISTANCE_SENSOR}",
    "args0": [
         {
             "type": "field_dropdown",
             "name": "DIRECTION",
             "options": [
                     [
                      "%{BKY_FIELD_LABEL_LEFT}",
                      "LEFT"
                      ],
                     [
                      "%{BKY_FIELD_LABEL_RIGHT}",
                      "RIGHT"
                      ],
            ]
         }
     ],
    "output": "Number",
    "style": "sensor_blocks",
    "tooltip": "get imu sensor",
    "helpUrl": "https://github.com/oroca/OROCA-EduBot"
},
{
    "type": "edubot_sensor_imu",
    "message0": "%{BKY_LABEL_SENSOR_IMU_SENSOR}",
    "args0": [
         {
             "type": "field_dropdown",
             "name": "AXIS",
             "options": [
                     [
                      "roll",
                      "ROLL"
                      ],
                     [
                      "pitch",
                      "PITCH"
                      ],
                     [
                      "yaw",
                      "YAW"
                      ]
            ]
         }
     ],
    "output": "Number",
    "style": "sensor_blocks",
    "tooltip": "get imu sensor",
    "helpUrl": "https://github.com/oroca/OROCA-EduBot"
},
{
    "type": "edubot_sensor_accel",
    "message0": "%{BKY_LABEL_SENSOR_ACCELEROMETER}",
    "args0": [
         {
             "type": "field_dropdown",
             "name": "AXIS",
             "options": [
                     [
                      "x",
                      "X"
                      ],
                     [
                      "y",
                      "Y"
                      ],
                     [
                      "z",
                      "Z"
                      ]
            ]
         }
     ],
    "output": "Number",
    "style": "sensor_blocks",
    "tooltip": "get accelerometer sensor",
    "helpUrl": "https://github.com/oroca/OROCA-EduBot"
},
{
    "type": "edubot_sensor_gyro",
    "message0": "%{BKY_LABEL_SENSOR_GYRO_SENSOR}",
    "args0": [
         {
             "type": "field_dropdown",
             "name": "AXIS",
             "options": [
                     [
                      "x",
                      "X"
                      ],
                     [
                      "y",
                      "Y"
                      ],
                     [
                      "z",
                      "Z"
                      ]
            ]
         }
     ],
    "output": "Number",
    "style": "sensor_blocks",
    "tooltip": "get gyro sensor",
    "helpUrl": "https://github.com/oroca/OROCA-EduBot"
},

]);