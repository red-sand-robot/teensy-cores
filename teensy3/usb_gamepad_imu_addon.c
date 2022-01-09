// usb_gamepad_imu_ao.c
#include "usb_dev.h"
#include "usb_gamepad_imu_addon.h"
#include "core_pins.h" // for yield()
#include "HardwareSerial.h"
#include <string.h> // for memcpy()

//#ifdef USB_GAMEPAD_IMU_ADDON // defined by usb_dev.h -> usb_desc.h
#ifdef GAMEPAD_IMU_ADDON_INTERFACE //defined by usb_dev.h -> usb_desc.h

uint16_t usb_gamepad_imu_addon_data[9];

// Maximum number of transmit packets to queue so we don't starve other endpoints for memory
#define TX_PACKET_LIMIT 3

static uint8_t transmit_previous_timeout=0;

// When the PC isn't listening, how long do we wait before discarding data?
#define TX_TIMEOUT_MSEC 30
#if F_CPU == 256000000
  #define TX_TIMEOUT (TX_TIMEOUT_MSEC * 1706)
#elif F_CPU == 240000000
  #define TX_TIMEOUT (TX_TIMEOUT_MSEC * 1600)
#elif F_CPU == 216000000
  #define TX_TIMEOUT (TX_TIMEOUT_MSEC * 1440)
#elif F_CPU == 192000000
  #define TX_TIMEOUT (TX_TIMEOUT_MSEC * 1280)
#elif F_CPU == 180000000
  #define TX_TIMEOUT (TX_TIMEOUT_MSEC * 1200)
#elif F_CPU == 168000000
  #define TX_TIMEOUT (TX_TIMEOUT_MSEC * 1100)
#elif F_CPU == 144000000
  #define TX_TIMEOUT (TX_TIMEOUT_MSEC * 932)
#elif F_CPU == 120000000
  #define TX_TIMEOUT (TX_TIMEOUT_MSEC * 764)
#elif F_CPU == 96000000
  #define TX_TIMEOUT (TX_TIMEOUT_MSEC * 596)
#elif F_CPU == 72000000
  #define TX_TIMEOUT (TX_TIMEOUT_MSEC * 512)
#elif F_CPU == 48000000
  #define TX_TIMEOUT (TX_TIMEOUT_MSEC * 428)
#elif F_CPU == 24000000
  #define TX_TIMEOUT (TX_TIMEOUT_MSEC * 262)
#endif

/* todo
int usb_gamepad_imu_addon_recv(void *buffer, uint32_t timeout)
{
	usb_packet_t *rx_packet;
	uint32_t begin = millis();

	while (1) {
		if (!usb_configuration) return -1;
		rx_packet = usb_rx(GAMEPAD_IMU_AO_RX_ENDPOINT);
		if (rx_packet) break;
		if (millis() - begin > timeout || !timeout) return 0;
		yield();
	}
	memcpy(buffer, rx_packet->buf, GAMEPAD_IMU_AO_RX_SIZE);
	usb_free(rx_packet);
	return GAMEPAD_IMU_AO_RX_SIZE;
}

int usb_gamepad_imu_addon_available(void)
{
	uint32_t usb_gamepad_imu_ao_tx_byte_count;

	if (!usb_configuration) return 0;
	count = usb_rx_byte_count(GAMEPAD_IMU_AO_RX_ENDPOINT);
	return usb_gamepad_imu_ao_tx_byte_count;
}
*/
int usb_gamepad_imu_addon_send_buffer_check(void)
{
  uint32_t usb_gamepad_imu_addon_tx_packet_count;

	if (!usb_configuration) return 0;
	usb_gamepad_imu_addon_tx_packet_count = usb_tx_packet_count(GAMEPAD_IMU_ADDON_ENDPOINT); // GAMEPAD_IMU_ADDON_ENDPOINT will need to change if receiving data is implemented
	return usb_gamepad_imu_addon_tx_packet_count;
}

int usb_gamepad_imu_addon_send(void)
{
    uint32_t wait_count=0;
    usb_packet_t *tx_packet;

    while (1) {
        if (!usb_configuration) {
            return -1;
        }
        if (usb_tx_packet_count(GAMEPAD_IMU_ADDON_ENDPOINT) < TX_PACKET_LIMIT) {
            tx_packet = usb_malloc(); 
            if (tx_packet) break; 
        }        
        if (++wait_count > TX_TIMEOUT || transmit_previous_timeout) {
            transmit_previous_timeout = 1;
            return -1;
        }
        yield();
    }

    transmit_previous_timeout = 0;
    memcpy(tx_packet->buf, usb_gamepad_imu_addon_data, GAMEPAD_IMU_ADDON_SIZE);
    tx_packet->len = GAMEPAD_IMU_ADDON_SIZE;
    usb_tx(GAMEPAD_IMU_ADDON_ENDPOINT, tx_packet);

    return 0;
}
#endif // USB_GAMEPAD_IMU_ADDON