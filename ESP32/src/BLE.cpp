#include "BLE.h"


BLEServer *pServer = NULL;
BLECharacteristic *pCharacteristic;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint8_t BLEbuf[32] = { 0 };
uint32_t cnt = 0;




class MyServerCallbacks : public BLEServerCallbacks {
	void onConnect(BLEServer* pServer) {
		deviceConnected = true;
	};

	void onDisconnect(BLEServer* pServer) {
		deviceConnected = false;
	}
};

class MyCallbacks : public BLECharacteristicCallbacks {
	void onWrite(BLECharacteristic *pCharacteristic) {
		std::string rxValue = pCharacteristic->getValue();

		if (rxValue.length() > 0)
		{
	//  Serial.print("------>Received Value: ");
			for (int i = 0; i < rxValue.length(); i++) {
	//	Serial.print(rxValue[i]);
				rxValue.copy((char*)BLEbuf, rxValue.length(),0);
  //    Serial.printf("%s\r\n",BLEbuf);
			}
	//  Serial.println();

			if (rxValue.find("A") != -1) {
	//      Serial.print("Rx A!");
			}
			else if (rxValue.find("B") != -1) {
	//     Serial.print("Rx B!");
			}
	//  Serial.println();
		}
	}
};


void BLEInit(void)
{
	// Create the BLE Device
  BLEDevice::init("YingHuo");

  // Create the BLE Server
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID_TX, BLECharacteristic::PROPERTY_NOTIFY);

  pCharacteristic->addDescriptor(new BLE2902());

  BLECharacteristic *pCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID_RX, BLECharacteristic::PROPERTY_WRITE);

  pCharacteristic->setCallbacks(new MyCallbacks());

  // Start the service
  pService->start();

  // Start advertising
  pServer->getAdvertising()->start();

}
