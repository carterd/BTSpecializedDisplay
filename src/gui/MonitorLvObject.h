#ifndef _MONITOR_LV_OBJECT_H
#define _MONITOR_LV_OBJECT_H

#include <lvgl.h>
#include <ArduinoBLE.h>

#include "BaseLvObject.h"
#include "../dev/BluetoothBikeController.h"
#include "../dev/BluetoothBikeDefinitions.h"
#include "../dev/BluetoothBikeState.h"

/// <summary>
/// This is the base object for monitor objects
/// </summary>
class MonitorLvObject : public BaseLvObject
{
protected:
    /// <summary>
    /// A bluetooth controller is to be associated with monitors so they can read intial stats and set which stats are required by the monitor
    /// </summary>
    BluetoothBikeController* bluetoothBikeController;

    /// <summary>
    /// State of the monitor if it's in focus, i.e. display should be updated on status changes
    /// </summary>
    bool infocus = false;
public:
    /// <summary>
    /// Default constructor
    /// </summary>
    MonitorLvObject();

    /// <summary>
    /// Only a monitor in focus should be required to update their gui
    /// </summary>
    virtual void focusLvObj(BaseLvObject* defocusLvObj = NULL) { this->infocus = true; this->updateLvObj(); }

    /// <summary>
    /// Defocus a monitor and hence no more updated on the display
    /// </summary>
    virtual void defocusLvObj() { this->infocus = false; }

    /// <summary>
    /// Setter for the bluetoothController
    /// </summary>
    /// <param name="bluetoothBikeController"></param>
    virtual void setBluetoothController(BluetoothBikeController* bluetoothBikeController);

    /// <summary>
    /// Called each time stats are updated, currently this could be stats on other monitors so a check is required to see if the monitor is really updated.
    /// </summary>
    virtual void statusUpdate() { if (this->infocus) this->updateLvObj(); }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="forceUpdate">If true this will force the update instead of inspecting to identify if changes are required</param>
    virtual void updateLvObj() = 0;

    /// <summary>
    /// Init the stats that the monitor needs to be read and updated from the bike.
    /// </summary>
    virtual void initBluetoothStats() = 0;
};

#endif