#ifndef _CADENCE_MONITOR_SMALL_H
#define _CADENCE_MONITOR_SMALL_H

#include "BaseNumericMonitorSmall.h"


class CadenceMonitorSmall : public BaseNumericMonitorSmall
{
public:
    CadenceMonitorSmall(const char* title = " " LV_SYMBOL_REFRESH, const char* units = " rpm") : BaseNumericMonitorSmall(BikeStateAttributeIndex::CRANK_ROTATIONS_PER_MIN, MonitorAttributeType::EVERY_SECOND, title, units) {}

	virtual void initBluetoothStats()
	{
		BaseNumericMonitorSmall::initBluetoothStats();
		// Extra bluetooth stat is required for this attribute
		this->bluetoothBike->readBikeStateAttribute(BikeStateAttributeIndex::CRANK_ROTATIONS, MonitorAttributeType::EVERY_TEN_SECONDS);
	}
};

#endif