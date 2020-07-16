//
//  communicationmanager.c
//  C12_rtos
//
//  Created by Matthew Fonken on 8/13/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include "communicationmanager.h"

void CommunicationManagerInit(void)
{
    
}

generic_comm_return_t CommunicationManager_PerformEvent( comm_event_t event )
{
    generic_comm_return_t ret = false;
    switch( event.generic_comm_event.host->generic_comm_host.protocol )
    {
        case COMPONENT_PROTOCOL_I2C:
            ret = (generic_comm_return_t)PAPI.I2C.Perform(*(i2c_event_t *)&event );
            break;
        case COMPONENT_PROTOCOL_SPI:
            ret = (generic_comm_return_t)PAPI.SPI.Perform(*(spi_event_t *)&event );
            break;
        case COMPONENT_PROTOCOL_UART:
            //            performUARTEvent(*(uart_event_t *)&event, data );
            break;
        case COMPONENT_PROTOCOL_BLE:
            //            performBLEEvent(*(ble_event_t *)&event, data );
            break;
        case COMPONENT_PROTOCOL_SUB:
            //            performSubEvent(*(sub_event_t *)&event, data );
            break;
        default:
            break;
    }
    return ret;
}

void CommunicationManager_PerformTransmit( comm_packet_t * p_packet )
{
    
}
void CommunicationManager_PerformReceive( comm_packet_t * p_packet )
{
#if __BUILD__ == GECKO
    /* Event pointer for handling events */
    struct gecko_cmd_packet* evt;

    /* if there are no events pending then the next call to gecko_wait_event() may cause
     * device go to deep sleep. Make sure that debug prints are flushed before going to sleep */
    if (!gecko_event_pending()) {
      flushLog();
    }

    /* Check for stack event. This is a blocking event listener. If you want non-blocking please see UG136. */
    evt = gecko_wait_event();

    /* Handle events */
    switch (BGLIB_MSG_ID(evt->header)) {
      /* This boot event is generated when the system boots up after reset.
       * Do not call any stack commands before receiving the boot event.
       * Here the system is set to start advertising immediately after boot procedure. */
      case gecko_evt_system_boot_id:

        bootMessage(&(evt->data.evt_system_boot));
        printLog("boot event - starting advertising\r\n");

        /* Set advertising parameters. 100ms advertisement interval.
         * The first parameter is advertising set handle
         * The next two parameters are minimum and maximum advertising interval, both in
         * units of (milliseconds * 1.6).
         * The last two parameters are duration and maxevents left as default. */
        gecko_cmd_le_gap_set_advertise_timing(0, 160, 160, 0, 0);

        /* Start general advertising and enable connections. */
        gecko_cmd_le_gap_start_advertising(0, le_gap_general_discoverable, le_gap_connectable_scannable);
        break;

      case gecko_evt_le_connection_opened_id:

        printLog("connection opened\r\n");

        break;

      case gecko_evt_le_connection_closed_id:

        printLog("connection closed, reason: 0x%2.2x\r\n", evt->data.evt_le_connection_closed.reason);

        /* Check if need to boot to OTA DFU mode */
        if (boot_to_dfu) {
          /* Enter to OTA DFU mode */
          gecko_cmd_system_reset(2);
        } else {
          /* Restart advertising after client has disconnected */
          gecko_cmd_le_gap_start_advertising(0, le_gap_general_discoverable, le_gap_connectable_scannable);
        }
        break;

      /* Events related to OTA upgrading
         ----------------------------------------------------------------------------- */

      /* Check if the user-type OTA Control Characteristic was written.
       * If ota_control was written, boot the device into Device Firmware Upgrade (DFU) mode. */
      case gecko_evt_gatt_server_user_write_request_id:

        if (evt->data.evt_gatt_server_user_write_request.characteristic == gattdb_ota_control) {
          /* Set flag to enter to OTA mode */
          boot_to_dfu = 1;
          /* Send response to Write Request */
          gecko_cmd_gatt_server_send_user_write_response(
            evt->data.evt_gatt_server_user_write_request.connection,
            gattdb_ota_control,
            bg_err_success);

          /* Close connection to enter to DFU OTA mode */
          gecko_cmd_le_connection_close(evt->data.evt_gatt_server_user_write_request.connection);
        }
        break;

      /* Add additional event handlers as your application requires */

      default:
        break;
    }
#endif
}
