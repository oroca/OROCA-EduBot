'use strict';

goog.require('Blockly.Blocks');
goog.require('Blockly');

Blockly.defineBlocksWithJsonArray([
{
    "type": "edubot_motor_set_linear_vel",
    "message0": "🚗 Move %1 with Velocity %2 (%)",
    "args0": [
                {
                    "type": "field_dropdown",
                    "name": "DIRECTION",
                    "options": [
                        [
                            "Forward",
                            "FORWARD"
                        ],
                        [
                            "Backward",
                            "BACKWARD"
                        ]
                    ]
                },
                {
                    "type": "input_value",
                    "name": "MAX_SPEED",
                    "check": "Number"
                }
    ],
    "previousStatement": null,
    "nextStatement": null,
    "colour": "200",
    "tooltip": "Move Robot with Speed %1",
    "helpUrl": "https://github.com/oroca/OROCA-EduBot"
},
{
    "type": "edubot_motor_to_linear_pos",
    "message0": "🚗 Move %1 to %2 (cm)",
    "args0": [
                {
                    "type": "field_dropdown",
                    "name": "DIRECTION",
                    "options": [
                        [
                            "Forward",
                            "FORWARD"
                        ],
                        [
                            "Backward",
                            "BACKWARD"
                        ]
                    ]
                },
                {
                    "type": "input_value",
                    "name": "TARGET_DISTANCE",
                    "check": "Number"
                }
    ],
    "previousStatement": null,
    "nextStatement": null,
    "colour": "200",
    "tooltip": "Move Robot to %1",
    "helpUrl": "https://github.com/oroca/OROCA-EduBot"
},
{
    "type": "edubot_motor_set_angular_vel",
    "message0": "🚗 Turn %1 with Velocity %2 (%)",
    "args0": [
                {
                    "type": "field_dropdown",
                    "name": "DIRECTION",
                    "options": [
                        [
                            "CW",
                            "CW"
                        ],
                        [
                            "CCW",
                            "CCW"
                        ]
                    ]
                },
                {
                    "type": "input_value",
                    "name": "MAX_SPEED",
                    "check": "Number"
                }
    ],
    "previousStatement": null,
    "nextStatement": null,
    "colour": "200",
    "tooltip": "Turn Robot with %1",
    "helpUrl": "https://github.com/oroca/OROCA-EduBot"
},
{
    "type": "edubot_motor_to_angular_pos",
    "message0": "🚗 Turn %1 to %2 (deg)",
    "args0": [
                {
                    "type": "field_dropdown",
                    "name": "DIRECTION",
                    "options": [
                        [
                            "CW",
                            "CW"
                        ],
                        [
                            "CCW",
                            "CCW"
                        ]
                    ]
                },
                {
                    "type": "input_value",
                    "name": "TARGET_DEG",
                    "check": "Number"
                }
    ],
    "previousStatement": null,
    "nextStatement": null,
    "colour": "200",
    "tooltip": "Turn Robot to %1",
    "helpUrl": "https://github.com/oroca/OROCA-EduBot"
},
{
    "type": "edubot_motor_stop",
    "message0": "🚫 Stop",
    "args0": [
    ],
    "previousStatement": null,
    "nextStatement": null,
    "colour": "200",
    "tooltip": "Stop robot",
    "helpUrl": "https://github.com/oroca/OROCA-EduBot"
},
]);