'use strict';

goog.require('Blockly.Blocks');
goog.require('Blockly');

Blockly.defineBlocksWithJsonArray([
{
    "type": "edubot_is_moving",
    "message0": "is moving?",
    "output": "Boolean",
    "style": "move_blocks",
    "tooltip": "is moving?",
    "helpUrl": "https://github.com/oroca/OROCA-EduBot"
},
{
    "type": "edubot_set_max_velocity",
    "message0": "set max. velocity to %1",
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
    "message0": "set acceleration to %1",
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
    "message0": "move by step left %1 and right %2 ",
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
    "message0": "move by velocity left %1 and right %2 ",
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
    "message0": "move by distance left %1 and right %2 ",
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
    "message0": "move forward",
    "previousStatement": null,
    "nextStatement": null,
    "inputsInline": true,
    "style": "move_blocks",
    "tooltip": "move forward",
    "helpUrl": "https://github.com/oroca/OROCA-EduBot"
},
{
    "type": "edubot_motor_move_backward",
    "message0": "move backward",
    "previousStatement": null,
    "nextStatement": null,
    "inputsInline": true,
    "style": "move_blocks",
    "tooltip": "move backward",
    "helpUrl": "https://github.com/oroca/OROCA-EduBot"
},
{
    "type": "edubot_motor_stop",
    "message0": "stop",
    "previousStatement": null,
    "nextStatement": null,
    "inputsInline": true,
    "style": "move_blocks",
    "tooltip": "stop",
    "helpUrl": "https://github.com/oroca/OROCA-EduBot"
},
{
    "type": "edubot_motor_turn_left",
    "message0": "turn left",
    "previousStatement": null,
    "nextStatement": null,
    "inputsInline": true,
    "style": "move_blocks",
    "tooltip": "turn left",
    "helpUrl": "https://github.com/oroca/OROCA-EduBot"
},
{
    "type": "edubot_motor_turn_right",
    "message0": "turn right",
    "previousStatement": null,
    "nextStatement": null,
    "inputsInline": true,
    "style": "move_blocks",
    "tooltip": "turn right",
    "helpUrl": "https://github.com/oroca/OROCA-EduBot"
},
{
    "type": "edubot_sleep",
    "message0": "sleep (ms) %1",
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
    "message0": "set colour left %1 and right %2",
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
    "message0": "turn off LEDs",
    "previousStatement": null,
    "nextStatement": null,
    "inputsInline": true,
    "style": "output_blocks",
    "tooltip": "turn off LEDs",
    "helpUrl": "https://github.com/oroca/OROCA-EduBot"
},
{
    "type": "edubot_misc_set_text",
    "message0": "set text %1",
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
    "message0": "set image %1",
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
    "message0": "clear display",
    "previousStatement": null,
    "nextStatement": null,
    "inputsInline": true,
    "style": "output_blocks",
    "tooltip": "clear display",
    "helpUrl": "https://github.com/oroca/OROCA-EduBot"
},
{
    "type": "edubot_misc_button_state",
    "message0": "get button state",
    "output": "Number",
    "style": "input_blocks",
    "tooltip": "get button state",
    "helpUrl": "https://github.com/oroca/OROCA-EduBot"
},
{
    "type": "edubot_misc_is_button_pressed",
    "message0": "is button pressed?",
    "output": "Boolean",
    "style": "input_blocks",
    "tooltip": "is button pressed?",
    "helpUrl": "https://github.com/oroca/OROCA-EduBot"
},
{
    "type": "edubot_misc_get_batt_voltage",
    "message0": "battery level",
    "output": "Number",
    "style": "input_blocks",
    "tooltip": "get button state",
    "helpUrl": "https://github.com/oroca/OROCA-EduBot"
},
{
    "type": "edubot_misc_is_need_charging",
    "message0": "need charging?",
    "output": "Boolean",
    "style": "input_blocks",
    "tooltip": "need charging?",
    "helpUrl": "https://github.com/oroca/OROCA-EduBot"
},
{
    "type": "edubot_sensor_floor_sensor",
    "message0": "floor sensor %1",
    "args0": [
         {
             "type": "field_dropdown",
             "name": "DIRECTION",
             "options": [
                     [
                      "left_outer",
                      "LEFT_OUTER"
                      ],
                     [
                      "left_inner",
                      "LEFT_INNER"
                      ],
                     [
                      "right_inner",
                      "RIGHT_INNER"
                      ],
                      [
                      "right_outer",
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
    "message0": "distance sensor %1",
    "args0": [
         {
             "type": "field_dropdown",
             "name": "DIRECTION",
             "options": [
                     [
                      "left",
                      "LEFT"
                      ],
                     [
                      "right",
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
    "message0": "imu sensor %1",
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
    "message0": "accelerometer sensor %1",
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
    "message0": "gyro sensor %1",
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