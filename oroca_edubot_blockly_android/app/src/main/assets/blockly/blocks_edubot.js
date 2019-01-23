'use strict';

goog.require('Blockly.Blocks');
goog.require('Blockly');

Blockly.defineBlocksWithJsonArray([
{
    "type": "edubot_motor_set_linear_vel",
    "message0": "%{BKY_EDUBUT_MOVE_MSG_MOVE1}",
    "args0": [
                {
                    "type": "field_dropdown",
                    "name": "DIRECTION",
                    "options": [
                        [
                            "↑️ %{BKY_EDUBUT_MOVE_LABEL_FORWARD}",
                            "FORWARD"
                        ],
                        [
                            "↓️ %{BKY_EDUBUT_MOVE_LABEL_BACKWARD}",
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
    "colour": "%{BKY_EDUBOT_MOVE_HUE}",
    "tooltip": "Move Robot with Speed %1",
    "helpUrl": "https://github.com/oroca/OROCA-EduBot"
},
{
    "type": "edubot_motor_to_linear_pos",
    "message0": "%{BKY_EDUBUT_MOVE_MSG_MOVE2}",
    "args0": [
                {
                    "type": "field_dropdown",
                    "name": "DIRECTION",
                    "options": [
                        [
                            "↑️ %{BKY_EDUBUT_MOVE_LABEL_FORWARD}",
                            "FORWARD"
                        ],
                        [
                            "↓️ %{BKY_EDUBUT_MOVE_LABEL_BACKWARD}",
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
    "colour": "%{BKY_EDUBOT_MOVE_HUE}",
    "tooltip": "Move Robot to %1",
    "helpUrl": "https://github.com/oroca/OROCA-EduBot"
},
{
    "type": "edubot_motor_set_angular_vel",
    "message0": "%{BKY_EDUBUT_MOVE_MSG_TURN1}",
    "args0": [
                {
                    "type": "field_dropdown",
                    "name": "DIRECTION",
                    "options": [
                        [
                            "↷ %{BKY_EDUBUT_MOVE_LABEL_CW}",
                            "CW"
                        ],
                        [
                            "↶ %{BKY_EDUBUT_MOVE_LABEL_CCW}",
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
    "colour": "%{BKY_EDUBOT_MOVE_HUE}",
    "tooltip": "Turn Robot with %1",
    "helpUrl": "https://github.com/oroca/OROCA-EduBot"
},
{
    "type": "edubot_motor_to_angular_pos",
    "message0": "%{BKY_EDUBUT_MOVE_MSG_TURN2}",
    "args0": [
                {
                    "type": "field_dropdown",
                    "name": "DIRECTION",
                    "options": [
                        [
                            "↷ %{BKY_EDUBUT_MOVE_LABEL_CW}",
                            "CW"
                        ],
                        [
                            "↶ %{BKY_EDUBUT_MOVE_LABEL_CCW}",
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
    "colour": "%{BKY_EDUBOT_MOVE_HUE}",
    "tooltip": "Turn Robot to %1",
    "helpUrl": "https://github.com/oroca/OROCA-EduBot"
},
{
    "type": "edubot_motor_stop",
    "message0": "%{BKY_EDUBUT_MOVE_MSG_STOP}",
    "previousStatement": null,
    "nextStatement": null,
    "colour": "%{BKY_EDUBOT_MOVE_HUE}",
    "tooltip": "Stop robot",
    "helpUrl": "https://github.com/oroca/OROCA-EduBot"
},
{
    "type": "edubot_distance_sensor",
    "message0": "%{BKY_EDUBUT_SENSING_MSG_DISTANCE}",
    "output": "Number",
    "colour": "%{BKY_EDUBOT_SENSING_HUE}",
    "tooltip": "",
    "helpUrl": ""
},
{
    "type": "edubot_floor_sensor",
    "message0": "%{BKY_EDUBUT_SENSING_MSG_FLOOR}",
    "args0": [
                {
                    "type": "field_dropdown",
                    "name": "DIRECTION",
                    "options": [
                        [
                            "◀︎ %{BKY_EDUBUT_MOVE_LABEL_LEFT}",
                            "LEFT"
                        ],
                        [
                            "● %{BKY_EDUBUT_MOVE_LABEL_CENTER}",
                            "CENTER"
                        ],
                        [
                            "▶︎ %{BKY_EDUBUT_MOVE_LABEL_RIGHT}",
                            "RIGHT"
                        ]
                    ]
                }
    ],
    "output": "Number",
    "colour": "%{BKY_EDUBOT_SENSING_HUE}",
    "tooltip": "",
    "helpUrl": ""
},
]);