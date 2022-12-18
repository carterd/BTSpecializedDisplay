#ifndef _BASE_NUMERIC_MONITOR_MEDIUM_H
#define _BASE_NUMERIC_MONITOR_MEDIUM_H

#include "..\..\MonitorLvObject.h"

class BaseNumericMonitorMedium : public MonitorLvObject
{
protected:
    union NumericAttributeValue {
        bool valueBool;
        uint8_t valueUint8;
        uint16_t valueUint16;
        uint32_t valueUint32;
        float valueFloat;
    };

    /// <summary>
    /// This is attribute in the bike state to display
    /// </summary>
    BikeStateAttributeIndex bikeStateAttributeIndex;
    /// <summary>
    /// How often the attribute should be checked
    /// </summary>
    MonitorAttributeType monitorAttributeType;

    /// <summary>
    /// This is the value label object require to be updated to show different values
    /// </summary>
    lv_obj_t* value_obj;
    /// <summary>
    /// This is the title displayed for the value
    /// </summary>
    const char* attributeTitle;
    /// <summary>
    /// This is the optional units of the value
    /// </summary>
    const char* attributeUnits;

    /// <summary>
    /// Optioal print format if the defaults aren't valid
    /// </summary>
    const char* printFormat;

    NumericAttributeValue previousBikeStateAttribute;

public:
    BaseNumericMonitorMedium(BikeStateAttributeIndex bikeStateAttributeIndex, MonitorAttributeType monitorAttributeType, const char* attributeTitle, const char* attributeUnits = NULL, const char* printFormat = NULL);

    /// <summary>
    /// Returns the LV object instance to represent this class instance
    /// </summary>
    /// <returns>The LV object instance to represent this class instance</returns>
    virtual lv_obj_t* createLvObj(lv_obj_t* parent);

    /// <summary>
    /// The callback on the list required to be updated, i.e. a bluetooth device detected
    /// </summary>
    /// <param name="event">The lv event that identifies pressing the device entry</param>
    virtual void updateLvObj();

    /// <summary>
    /// This will ensure the stats are initialised and the correct interest for the monitor is assigned to the controller
    /// </summary>
    virtual void initBluetoothStats();

    /// <summary>
    /// Only a monitor in focus should be required to update their gui, hence set this object to be infocus hence updates require display updates
    /// </summary>
    virtual void focusLvObj(BaseLvObject* defocusLvObj = NULL);
};

#endif
