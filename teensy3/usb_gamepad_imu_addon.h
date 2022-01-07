// usb_imu_addon.h
// header file for IMU Add-On usb communication.
#ifndef _USB_GAMEPAD_IMU_ADDON_H_
#define _USB_GAMEPAD_IMU_ADDON_H_

#if defined(GAMEPAD_IMU_ADDON_INTERFACE)

#include <inttypes.h>

// C language implementation
#ifdef __cplusplus
extern "C" {
#endif
int usb_gamepad_imu_addon_send(void);
int usb_gamepad_imu_addon_send_buffer_check(void);
// we have packets that are 12 bytes long - 2 bytes per axes
extern uint16_t usb_gamepad_imu_addon_data[9]; //9, not 6, for the report ID + timestamp (report ID is 1 byte, other is padding since I don't feel like bit shifting everything)
#ifdef __cplusplus
}
#endif

// C++ interface
#ifdef __cplusplus
class usb_gamepad_imu_addon_class
{
private:
    static uint8_t auto_send;

public:
    void collectAccelXYZ(int16_t x, int16_t y, int16_t z) {
        if (x < -32767) x = -32767;
        if (x > 32767) x = 32767;
        usb_gamepad_imu_addon_data[1] =  x ;

        if (y < -32767) y = -32767;
        if (y > 32767) y = 32767;
        usb_gamepad_imu_addon_data[2] =  y ;

        if (z < -32767) z = -32767;
        if (z > 32767) z = 32767;
        usb_gamepad_imu_addon_data[3] = z ;
        
        if(auto_send) usb_gamepad_imu_addon_send();
    }
    void collectAccelX(int16_t val) {
        usb_gamepad_imu_addon_data[1] = val;
        if(auto_send) usb_gamepad_imu_addon_send();
    }
    void collectAccelY(int16_t val) {
        usb_gamepad_imu_addon_data[2] = val;
        if(auto_send) usb_gamepad_imu_addon_send();
    }
    void collectAccelZ(int16_t val) {
        usb_gamepad_imu_addon_data[3] = val;
        if(auto_send) usb_gamepad_imu_addon_send();
    }
    void collectGyroXYZ(int16_t x, int16_t y, int16_t z) {
        if (x < -32767) x = -32767;
        if (x > 32767) x = 32767;
        usb_gamepad_imu_addon_data[4] = x ;

        if (y < -32767) y = -32767;
        if (y > 32767) y = 32767;
        usb_gamepad_imu_addon_data[5] = y ;

        if (z < -32767) z = -32767;
        if (z > 32767) z = 32767;
        usb_gamepad_imu_addon_data[6] = z ;
        if(auto_send) usb_gamepad_imu_addon_send();
    }
    void collectGyroX(int16_t val) {
        usb_gamepad_imu_addon_data[4] = val;
        if(auto_send) usb_gamepad_imu_addon_send();
    }
    void collectGyroY(int16_t val) {
        usb_gamepad_imu_addon_data[5] = val;
        if(auto_send) usb_gamepad_imu_addon_send();
    }
    void collectGyroZ(int16_t val) {
        usb_gamepad_imu_addon_data[6] = val;
        if(auto_send) usb_gamepad_imu_addon_send();
    }

    void collectTimestamp(uint32_t val) {
        usb_gamepad_imu_addon_data[7] = (val >>  0) & 0xFFFF; 
        usb_gamepad_imu_addon_data[8] = (val >> 16) & 0xFFFF;
    }
    
    void setAutoSend(bool send) {
        auto_send = send;
    }

    void send() {
        usb_gamepad_imu_addon_data[0] = 1; // puts the Report ID in byte 0, and dummy data (constant) in byte 1 to align everything
        usb_gamepad_imu_addon_send();
    }

    void checkSendBuffer(){
        usb_gamepad_imu_addon_send_buffer_check();
    }
};
extern usb_gamepad_imu_addon_class imu_addon;

#endif // __cplusplus
#endif // GAMEPAD_IMU_ADDON_INTERFACE
#endif // _USB_GAMEPAD_IMU_ADDON_H_