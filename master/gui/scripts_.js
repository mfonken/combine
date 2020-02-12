// /* When the user clicks on the button,
// toggle between hiding and showing the dropdown content */
var version = "v0.1";

var example_profile = {"Families":{"type":"list","Families_0":{"IDLE":{"0":"SYSTEM_FAMILY_0"},"num_IDLE":1,"ALL":{"0":"SYSTEM_FAMILY_D","1":"SYSTEM_FAMILY_C","2":"SYSTEM_FAMILY_B","3":"SYSTEM_FAMILY_A","4":"SYSTEM_FAMILY_0"},"num_ALL":5,"ERROR":{"0":"SYSTEM_FAMILY_C","1":"SYSTEM_FAMILY_B","2":"SYSTEM_FAMILY_0"},"num_ERROR":3}},"Components":{"type":"group","Components_0":{"NAME":"EFR32_BLE","ID":"APPLICATION_COMPONENT_BLE_RADIO_PRIMARY","FAMILY":"SYSTEM_FAMILY_A","COMM":"COMM_BLE","ROUTE":"COMM_ROUTE_PRIMARY","ADDR":"COMM_ADDR_NONE","PORT":"COMM_PORT_NONE","PIN":"INTERNAL","STATE":"COMPONENT_STATE_OFF"},"Components_1":{"NAME":"EFR32_SUB","ID":"APPLICATION_COMPONENT_SUB_RADIO_PRIMARY","FAMILY":"SYSTEM_FAMILY_B","COMM":"COMM_SUB","ROUTE":"COMM_ROUTE_PRIMARY","ADDR":"COMM_ADDR_NONE","PORT":"COMM_PORT_NONE","PIN":"INTERNAL","STATE":"COMPONENT_STATE_OFF"},"Components_2":{"NAME":"BNO080","ID":"APPLICATION_COMPONENT_MOTION_PRIMARY","FAMILY":"SYSTEM_FAMILY_B","COMM":"COMM_SPI","ROUTE":"COMM_ROUTE_PRIMARY","ADDR":"COMM_ADDR_NONE","PORT":"PORTC","PIN":"9","STATE":"COMPONENT_STATE_OFF"},"Components_3":{"NAME":"RHOMOD","ID":"APPLICATION_COMPONENT_RHO_MODULE_PRIMARY","FAMILY":"SYSTEM_FAMILY_0","COMM":"COMM_I2C","ROUTE":"COMM_ROUTE_PRIMARY","ADDR":"0xee","PORT":"PORTA","PIN":"2","STATE":"COMPONENT_STATE_INTERRUPT"},"Components_4":{"NAME":"STC310","ID":"APPLICATION_COMPONENT_BATTERY_MONITOR_PRIMARY","FAMILY":"SYSTEM_FAMILY_C","COMM":"COMM_I2C","ROUTE":"COMM_ROUTE_PRIMARY","ADDR":"0x00","PORT":"PORT0","PIN":"0","STATE":"COMPONENT_STATE_INTERRUPT"},"Components_5":{"NAME":"XC9265","ID":"APPLICATION_COMPONENT_REGULATOR_1V5","FAMILY":"SYSTEM_FAMILY_A","COMM":"COMM_NONE","ROUTE":"COMM_ROUTE_NONE","ADDR":"COMM_ADDR_NONE","PORT":"COMM_PORT_NONE","PIN":"INTERNAL","STATE":"COMPONENT_STATE_OFF"}},"Subactivities":{"type":"struct","Subactivities_0":{"ID":"APPLICATION_SCHEDULER_ID_MOTION_INTERRUPT","data":"INTERRUPT_ACTION_IMMEDIATE","component_id":"APPLICATION_COMPONENT_MOTION_PRIMARY","handler_id":"APPLICATION_SUBACTIVITY_HANDLE_MOTION_EVENT"},"Subactivities_1":{"ID":"APPLICATION_SCHEDULER_ID_TOUCH_INTERRUPT","data":"0","component_id":"APPLICATION_COMPONENT_TOUCH_PRIMARY","handler_id":"APPLICATION_SUBACTIVITY_HANDLE_TOUCH_EVENT"},"Subactivities_2":{"ID":"APPLICATION_SCHEDULER_ID_BATTERY_MONITOR_POLL","data":"1","component_id":"APPLICATION_COMPONENT_BATTERY_MONITOR_PRIMARY","handler_id":"APPLICATION_SUBACTIVITY_POLL_BATTERY_MONITOR"},"Subactivities_3":{"ID":"APPLICATION_SCHEDULER_ID_RHO_INTERRUPT","data":"INTERRUPT_ACTION_QUEUE","component_id":"APPLICATION_COMPONENT_RHO_MODULE_PRIMARY","handler_id":"APPLICATION_SUBACTIVITY_HANDLE_RHO_EVENT"},"Subactivities_4":{"ID":"APPLICATION_INTERRUPTER_ID_TAU_PACKET_TRANSMIT","data":"0","component_id":"APPLICATION_COMPONENT_BLE_RADIO_PRIMARY","handler_id":"APPLICATION_SUBACTIVITY_TRANSMIT_HOST_PACKET"},"Subactivities_5":{"ID":"APPLICATION_INTERRUPTER_ID_TAU_PACKET_RECEIVE","data":"INTERRUPT_ACTION_QUEUE","component_id":"APPLICATION_COMPONENT_SUB_RADIO_PRIMARY","handler_id":"APPLICATION_SUBACTIVITY_RECEIVE_HOST_PACKET"}},"Tasks":{"type":"struct","Tasks_0":{"ID":"APPLICATION_TASK_SHELF_ENTRY_ID_NULL_TASKS","interrupts":"","num_interrupts":0,"scheduled":"","num_scheduled":0},"Tasks_1":{"ID":"APPLICATION_TASK_SHELF_ENTRY_ID_GLOBAL_TASKS","interrupts":{"0":"APPLICATION_SCHEDULER_ID_MOTION_INTERRUPT"},"num_interrupts":1,"scheduled":"","num_scheduled":0},"Tasks_2":{"ID":"APPLICATION_TASK_SHELF_ENTRY_ID_SENSOR_MOTION_TASKS","interrupts":{"0":"APPLICATION_SCHEDULER_ID_MOTION_INTERRUPT"},"num_interrupts":1,"scheduled":"","num_scheduled":0},"Tasks_3":{"ID":"APPLICATION_TASK_SHELF_ENTRY_ID_SENSOR_BATTERY_MONITOR_TASKS","interrupts":"","num_interrupts":0,"scheduled":{"0":"APPLICATION_PROBE_ID_BATTERY_MONITOR"},"num_scheduled":1},"Tasks_4":{"ID":"APPLICATION_TASK_SHELF_ENTRY_ID_SENSOR_RHO_TASKS","interrupts":"","num_interrupts":0,"scheduled":{"0":"APPLICATION_SCHEDULER_ID_RHO_INTERRUPT"},"num_scheduled":1},"Tasks_5":{"ID":"APPLICATION_TASK_SHELF_ENTRY_ID_COMMUNICATION_HOST_RADIO_TASKS","interrupts":{"0":"APPLICATION_INTERRUPTER_ID_TAU_PACKET_RECEIVE"},"num_interrupts":1,"scheduled":{"0":"APPLICATION_INTERRUPTER_ID_TAU_PACKET_TRANSMIT"},"num_scheduled":1}},"States":{"type":"nested","States_0":{"1":{"ACTIVITY":"SYSTEM_ACTIVITY_STARTUP","SUBACTIVITIES":{"0":"APPLICATION_SUBACTIVITY_SELF_CHECK","1":"APPLICATION_SUBACTIVITY_INIT_COMMUNICATION","2":"APPLICATION_SUBACTIVITY_INIT_COMPONENTS","3":"APPLICATION_SUBACTIVITY_INIT_TAU_CLIENT","4":"APPLICATION_SUBACTIVITY_INIT_CONFIRM"},"num_SUBACTIVITIES":5,"EXIT_STATE":"SYSTEM_STATE_IDLE"},"STATE":"SYSTEM_STATE_STARTUP","FAMILIES":"Families_ALL","TASKS":{"0":"APPLICATION_TASK_SHELF_ENTRY_ID_GLOBAL_TASKS"},"num_TASKS":1},"States_1":{"1":{"ACTIVITY":"SYSTEM_ACTIVITY_NONE","SUBACTIVITIES":"","num_SUBACTIVITIES":0,"EXIT_STATE":"SYSTEM_STATE_IDLE"},"STATE":"SYSTEM_STATE_IDLE","FAMILIES":"Families_IDLE","TASKS":{"0":"APPLICATION_TASK_SHELF_ENTRY_ID_NULL_TASKS"},"num_TASKS":1},"States_2":{"1":{"ACTIVITY":"SYSTEM_ACTIVITY_NONE","SUBACTIVITIES":"","num_SUBACTIVITIES":0,"EXIT_STATE":"SYSTEM_STATE_IDLE"},"STATE":"SYSTEM_STATE_WAITING","FAMILIES":"Families_IDLE","TASKS":{"0":"APPLICATION_TASK_SHELF_ENTRY_ID_NULL_TASKS"},"num_TASKS":1},"States_3":{"1":{"ACTIVITY":"SYSTEM_ACTIVITY_NONE","SUBACTIVITIES":"","num_SUBACTIVITIES":0,"EXIT_STATE":"SYSTEM_STATE_ACTIVE"},"STATE":"SYSTEM_STATE_ACTIVE","FAMILIES":"Families_ALL","TASKS":{"0":"APPLICATION_TASK_SHELF_ENTRY_ID_SENSOR_TOUCH_TASKS","1":"APPLICATION_TASK_SHELF_ENTRY_ID_SENSOR_MOTION_TASKS"},"num_TASKS":2},"States_4":{"1":{"ACTIVITY":"SYSTEM_ACTIVITY_NONE","SUBACTIVITIES":"","num_SUBACTIVITIES":0,"EXIT_STATE":"SYSTEM_STATE_IDLE"},"STATE":"SYSTEM_STATE_ASLEEP","FAMILIES":"Families_IDLE","TASKS":{"0":"APPLICATION_TASK_SHELF_ENTRY_ID_NULL_TASKS"},"num_TASKS":1},"States_5":{"1":{"ACTIVITY":"SYSTEM_ACTIVITY_NONE","SUBACTIVITIES":"","num_SUBACTIVITIES":0,"EXIT_STATE":"SYSTEM_STATE_RECOVERY"},"STATE":"SYSTEM_STATE_ERROR","FAMILIES":"Families_ERROR","TASKS":{"0":"APPLICATION_TASK_SHELF_ENTRY_ID_NULL_TASKS"},"num_TASKS":1},"States_6":{"1":{"ACTIVITY":"SYSTEM_ACTIVITY_NONE","SUBACTIVITIES":"","num_SUBACTIVITIES":0,"EXIT_STATE":"SYSTEM_STATE_RECOVERY"},"STATE":"SYSTEM_STATE_RECOVERY","FAMILIES":"Families_ERROR","TASKS":{"0":"APPLICATION_TASK_SHELF_ENTRY_ID_NULL_TASKS"},"num_TASKS":1},"States_7":{"1":{"ACTIVITY":"SYSTEM_ACTIVITY_NONE","SUBACTIVITIES":"","num_SUBACTIVITIES":0,"EXIT_STATE":"SYSTEM_STATE_ERROR"},"STATE":"SYSTEM_STATE_UNKNOWN","FAMILIES":"Families_ERROR","TASKS":{"0":"APPLICATION_TASK_SHELF_ENTRY_ID_NULL_TASKS"},"num_TASKS":1}}}
// {"Families":{"type":"list","Families_0":{"IDLE":{"0":"SYSTEM_FAMILY_0"},"num_IDLE":1,"ALL":{"0":"SYSTEM_FAMILY_A"},"num_ALL":1,"ERROR":{"0":"SYSTEM_FAMILY_B"},"num_ERROR":1}},"Components":{"type":"group","Components_0":{"NAME":"ABC","ID":"APPLICATION_COMPONENT_MOTION_PRIMARY","FAMILY":"SYSTEM_FAMILY_0","COMM":"COMM_I2C","ROUTE":"COMM_ROUTE_PRIMARY","ADDR":"0xab","PORT":"PORTA","PIN":"0","STATE":"COMPONENT_STATE_ON"}},"Subactivities":{"type":"struct","Subactivities_0":{"ID":"APPLICATION_COMBINE_GLOBAL","data":"INTERRUPT_ACTION_IGNORE","component_id":"APPLICATION_COMPONENT_MOTION_PRIMARY","handler_id":"APPLICATION_SUBACTIVITY_WAIT_FOR_WAKE"}},"Tasks":{"type":"struct","Tasks_0":{"ID":"APPLICATION_TASK_SHELF_ENTRY_ID_GLOBAL_TASKS","interrupts":{"0":"APPLICATION_COMBINE_GLOBAL"},"num_interrupts":1,"scheduled":{"0":"APPLICATION_PROBE_ID_HOST"},"num_scheduled":1}},"States":{"type":"nested","States_0":{"1":{"ACTIVITY":"SYSTEM_ACTIVITY_STARTUP","SUBACTIVITIES":{"0":"APPLICATION_SUBACTIVITY_SELF_CHECK"},"num_SUBACTIVITIES":1,"EXIT_STATE":"SYSTEM_STATE_IDLE"},"STATE":"SYSTEM_STATE_STARTUP","FAMILIES":"Families_ALL","TASKS":{"0":"APPLICATION_TASK_SHELF_ENTRY_ID_GLOBAL_TASKS"},"num_TASKS":1}}}
// {"Families":{"type":"list","Families_0":{"IDLE":{"0":"SYSTEM_FAMILY_0"},"num_IDLE":1,"ALL":{"0":"SYSTEM_FAMILY_A"},"num_ALL":1,"ERROR":{"0":"SYSTEM_FAMILY_B"},"num_ERROR":1},"Families_1":{"IDLE":{"0":"SYSTEM_FAMILY_B"},"num_IDLE":1,"ALL":{"0":"SYSTEM_FAMILY_C"},"num_ALL":1,"ERROR":{"0":"SYSTEM_FAMILY_D"},"num_ERROR":1}},"Components":{"type":"group","Components_0":{"NAME":"ABC","ID":"APPLICATION_COMPONENT_MOTION_PRIMARY","FAMILY":"SYSTEM_FAMILY_0","COMM":"COMM_I2C","ROUTE":"COMM_ROUTE_PRIMARY","ADDR":"0xab","PORT":"PORTA","PIN":"0","STATE":"COMPONENT_STATE_ON"}},"Subactivities":{"type":"struct","Subactivities_0":{"ID":"APPLICATION_COMBINE_GLOBAL","data":"INTERRUPT_ACTION_IGNORE","component_id":"APPLICATION_COMPONENT_MOTION_PRIMARY","handler_id":"APPLICATION_SUBACTIVITY_WAIT_FOR_WAKE"}},"Tasks":{"type":"struct","Tasks_0":{"ID":"APPLICATION_TASK_SHELF_ENTRY_ID_GLOBAL_TASKS","interrupts":{"0":"APPLICATION_COMBINE_GLOBAL"},"num_interrupts":1,"scheduled":{"0":"APPLICATION_PROBE_ID_HOST"},"num_scheduled":1}},"States":{"type":"nested","States_0":{"1":{"ACTIVITY":"SYSTEM_ACTIVITY_STARTUP","SUBACTIVITIES":{"0":"APPLICATION_SUBACTIVITY_SELF_CHECK"},"num_SUBACTIVITIES":1,"EXIT_STATE":"SYSTEM_STATE_IDLE"},"STATE":"SYSTEM_STATE_STARTUP","FAMILIES":"Families_ALL","TASKS":{"0":"APPLICATION_TASK_SHELF_ENTRY_ID_GLOBAL_TASKS"},"num_TASKS":1}}}
//{"Families":{"type":"list","Families_0":{"IDLE":{"0":"SYSTEM_FAMILY_0"},"num_IDLE":1,"ALL":{"0":"SYSTEM_FAMILY_A"},"num_ALL":1,"ERROR":{"0":"SYSTEM_FAMILY_B"},"num_ERROR":1}},"Components":{"type":"group","Components_0":{"NAME":"ABC","ID":"APPLICATION_COMPONENT_MOTION_PRIMARY","FAMILY":"SYSTEM_FAMILY_0","COMM":"COMM_I2C","ROUTE":"COMM_ROUTE_PRIMARY","ADDR":"0xab","PORT":"PORTA","PIN":"0","STATE":"COMPONENT_STATE_ON"},"Components_1":{"NAME":"DEF","ID":"APPLICATION_COMPONENT_TIP_PRIMARY","FAMILY":"SYSTEM_FAMILY_A","COMM":"COMM_SPI","ROUTE":"COMM_ROUTE_PRIMARY","ADDR":"0xde","PORT":"PORTB","PIN":"1","STATE":"COMPONENT_STATE_INTERRUPT"},"Components_2":{"NAME":"GHI","ID":"APPLICATION_COMPONENT_BLE_RADIO_PRIMARY","FAMILY":"SYSTEM_FAMILY_C","COMM":"COMM_UART","ROUTE":"COMM_ROUTE_SECONDARY","ADDR":"0x12","PORT":"PORTC","PIN":"2","STATE":"COMPONENT_STATE_OFF"}},"Subactivities":{"type":"struct","Subactivities_0":{"ID":"APPLICATION_COMBINE_GLOBAL","data":"INTERRUPT_ACTION_IGNORE","component_id":"APPLICATION_COMPONENT_MOTION_PRIMARY","handler_id":"APPLICATION_SUBACTIVITY_WAIT_FOR_WAKE"}},"Tasks":{"type":"struct","Tasks_0":{"ID":"APPLICATION_TASK_SHELF_ENTRY_ID_GLOBAL_TASKS","interrupts":{"0":"APPLICATION_COMBINE_GLOBAL"},"num_interrupts":1,"scheduled":{"0":"APPLICATION_PROBE_ID_HOST"},"num_scheduled":1}},"States":{"type":"nested","States_0":{"1":{"ACTIVITY":"SYSTEM_ACTIVITY_STARTUP","SUBACTIVITIES":{"0":"APPLICATION_SUBACTIVITY_SELF_CHECK"},"num_SUBACTIVITIES":1,"EXIT_STATE":"SYSTEM_STATE_IDLE"},"STATE":"SYSTEM_STATE_STARTUP","FAMILIES":"Families_ALL","TASKS":{"0":"APPLICATION_TASK_SHELF_ENTRY_ID_GLOBAL_TASKS"},"num_TASKS":1}}};
//{"Families":{"type":"list","Families_0":{"IDLE":{"0":"SYSTEM_FAMILY_0"},"num_IDLE":1,"ALL":{"0":"SYSTEM_FAMILY_0","1":"SYSTEM_FAMILY_A","2":"SYSTEM_FAMILY_B","3":"SYSTEM_FAMILY_C","4":"SYSTEM_FAMILY_D"},"num_ALL":5,"ERROR":{"0":"SYSTEM_FAMILY_0","1":"SYSTEM_FAMILY_B","2":"SYSTEM_FAMILY_C"},"num_ERROR":3}},"Components":{"type":"group","Components_0":{"NAME":"EFR32_BLE","ID":"APPLICATION_COMPONENT_BLE_RADIO_PRIMARY","FAMILY":"SYSTEM_FAMILY_A","COMM":"COMM_BLE","ROUTE":"COMM_ROUTE_PRIMARY","ADDR":"COMM_ADDR_NONE","PORT":"COMM_PORT_NONE","PIN":"INTERNAL","STATE":"COMPONENT_STATE_OFF"},"Components_1":{"NAME":"BNO080","ID":"APPLICATION_COMPONENT_MOTION_PRIMARY","FAMILY":"SYSTEM_FAMILY_B","COMM":"COMM_SPI","ROUTE":"COMM_ROUTE_PRIMARY","ADDR":"COMM_ADDR_NONE","PORT":"PORTC","PIN":"9","STATE":"COMPONENT_STATE_OFF"}},"Subactivities":{"type":"struct","Subactivities_0":{"ID":"APPLICATION_SCHEDULER_ID_MOTION_INTERRUPT","data":"INTERRUPT_ACTION_IMMEDIATE","component_id":"APPLICATION_COMPONENT_MOTION_PRIMARY","handler_id":"APPLICATION_SUBACTIVITY_HANDLE_MOTION_EVENT"}},"Tasks":{"type":"struct","Tasks_0":{"ID":"APPLICATION_TASK_SHELF_ENTRY_ID_NULL_TASKS","interrupts":"","num_interrupts":0,"scheduled":"","num_scheduled":0},"Tasks_1":{"ID":"APPLICATION_TASK_SHELF_ENTRY_ID_GLOBAL_TASKS","interrupts":{"0":"APPLICATION_SCHEDULER_ID_MOTION_INTERRUPT"},"num_interrupts":1,"scheduled":"","num_scheduled":0},"Tasks_2":{"ID":"APPLICATION_TASK_SHELF_ENTRY_ID_SENSOR_MOTION_TASKS","interrupts":{"0":"APPLICATION_SCHEDULER_ID_MOTION_INTERRUPT"},"num_interrupts":1,"scheduled":"","num_scheduled":0}},"States":{"type":"nested","States_0":{"1":{"ACTIVITY":"SYSTEM_ACTIVITY_STARTUP","SUBACTIVITIES":{"0":"APPLICATION_SUBACTIVITY_INIT_CONFIRM","1":"APPLICATION_SUBACTIVITY_INIT_TAU_CLIENT","2":"APPLICATION_SUBACTIVITY_INIT_COMPONENTS","3":"APPLICATION_SUBACTIVITY_INIT_COMMUNICATION","4":"APPLICATION_SUBACTIVITY_SELF_CHECK"},"num_SUBACTIVITIES":5,"EXIT_STATE":"SYSTEM_STATE_IDLE"},"STATE":"SYSTEM_STATE_STARTUP","FAMILIES":"Families_ALL","TASKS":{"0":"APPLICATION_TASK_SHELF_ENTRY_ID_GLOBAL_TASKS"},"num_TASKS":1},"States_1":{"1":{"ACTIVITY":"SYSTEM_ACTIVITY_NONE","SUBACTIVITIES":"","num_SUBACTIVITIES":0,"EXIT_STATE":"SYSTEM_STATE_IDLE"},"STATE":"SYSTEM_STATE_IDLE","FAMILIES":"Families_IDLE","TASKS":{"0":"APPLICATION_TASK_SHELF_ENTRY_ID_NULL_TASKS"},"num_TASKS":1}}}
var all_options =
{
    "Families":[{
        "options":"list",
        "IDLE":[
            "array:.Components.FAMILY"
        ],
        "ALL":[
            "array:.Components.FAMILY"
        ],
        "ERROR":[
            "array:.Components.FAMILY"
        ]
    }],
    "Components":[{
        "options":"group:~",
        "NAME":[
            "enter name"
        ],
        "ID":[
            "APPLICATION_COMPONENT_NONE",
            "APPLICATION_COMPONENT_MOTION_PRIMARY",
            "APPLICATION_COMPONENT_RHO_MODULE_PRIMARY",
            "APPLICATION_COMPONENT_TOUCH_PRIMARY",
            "APPLICATION_COMPONENT_TIP_PRIMARY",
            "APPLICATION_COMPONENT_BATTERY_MONITOR_PRIMARY",
            "APPLICATION_COMPONENT_BLE_RADIO_PRIMARY",
            "APPLICATION_COMPONENT_SUB_RADIO_PRIMARY",
            "APPLICATION_COMPONENT_HAPTIC_PRIMARY",
            "APPLICATION_COMPONENT_REGULATOR_1V5"
        ],
        "FAMILY":[
            "SYSTEM_FAMILY_NONE",
            "SYSTEM_FAMILY_0",
            "SYSTEM_FAMILY_A",
            "SYSTEM_FAMILY_B",
            "SYSTEM_FAMILY_C",
            "SYSTEM_FAMILY_D"
        ],
        "COMM":[
            "COMM_NONE",
            "COMM_I2C",
            "COMM_SPI",
            "COMM_UART",
            "COMM_BLE",
            "COMM_SUB"
        ],
        "ROUTE":[
            "COMM_ROUTE_NONE",
            "COMM_ROUTE_PRIMARY",
            "COMM_ROUTE_SECONDARY"
        ],
        "ADDR":[
            "enter address",
            "COMM_ADDR_NONE"
        ],
        "PORT":[
            "PORT0",
            "PORTA",
            "PORTB",
            "PORTC",
            "PORTD",
            "PORTF",
            "COMM_PORT_NONE"
        ],
        "PIN":[
            "enter pin number",
            "INTERNAL"
        ],
        "STATE":[
            "COMPONENT_STATE_OFF",
            "COMPONENT_STATE_ON",
            "COMPONENT_STATE_Z",
            "COMPONENT_STATE_INTERRUPT"
        ]
    }],
    "Subactivities:struct":[{
        "ID":[
            "APPLICATION_ACTION_ID_NONE",
            "APPLICATION_COMBINE_GLOBAL",

            "APPLICATION_PROBE_ID_HOST",
            "APPLICATION_PROBE_ID_RHO",
            "APPLICATION_PROBE_ID_TIP",
            "APPLICATION_PROBE_ID_BATTERY_MONITOR",

            "APPLICATION_INTERRUPTER_ID_TAU_PACKET_TRANSMIT",
            "APPLICATION_INTERRUPTER_ID_TAU_PACKET_RECEIVE",
            "APPLICATION_INTERRUPTER_ID_SUB_RADIO_PACKET_TRANSMIT",
            "APPLICATION_INTERRUPTER_ID_HAPTIC_TRIGGER",

            "APPLICATION_SCHEDULER_ID_TAU_PERFORM",
            "APPLICATION_SCHEDULER_ID_TAU_PACKET_QUEUE",
            "APPLICATION_SCHEDULER_ID_TIP_POLL",
            "APPLICATION_SCHEDULER_ID_BATTERY_MONITOR_POLL",

            "APPLICATION_SCHEDULER_ID_MOTION_INTERRUPT",
            "APPLICATION_SCHEDULER_ID_RHO_INTERRUPT",
            "APPLICATION_SCHEDULER_ID_TOUCH_INTERRUPT"
        ],
        "data":[
            "enter info",
            "INTERRUPT_ACTION_IGNORE",
            "INTERRUPT_ACTION_QUEUE",
            "INTERRUPT_ACTION_IMMEDIATE"
        ],
        "component_id":[
            "APPLICATION_COMPONENT_NONE",
            "APPLICATION_COMPONENT_MOTION_PRIMARY",
            "APPLICATION_COMPONENT_MOTION_SECONDARY",
            "APPLICATION_COMPONENT_RHO_MODULE_PRIMARY",
            "APPLICATION_COMPONENT_RHO_MODULE_SECONDARY",
            "APPLICATION_COMPONENT_TOUCH_PRIMARY",
            "APPLICATION_COMPONENT_TOUCH_SECONDARY",
            "APPLICATION_COMPONENT_TIP_PRIMARY",
            "APPLICATION_COMPONENT_TIP_SECONDARY",
            "APPLICATION_COMPONENT_BATTERY_MONITOR_PRIMARY",
            "APPLICATION_COMPONENT_BLE_RADIO_PRIMARY",
            "APPLICATION_COMPONENT_SUB_RADIO_PRIMARY",
            "APPLICATION_COMPONENT_HAPTIC_PRIMARY",
            "APPLICATION_COMPONENT_HAPTIC_SECONDARY",
            "APPLICATION_COMPONENT_REGULATOR_1V5"
        ],
        "handler_id":[
            "APPLICATION_SUBACTIVITY_NONE",
            "APPLICATION_SUBACTIVITY_SELF_CHECK",
            "APPLICATION_SUBACTIVITY_WAIT_FOR_WAKE",

            /* Initialization */
            "APPLICATION_SUBACTIVITY_INIT_COMMUNICATION",
            "APPLICATION_SUBACTIVITY_INIT_SUB_VREG",
            "APPLICATION_SUBACTIVITY_INIT_COMPONENTS",
            "APPLICATION_SUBACTIVITY_INIT_TAU_CLIENT",
            "APPLICATION_SUBACTIVITY_INIT_RHO_CLIENT",
            "APPLICATION_SUBACTIVITY_INIT_CONFIRM",
            "APPLICATION_SUBACTIVITY_BATTERY_MONITOR_ACTIVE",

            /* Profile */
            //    "APPLICATION_SUBACTIVITY_PROFILE_FETCH",
            //    "APPLICATION_SUBACTIVITY_PROFILE_PERFORM",
            //    "APPLICATION_SUBACTIVITY_PROFILE_UPDATE",
            //    "APPLICATION_SUBACTIVITY_PROFILE_STORE",

            /* Probes */
            "APPLICATION_SUBACTIVITY_SEND_HOST_PROBE",
            "APPLICATION_SUBACTIVITY_RECEIVE_HOST_PROBE",

            /* Handles */
            "APPLICATION_SUBACTIVITY_HANDLE_MOTION_EVENT",
            "APPLICATION_SUBACTIVITY_HANDLE_RHO_EVENT",
            "APPLICATION_SUBACTIVITY_HANDLE_TOUCH_EVENT",

            "APPLICATION_SUBACTIVITY_POLL_BATTERY_MONITOR",
            "APPLICATION_SUBACTIVITY_POLL_TIP",

            "APPLICATION_SUBACTIVITY_TRIGGER_HAPTIC",

            "APPLICATION_SUBACTIVITY_TRANSMIT_HOST_PACKET",
            "APPLICATION_SUBACTIVITY_RECEIVE_HOST_PACKET",
            "APPLICATION_SUBACTIVITY_TRANSMIT_SUB_RADIO_PACKET",
            "APPLICATION_SUBACTIVITY_RECEIVE_SUB_RADIO_PACKET",

            /* Tau */
            "APPLICATION_SUBACTIVITY_TAU_STANDARD_RHO_START",
            "APPLICATION_SUBACTIVITY_TAU_STANDARD_MOTION_START",
            "APPLICATION_SUBACTIVITY_TAU_STANDARD_START",

            /* Sleep/Idle */
            "APPLICATION_SUBACTIVITY_TAU_STANDARD_RHO_STOP",
            "APPLICATION_SUBACTIVITY_TAU_STANDARD_MOTION_STOP",
            "APPLICATION_SUBACTIVITY_TAU_STOP",
            "APPLICATION_SUBACTIVITY_BATTERY_MONITOR_SLEEP",
        ]
    }],
    "Tasks:struct:~":[{
        "ID":[
            "APPLICATION_TASK_SHELF_ENTRY_ID_NULL_TASKS",
            "APPLICATION_TASK_SHELF_ENTRY_ID_GLOBAL_TASKS",
            "APPLICATION_TASK_SHELF_ENTRY_ID_SENSOR_MOTION_TASKS",
            "APPLICATION_TASK_SHELF_ENTRY_ID_SENSOR_TOUCH_TASKS",
            "APPLICATION_TASK_SHELF_ENTRY_ID_SENSOR_TIP_TASKS",
            "APPLICATION_TASK_SHELF_ENTRY_ID_SENSOR_BATTERY_MONITOR_TASKS",
            "APPLICATION_TASK_SHELF_ENTRY_ID_SENSOR_RHO_TASKS",
            "APPLICATION_TASK_SHELF_ENTRY_ID_DRIVER_HAPTIC_PRIMARY_TASKS",
            "APPLICATION_TASK_SHELF_ENTRY_ID_COMMUNICATION_HOST_RADIO_TASKS",
            "APPLICATION_TASK_SHELF_ENTRY_ID_COMMUNICATION_SUB_RADIO_TASKS"
        ],
        "interrupts":[
            "array:.Subactivities:struct.ID"
        ],
        "scheduled":[
            "array:.Subactivities:struct.ID"
        ]
    }],
    "States:nested":[{
        "STATE":[
            "SYSTEM_STATE_STARTUP",
            "SYSTEM_STATE_IDLE",
            "SYSTEM_STATE_WAITING",
            "SYSTEM_STATE_ACTIVE",
            "SYSTEM_STATE_ASLEEP",
            "SYSTEM_STATE_ERROR",
            "SYSTEM_STATE_RECOVERY",
            "SYSTEM_STATE_UNKNOWN"
        ],
        "ACTIVITY:>":[
            "SYSTEM_ACTIVITY_NONE",
            "SYSTEM_ACTIVITY_STARTUP",
            "SYSTEM_ACTIVITY_IDLE",
            "SYSTEM_ACTIVITY_SLEEP",
        ],
        "SUBACTIVITIES:>":[
            "array:.Subactivities:struct.handler_id",
        ],
        "EXIT_STATE:>":[
            ":.States:nested.STATE"
        ],
        "FAMILIES":[
            ":.Families",
        ],
        "TASKS":[
            "array:.Tasks:struct:~.ID"
        ]
    }]
};

var num_entries_per_category = {};

function createMain(index) {
    $.each(all_options, function (global_key, global_list) {
        num_entries_per_category[global_key] = 0;
        var key_items = global_key.split(":");
        var name_key = key_items[0];
        var global_type = "global_type_";
        // if(key_items.length > 1) {
        if(global_list[0]['options'] != undefined) {
            // console.log(global_list[0]['options']);
            global_type += global_list[0]['options'].split(":");
        }
        else {
            global_type += "none";
        }
        var list_div_id = name_key + "_div";
        $(".main_div").append('<div class="generated_div" id=\"' + list_div_id + '\"></div>');
        $("#" + list_div_id).prepend('<h2><a class="generated_div_title">' + name_key + '</a></h2>');
        $("#" + list_div_id).append('<ul class="generated_list ' + global_type + '" id=\"' + name_key + '_list\"></ul>');
        createEntry(name_key, global_key);
        var params = '\'' + name_key + '\',\'' + global_key + '\'';
        $("#" + list_div_id).append('<div class="add_btn_div"><a class="icon_btn add_btn" onclick="createEntry(' + params + ')" ><i class="fa fa-plus"></i></a></div>');
        $(".main_div").append('<hr>');
    });
}

function createEntry(name_key, global_key)
{
    // console.log(name_key, global_key);
    num_entries_per_category[global_key] += 1;
    var index =  num_entries_per_category[global_key];
    var list_id = name_key + "_list";
    var list_name = "#" + list_id;

    var entry_id = 'entry_' + index;
    $(list_name).append('<li class="entry_group" id=' + entry_id + '></li>');
    $(list_name + " #" + entry_id).append('<div class="entry_box_div"><ul class="entry_box_list" id="options_list_' + index + '"></ul></div>');

    $.each(all_options[global_key][0], function (key, list) {
        if(key == 'options') return;
        // console.log(key);
        $(list_name + " li #options_list_" + index).append('<li class="group_title" id=' + key.split(":")[0] + '></li>');
        var dropdown_list = $(list_name + " li").last();
        createDropdownElement(dropdown_list, key, list);
    });

    var params = '\'' + name_key + '\', ' + index;
    $(list_name + " li #options_list_" + index).append('<li class="icon_btn_list_item"><a class="icon_btn remove_btn" onclick=\"removeMe(' + params + ')\"><i class="fa fa-times"></i></a></li>');

}

function removeMe(global_key,index)
{
    var list_id = global_key + "_list";
    var entry_element = "#" + list_id + " #entry_" + index;
    // console.log("Removing " + global_key + " " + index + " | " + entry_element);
    $(entry_element).remove();
    // num_entries_per_category[global_key] -= 1;
}

function entryClick(sender, text, is_array)
{
    // console.log(sender, text, is_array);
    if(is_array) {
        var element_rep = sender.closest('.entry_array_element');
        if(text == "REMOVE") {
            $(element_rep).remove();
        }
        else if ( element_rep.children[0].text != "+") {
            element_rep.children[0].text = text;
        }
        else {
            var element_clone = $(element_rep).clone();
            element_clone.prependTo(element_rep.closest(".entry_array"));
            element_rep = element_clone.children("a").first();
            element_rep.text(sender.text);
        }
    }
    else {
        sender.closest("div").children[1].text = text;
    }
}


function createDropdownElement(element, key, list) {
    // console.log(element, key, list);
    var btn_id = element.attr('id') + "_btn";
    element.append('<div class="option_list_div"></div');
    var option_list_div = element.children("div");

    var nest_check = key.indexOf(":>");
    var nest_count = 0;
    if(nest_check >= 0) {
        nest_count = key.match(/>/g).length;
    }
    key = key.split(":")[0];

    option_list_div.append('<p class="entry_list_title" id="' + nest_count + '">' + key + ':</p>');

    var list_head =  list[0];
    var is_array = list_head.includes("array");
    option_list_element = option_list_div
    if(is_array) {
        option_list_div.append('<ul class="entry_array"><li class="entry_array_element"></li</ul>');
        option_list_element = option_list_div.children("ul").children(0);
    }
    option_list_element.append('<a class="btn dropdown-toggle entry_text" id="' + btn_id + '" type="button" data-toggle="dropdown">+</a>');

    var tie_check = list_head.indexOf(":.");
    if(tie_check >= 0) {
        var tie_path = list_head.substring(tie_check+2, list_head.length).split(".");
        var sub_list = all_options;
        var last_path_key = "";
        $.each(tie_path, function (i, path_key) {
            if(sub_list[path_key] == undefined) return;
            sub_list = sub_list[path_key];
            // console.log(tie_path, path_key, sub_list);
            if(i < tie_path.length - 1 && sub_list[0] != undefined)
            sub_list = sub_list[0];
            last_path_key = path_key;
        });
        // console.log(tie_path);
        // console.log("List", sub_list);
        // console.log("List[0]", $.type(sub_list[0]));
        if($.type(sub_list) == "array" && $.type(sub_list[0]) == "object") {
            list = [];
            last_path_key = last_path_key.split(":")[0];
            // console.log(last_path_key, sub_list);
            $.each(sub_list[0], function (key, sub_list_item) {
                if(key == 'options') return;
                console.log(last_path_key + "_" + key);
                list.push(last_path_key + "_" + key);
            });
        }
        else {
            list = sub_list;
        }
    }
    console.log(list);
    createDropdown(option_list_element, key, list, btn_id, is_array);
}

function createDropdown(element, key, list, btn_id, is_array) {
    var list_head =  list[0];

    element.append('<ul class="dropdown-menu" role="menu" aria-labelledby=\"' + btn_id + '\"></ul>');
    if(is_array && list[0] != "REMOVE") list.splice(0,0,"REMOVE");
    $.each(list, function (i, item) {
        // console.log(item);
        if($.type(item) == "array") return;
        if(item.includes("enter")) {
            var prefix = "";
            if( list_head.includes("address") ) prefix = "0x";
            option_list_element.children(":last").append('<li class="option_dropdown" role="presentation">' + prefix + '<input class="text_entry_' + key + '" type="text" name="fname"></li>');
            $('.text_entry_' + key).bind("enterKey",function(e) { entryClick(e.target, prefix + e.target.value); });
            $('.text_entry_'+ key).keyup(function(e){ if(e.keyCode == 13) { $(this).trigger("enterKey"); } });
        }
        else {
            option_list_element.children(":last").append('<li class="option_dropdown" role="presentation"><a id="entry_option" role="menuitem" onclick="entryClick(this, this.text, ' + is_array + ')" tabindex="-1">' + item + '</a></li>');
        }
    });


}

$(document).ready(function() {
    $( ".main_div" ).each(function( index ) {
        createMain(index);
    });

    $(".dropdown-toggle").dropdown();
});

function loadJSON() {
    $.each(example_profile, function(group_key, group) {
        // console.log(group_key, group);
        var found = false;
        var option_full_name = "";
        $.each(all_options, function(option_group_key, option_group) {
            if(found) return;
            if(option_group_key.substring(0, group_key.length) == group_key) {
                option_full_name = option_group_key;
                found = true;
            }
        });
        // console.log(group_key, option_full_name);

        var group_div = $("#" + group_key + "_div");
        if(group_div == undefined) return;

        var index = 0;
        $.each(group, function(item_key, item) {
            // console.log(item_key,item);
            if(item_key == "type") return;

            var item_index = parseInt(item_key.substring(group_key.length+1, item_key.length));
            // console.log(item_index + 1, group_div.children(".generated_list").children(".entry_group").length);
            if( item_index+1 > group_div.children(".generated_list").children(".entry_group").length ) {
                // console.log("Creating entry", group_key, option_full_name);
                createEntry(group_key, option_full_name);
            }

            var option_list = group_div.children(".generated_list").children(".entry_group").eq(index).children(".entry_box_div").children(".entry_box_list");
            $.each(item, function(item_list_key, item_list) {
                var to_enter = {};
                if($.isNumeric(item_list_key)) {
                    $.each(item_list, function(nested_item_list_key, nested_item_list) {
                        to_enter[nested_item_list_key] = nested_item_list;
                    });
                }
                else {
                    to_enter[item_list_key] = item_list;
                }
                $.each(to_enter, function(to_enter_key, to_enter_item) {
                    if(to_enter_key.substring(0,"num_".length) == "num_") return;
                    // console.log(item_list_key, item_list);
                    var item_list_element = option_list.children("#" + to_enter_key);
                    // console.log("#" + to_enter_key, item_list_element);
                    if(item_list_element == undefined) return;

                    // console.log($.type(item_list), item_list);
                    if($.type(to_enter_item) == "string") {
                        var options_source = item_list_element.children(".option_list_div").first().children(".dropdown-menu").children("li");
                        // console.log(options_source);
                        if(options_source.children("input").length > 0) {
                            var text_element = item_list_element.children(".option_list_div").first().children("a");
                            // console.log("text_element", text_element);
                            entryClick(options_source.children("input").first()[0], item_list, false);
                        }
                        else {
                            var found = false;
                            var options_to_trigger = options_source.children("a");
                            $.each(options_to_trigger, function(j, option) {
                                // console.log("option", to_enter_key, item_list, option.text);
                                if(to_enter_item == option.text) {
                                    $(option).trigger( "click" );
                                }
                            });
                        }
                    }
                    else {
                        var options_to_trigger = item_list_element.children(".option_list_div").children(".entry_array").children(".entry_array_element").first().children(".dropdown-menu").children("li").children("a");
                        $.each(to_enter_item, function(i, request) {
                            $.each(options_to_trigger, function(j, option) {
                                if(request == option.text) {
                                    // console.log(request, option);
                                    $(option).trigger( "click" );
                                }
                            });
                        });
                    }
                });
            });
            index++;
        });
    });
}

function generateJSON(print_json_string) {
    var full_parse = {};
    $.each($(".generated_list"), function (i, entry) {
        var entry_parse = {"type":"none"};
        var group_name = entry.id.replace("_list", "");
        var group_type_raw = $(entry).attr('class').split(' ')[1];
        // console.log(group_type_raw);
        var group_type = group_type_raw.substring("global_type_".length, group_type_raw.length);
        // console.log(group_type);
        entry_parse["type"] = group_type;
        $.each(entry.children, function (j, group) {
            if(group.className != 'entry_group') return;
            var component_name = group_name + "_" + j;
            entry_parse[component_name] = {};
            $.each(group.children[0].children[0].children, function (k, category) {
                // if(category.className != 'group_title') return;
                var data = category.children[0].children[1];
                var indent = category.children[0].children[0].id;
                if(entry_parse[component_name][indent] == undefined) {
                    if($.isNumeric(indent) && indent > 0) {
                        entry_parse[component_name][indent] = {};
                    }
                }
                if( data === undefined) return;
                else if ($(data).is("ul")) {
                    var name_list = {}; var num_elements = 0;
                    $.each(data.children, function(l, element_list) {
                        var name = element_list.children[0].text;
                        if(name == "+") return;
                        name_list[l] = name;
                        num_elements++;
                    });
                    if(num_elements == 0) name_list = "";

                    if(indent > 0) entry_parse[component_name][indent][category.id] = name_list;
                    else entry_parse[component_name][category.id] = name_list;

                    var num_id = category.id;
                    if(num_id[0] == ".") num_id = num_id.replace(".", ".num_");
                    else num_id = "num_" + num_id;
                    if(indent > 0) entry_parse[component_name][indent][num_id]= num_elements;
                    else entry_parse[component_name][num_id] = num_elements;
                }
                else {
                    var name = data.text;
                    if(category.id == name || name == "+") // check if unchanged
                    name = "";
                    if(indent > 0) entry_parse[component_name][indent][category.id] = name;
                    else entry_parse[component_name][category.id] = name;
                }
            });
        });
        full_parse[group_name] = entry_parse;
    });
    // console.log("print", print_json_string);
    if(print_json_string == true) {
        var json_txt = JSON.stringify(full_parse);
        console.log(json_txt);
    }
    return full_parse;
}

function today() {
    var d = new Date();
    return strDate = (d.getMonth()+1) + "/" + d.getDate() + "/" + d.getFullYear();
}

var profile_name = "C12_profile";

var c_header =
"\/\/ ********************** DO NOT EDIT - AUTO-GENERATED ********************** \/\/\n"
+ "\/\/ " + profile_name + ".h\n"
+ "\n"
+ "\/\/ Created by Combine Profile Generator " + version + " on " + today() + "\n"
+ "\/\/ Copyright © Marbl. All rights reserved.\n"
+ "\n"
+ "#ifndef " + profile_name + "_h\n"
+ "#define " + profile_name + "_h\n"
+ "\n"
+ "#define NONE 0\n\n";

function generateC() {
    json = generateJSON(false);

    var full_text = c_header;
    var collection = {};

    $.each(json, function (group_key, group) {
        var section_text = "";
        var include_group = false;
        $.each(all_options, function(option_group_key, option_group) {

            // console.log(option_group_key, option_group[0]['options']);
            if(option_group[0]['options'] != undefined && option_group[0]['options'].includes("~"))
            // if(option_group_key.includes(group_key) && option_group_key.includes("~"))
                include_group = true;
        });
        // console.log(group)
        group_key = deplural(group_key);

        switch (group["type"]) {
            case "list":
                // console.log("group", group_key);
                // collection[group_key] = [];
                $.each(group, function (entry_key, entry) {
                    if(!$.isPlainObject(entry)) return;
                    section_text = "";
                    var full_group_name = "";
                    var found_definition = false;
                    $.each(entry, function (list_key, list) {
                        if(!$.isPlainObject(list)) return;
                        found_definition = true;
                        full_group_name = group_key + "_" + list_key;
                        section_text += "#define " + full_group_name + " { ";

                        $.each(list, function (i, list_item) {
                            // console.log(list_item);
                            section_text += list_item;
                            if(i < Object.keys(list).length - 1)
                                section_text += ", ";
                        });
                        section_text += " }\n";
                        // console.log(full_group_name);
                        // collection[group_key].push(full_group_name);
                    });
                    if(!found_definition) return;
                    full_text += section_text;
                });
                full_text += "\n";
                break;
            case "group":
                if(include_group)
                    collection[group_key] = [];
                $.each(group, function (entry_key, entry) {
                    section_text = "";
                    // console.log("entry", entry_key, entry);
                    var group_item_names = {};
                    if(!$.isPlainObject(entry)) return;
                    var group_name = entry_key;
                    var i = 0;
                    // console.log(entry_key);
                    $.each(entry, function (item_key, item) {
                        if($.isPlainObject(item)) {
                            console.log("Group should not have internal list.");
                        }
                        else {
                            if(item_key != "NAME") {
                                section_text += "#define " + group_name + "_" + item_key + " " + item + "\n";
                                group_item_names[i++] = (group_name + "_" + item_key);
                            }
                            else {
                                group_name = item;
                            }
                        }
                    });
                    if(group_name == "") return;
                    section_text += "#define " + group_name + "_" + group_key + " { ";
                    $.each(group_item_names, function (i, group_item) {
                        section_text += group_item;
                        if( i < Object.keys(group_item_names).length - 1)
                        section_text += ", ";
                    });
                    section_text += " }\n\n";
                    if(include_group)
                        collection[group_key].push(group_name + "_" + group_key);
                    full_text += section_text;
                });
                break;
            case "struct":
                if(include_group)
                    collection[group_key] = [];
                $.each(group, function (entry_key, entry) {
                    section_text = "";
                    var id = "";
                    if(!$.isPlainObject(entry)) return;
                    $.each(entry, function (item_key, item) {
                        if($.isPlainObject(item)) {
                            section_text += "\t." + item_key + " = { ";
                            $.each(item, function (i, list_item) {
                                if(list_item == "" ) section_text += "NONE";
                                else section_text += list_item;
                                if(i < Object.keys(item).length - 1)
                                section_text += ", ";
                            });
                            section_text += " }, \\\n";
                        }
                        else {
                            if(item_key == "ID") {
                                id = item;
                                var macro_name = item.replace("_ID", "");
                                section_text += "#define " + macro_name + " { \\\n";
                                if(include_group)
                                    collection[group_key].push(macro_name);
                            }
                            section_text += "\t." + item_key + " = ";
                            if(item == "" ) section_text += "NONE";
                            else section_text += item;
                            if(item_key != Object.keys(entry)[Object.keys(entry).length-1]) {
                                section_text += ",";
                            }
                            section_text += " \\\n";
                        }
                    });
                    if(id == "") return;
                    section_text += "}\n\n";
                    full_text += section_text;
                });
                break;
            case "nested":
                collection[group_key] = [];
                $.each(group, function (entry_key, entry) {
                    // console.log(":", entry);
                    if($.isPlainObject(entry)) {
                        section_text += "\t\t{ \\\n";
                        $.each(entry, function (list_key, list) {
                            if($.isPlainObject(list)) {
                                if($.isNumeric(Object.keys(list)[0])) {
                                    section_text += "\t\t\t{ \/* " + list_key + " *\/ \\\n";
                                    $.each(list, function (item_key, item) {
                                        section_text += "\t\t\t\t" + item;
                                        // console.log(parseInt(item_key), Object.keys(list).length);
                                        if(parseInt(item_key) < Object.keys(list).length - 1) {
                                            section_text += ",";
                                        }
                                        section_text += " \\\n";
                                    });
                                    section_text += "\t\t\t}, \\\n";
                                }
                                else if(Object.keys(list).length > 0) {
                                    section_text += "\t\t\t{ \\\n";
                                    $.each(list, function (item_key, item) {
                                        if($.isPlainObject(item) || ( item == "" && $.type(item) == "string" )) {
                                            section_text += "\t\t\t\t{ \/* " + item_key + " *\/ \\\n";
                                            $.each(item, function (nested_key, nested) {
                                                section_text += "\t\t\t\t\t" + nested;
                                                if(nested_key != Object.keys(item)[Object.keys(item).length-1])
                                                    section_text += ",";
                                                section_text += " \\\n";
                                            });
                                            section_text += "\t\t\t\t}, \\\n";
                                        }
                                        else {
                                            section_text += "\t\t\t\t" + item + ", \\\n";
                                        }
                                    });
                                    section_text += "\t\t\t}, \\\n";
                                }
                            }
                            else {
                                section_text += "\t\t\t" + list + ", \/* " + list_key + " *\/ \\\n";
                            }
                        });
                        // section_text += ">\t\t\t},\n";
                        section_text += "\t\t}";
                        if(entry_key != Object.keys(group)[Object.keys(group).length-1])
                            section_text += ","
                        section_text += " \\\n";
                    }
                });
                // section_text += "\t\t}";
                // console.log(section_text);
                collection[group_key].push(section_text);
                break;
            default:
        }
        // if(section_text != "")
        // full_text += "\n";
    });

    section_text = "#define PROFILE_TEMPLATE \\\n{ \\\n"
    $.each(collection, function (group_key, group) {
        section_text += "\t{ \/* " + group_key.split(":")[0] + " *\/ \\\n";
        if(!group[0].includes("{")) {
            section_text += "\t\t" + group.length + ", \\\n";
            section_text += "\t\t{ \\\n";
        }
        // section_text += "\t\t\t{\n";
        $.each(group, function(i, group_item) {
            // console.log("A", group_item);
            if(!group[0].includes("{"))
                section_text += "\t\t\t";
            section_text += group_item;

            if(!group[0].includes("{")) {
                if(i < group.length-1) section_text +=",";
                section_text += " \\\n";
            }
        });
        // section_text += "\t\t\t}\n";
        if(!group[0].includes("{")) {
            section_text += "\t\t} \\\n";
        }
        // }
        section_text += "\t}";
        if(group_key != Object.keys(collection)[Object.keys(collection).length-1])
            section_text += ", ";
        section_text += " \\\n";
    });
    section_text += "};\n";
    section_text += "\n#endif \/*" + profile_name + "_h *\/";

    full_text += section_text;
    console.log(full_text);
    $(".main_div").append('<p class="full_text" style="color:white">' + full_text + '</p>');
    $(".main_div").append('<p class="text_display">' + full_text.replace(/\n/g,"<br>") + '</p>');
    $(".text_display").on("click", function() {
        $(".full_text").select();
        console.log(document.execCommand("copy"));

        $(".full_text").remove();
        $(".text_display").remove();
    });
}

function deplural(str) {
    // if(str[str.length-1] == "s" ) {
    //     if(str.substring(str.length-3, str.length-1) == "ie")
    //         return str.substring(0, str.length-3) + "y";
    //     else
    //         return str.substring(0, str.length-1);
    // }
    return str;
}
