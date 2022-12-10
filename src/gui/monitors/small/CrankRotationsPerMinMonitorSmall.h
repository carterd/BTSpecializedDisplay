#ifndef _CRANK_ROTATIONS_MONITOR_SMALL_H
#define _CRANK_ROTATIONS_MONITOR_SMALL_H

#include "BaseNumericSmall.h"


class CrankRotationsPerMinMonitorSmall : public BaseNumericSmall
{
public:
    CrankRotationsPerMinMonitorSmall(const char* title = "Cad:") : BaseNumericSmall(BikeStateAttributeIndex::CRANK_ROTATIONS_PER_MIN, MonitorAttributeType::EVERY_SECOND, title, NULL) {}

	virtual void initBluetoothStats()
	{
		BaseNumericSmall::initBluetoothStats();
		// Extra bluetooth stat is required for this attribute
		this->bluetoothBikeController->getConnectedBluetoothBike().readBikeStateAttribute(BikeStateAttributeIndex::CRANK_ROTATIONS, MonitorAttributeType::EVERY_TEN_SECONDS);
	}
};

#endif