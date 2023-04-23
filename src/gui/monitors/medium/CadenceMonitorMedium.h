#ifndef _CADENCE_MONITOR_MEDIUM_H
#define _CADENCE_MONITOR_MEDIUM_H

#include "BaseNumericMonitorMedium.h"


class CadenceMonitorMedium : public BaseNumericMonitorMedium
{
public:
    CadenceMonitorMedium(const char* title = " " LV_SYMBOL_REFRESH, const char* units = " rpm") : BaseNumericMonitorMedium(BikeStateAttributeIndex::CRANK_ROTATIONS_PER_MIN, MonitorAttributeType::EVERY_SECOND, title, units) {}

	virtual void initBluetoothStats()
	{
		BaseNumericMonitorMedium::initBluetoothStats();
		// Extra bluetooth stat is required for this attribute
		this->bluetoothBike->readBikeStateAttribute(BikeStateAttributeIndex::CRANK_ROTATIONS, MonitorAttributeType::EVERY_TEN_SECONDS);
	}
};

#endif