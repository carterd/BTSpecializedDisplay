#ifndef _MEMORY_MONITOR_SMALL_H
#define _MEMORY_MONITOR_SMALL_H

#include "BaseNumericMonitorSmall.h"


class MemoryMonitorSmall : public BaseNumericMonitorSmall
{
public:
    /// <summary>
    /// Constructor for the Display of monitor
    /// </summary>
    /// <param name="title"></param>
    /// <param name="attributeUnits"></param>
    MemoryMonitorSmall(const char* title = "Mem:", const char* attributeUnits = "") : BaseNumericMonitorSmall(BikeStateAttributeIndex::MOTOR_POWER, MonitorAttributeType::EVERY_SECOND, title, attributeUnits) {}

   	/// <summary>
	/// The callback on the list required to be updated, i.e. a bluetooth device detected
	/// </summary>
	/// <param name="event">The lv event that identifies pressing the device entry</param>
	virtual void updateLvObj();

    /// <summary>
    /// This will ensure the stats are initialised and the correct interest for the monitor is assigned to the controller
    /// </summary>
    virtual void initBluetoothStats() {}
};

#endif