#include "bluetooth_controller.h"

#include "8Bitdo_payload.h"

#include "btstack.h"
#include "pico/cyw43_arch.h"
#include <stdio.h>
#include <stdlib.h>

static BtControllerConfig *_config = NULL;
static controller_handler _handler = NULL;

static btstack_packet_callback_registration_t hci_event_callback_registration;
static hci_con_handle_t connection_handle = HCI_CON_HANDLE_INVALID;
static gatt_client_characteristic_t target_characteristic;
static gatt_client_notification_t notification_listener;

static btstack_packet_callback_registration_t sm_event_callback_registration;

static void start_scan()
{
  gap_set_scan_parameters(0, 30, 30);
  gap_start_scan();
}

static void hci_handle_gap_advertising(uint8_t packet_type, uint16_t channel, uint8_t *packet, uint16_t size)
{
  bd_addr_t addr;

  gap_event_advertising_report_get_address(packet, addr);

  int isController = memcmp(addr, _config->addr, 6) == 0;
  int isConnected = _config->status == CONTROLLER_CONNECTED;

  if (!isController || isConnected)
  {
    return;
  }

  printf("[HCI] Controller found!\n");

  gap_stop_scan();
  gap_connect(
      _config->addr,
      BD_ADDR_TYPE_LE_PUBLIC);

  _config->status = CONTROLLER_CONNECTED;

  printf("[HCI] Controller connected!\n");
}

static void handle_gatt_client_event(uint8_t packet_type, uint16_t channel, uint8_t *packet, uint16_t size)
{
  static gatt_client_service_t hid_service;
  static int hid_service_found = 0;

  if (packet_type != HCI_EVENT_PACKET)
    return;

  uint8_t event = hci_event_packet_get_type(packet);

  switch (event)
  {
  case GATT_EVENT_SERVICE_QUERY_RESULT:
  {
    gatt_client_service_t service;
    gatt_event_service_query_result_get_service(packet, &service);

    if (service.uuid16 == 0x1812)
    {
      printf("[GATT] HID Service found! (Handles: 0x%04X - 0x%04X)\n",
             service.start_group_handle, service.end_group_handle);

      hid_service = service;
      hid_service_found = 1;
    }
    else
    {
      printf("Generic service found (UUID: 0x%04X)\n", service.uuid16);
    }
    break;
  }

  case GATT_EVENT_CHARACTERISTIC_QUERY_RESULT:
  {
    gatt_client_characteristic_t characteristic;
    gatt_event_characteristic_query_result_get_characteristic(packet, &characteristic);

    // Get HID
    if (characteristic.uuid16 == 0x2A4D && (characteristic.properties & 0x10))
    {
      target_characteristic = characteristic;
    }
    break;
  }

  case GATT_EVENT_QUERY_COMPLETE:
    if (hid_service_found)
    {
      printf("[GATT] Service search finished");
      hid_service_found = 0;

      gatt_client_discover_characteristics_for_service(&handle_gatt_client_event, connection_handle, &hid_service);
    }

    else if (target_characteristic.value_handle != 0)
    {
      printf("[GATT] Characteristic search finished\n");

      gatt_client_listen_for_characteristic_value_updates(
          &notification_listener,
          &handle_gatt_client_event,
          connection_handle,
          &target_characteristic);

      gatt_client_write_client_characteristic_configuration(
          &handle_gatt_client_event,
          connection_handle,
          &target_characteristic,
          GATT_CLIENT_CHARACTERISTICS_CONFIGURATION_NOTIFICATION);

      target_characteristic.value_handle = 0;
    }
    else
    {
      printf("[GATT] Search finished.\n");
    }
    break;

  case GATT_EVENT_NOTIFICATION:
  {
    uint16_t length = gatt_event_notification_get_value_length(packet);
    const uint8_t *value = gatt_event_notification_get_value(packet);
    Controller_Payload *payload = (Controller_Payload *)value;
    _handler(payload);
    break;
  }
  }
}

static void hci_event_handler(uint8_t packet_type, uint16_t channel, uint8_t *packet, uint16_t size)
{
  if (packet_type != HCI_EVENT_PACKET)
    return;

  uint8_t event = hci_event_packet_get_type(packet);

  switch (event)
  {
  case BTSTACK_EVENT_STATE:
    if (btstack_event_state_get_state(packet) != HCI_STATE_WORKING)
    {
      return;
    }

    printf("[HCI] Starting Scan...\n");
    start_scan();

    break;

  // Bluetooth advertisement found
  case GAP_EVENT_ADVERTISING_REPORT:
    hci_handle_gap_advertising(packet_type, channel, packet, size);
    break;

  // Controller event handler
  case HCI_EVENT_LE_META:
  {
    uint8_t subevent =
        hci_event_le_meta_get_subevent_code(packet);

    switch (subevent)
    {
    case HCI_SUBEVENT_LE_CONNECTION_COMPLETE:
    {
      connection_handle = hci_subevent_le_connection_complete_get_connection_handle(packet);
      _config->status = CONTROLLER_CONNECTED;

      printf("[HCI] Controller found, requesting pairing\n");
      sm_request_pairing(connection_handle);
      break;
    }

    default:
      break;
    }

    break;
  }

  // Disconnection handler
  case HCI_EVENT_DISCONNECTION_COMPLETE:
    gap_start_scan();
    printf("[HCI] Controller disconnected\n");
    break;

  default:
    break;
  }
}

void clear_keys()
{
  int devices = le_device_db_max_count();
  for (size_t i = 0; i < devices; i++)
  {
    le_device_db_remove(i);
  }
}

static void sm_event_handler(uint8_t packet_type, uint16_t channel, uint8_t *packet, uint16_t size)
{
  if (packet_type != HCI_EVENT_PACKET)
    return;

  uint8_t event = hci_event_packet_get_type(packet);

  switch (event)
  {
  case SM_EVENT_PAIRING_STARTED:
    printf("[SM] Pairing started...\n");
    break;

  case SM_EVENT_JUST_WORKS_REQUEST:
    printf("[SM] Just Works requested\n");
    sm_just_works_confirm(connection_handle);
    break;

  case SM_EVENT_PAIRING_COMPLETE:
  {
    uint8_t status = sm_event_pairing_complete_get_status(packet);
    if (status != ERROR_CODE_SUCCESS)
    {
      printf("[SM] Pairing failed! Status: 0x%02x\n", status);
      gap_disconnect(connection_handle);
    }
    else
    {
      printf("[SM] Pairing successful!\n");
      printf("[SM] Starting GATT service search\n");
      gatt_client_discover_primary_services(&handle_gatt_client_event, connection_handle);
    }
    break;
  }

  case SM_EVENT_IDENTITY_RESOLVING_STARTED:
    printf("[SM] Identity resolving started\n");
    break;

  case SM_EVENT_IDENTITY_RESOLVING_FAILED:
    printf("[SM] Identity resolving failed...\n");
    break;

  case SM_EVENT_REENCRYPTION_STARTED:
    printf("[SM] Reencryption started...\n");
    break;

  case SM_EVENT_REENCRYPTION_COMPLETE:
  {
    uint8_t status = packet[2];
    if (status == ERROR_CODE_SUCCESS)
    {
      sm_request_pairing(connection_handle);
    }
    else
    {
      printf("[SM] Reencryption failed! Clearing keys...\n");
      clear_keys();
      gap_disconnect(connection_handle);
      _config->status = CONTROLLER_DISCONNECTED;
    }
    break;
  }

  default:
    break;
  }
}

void bluetooth_setup(BtControllerConfig *config)
{
  _config = config;
  _config->status = CONTROLLER_DISCONNECTED;

  cyw43_arch_init();
  l2cap_init();
  gatt_client_init();

  sm_init();

  le_device_db_init();

  sm_set_io_capabilities(IO_CAPABILITY_NO_INPUT_NO_OUTPUT);
  sm_set_authentication_requirements(SM_AUTHREQ_BONDING);

  hci_event_callback_registration.callback = &hci_event_handler;
  hci_add_event_handler(&hci_event_callback_registration);

  sm_event_callback_registration.callback = &sm_event_handler;
  sm_add_event_handler(&sm_event_callback_registration);

  hci_power_control(HCI_POWER_ON);

  btstack_run_loop_execute();
}

void bluetooth_register_handler(controller_handler handler)
{
  _handler = handler;
}