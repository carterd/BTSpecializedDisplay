#include "MonitorLvObject.h"

MonitorLvObject::MonitorLvObject() {
    this->bluetoothBikeController = NULL;
}

void MonitorLvObject::setBluetoothController(BluetoothBikeController* bluetoothBikeController) {
    this->bluetoothBikeController = bluetoothBikeController;
}
