#ifndef __BLE_H
#define __BLE_H

#include "Arduino.h"
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" // UART service UUID
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

extern BLEServer *pServer ;
extern BLECharacteristic *pCharacteristic;
extern bool deviceConnected ;
extern bool oldDeviceConnected ;
extern uint8_t BLEbuf[32] ;

void BLEInit(void);

#endif
