#ifndef BLUETOOTH_CONNECTION_H
#define BLUETOOTH_CONNECTION_H

#include <lvgl.h>

#include "BaseLvObject.h"
#include "ButtonLabel.h"
#include "..\dev\BluetoothBikeController.h"
#include "..\dev\ConfigStore.h"

/// <summary>
/// The connection shows an image with a spinner attempts to make a connection to any connecting 
/// </summary>
class BluetoothConnection : public BaseLvObject
{
private:
    /// <summary>
    /// This is the LV background image
    /// </summary>
    lv_img_dsc_t* image;
    /// <summary>
    /// The indev for the encoder to give buttons context for exit
    /// </summary>
    lv_indev_t* indev;
    /// <summary>
    /// The group for the encoder to give buttons context for exit
    /// </summary>
    lv_group_t* group;

    /// <summary>
    /// This is the button label or NULL for not button label
    /// </summary>
    ButtonLabel* buttonLabel = NULL;
    /// <summary>
    /// This is the bluetooth controller object
    /// </summary>
    BluetoothBikeController* bluetoothController;
    /// <summary>
    /// The config store which contains the bluetooth devices to connect to
    /// </summary>
    ConfigStore* configStore;


    bool connecting;

    /// <summary>
    /// This is the object given focus on this object loosing focus
    /// </summary>
    BaseLvObject* defocusLvObj;

private:
    /// <summary>
    /// Call on hiding the button label if one has been defined for this gui object
    /// </summary>
    void hideButtonLabels();
    /// <summary>
    /// This will start the process of connection to any of the BLE devices in the config store
    /// </summary>
    void startBTConnection();
    /// <summary>
    /// This will end the process of connection
    /// </summary>
    void stopBTConnection();
public:
    /// <summary>
    /// The construct of for the bluetooth connection object
    /// </summary>
    /// <param name="bluetoothMaster">an instance of the bluetooth controller</param>
    /// <param name="configStore">an instance of the config store for devices to connect to</param>
    /// <param name="configStore">an image to be the background image while connecting</param>
    BluetoothConnection(BluetoothBikeController* bluetoothMaster, ConfigStore* configStore, lv_img_dsc_t* image, lv_indev_t* indev);

    /// <summary>
    /// Returns the LV object instance to represent this class instance
    /// </summary>
    /// <returns>The LV object instance to represent this class instance</returns>
    virtual lv_obj_t* createLvObj(lv_obj_t* parent);

    /// <summary>
    /// This means the object and any sub objects should set any groups to be in focus at this point
    /// </summary>
    virtual void focusLvObj(BaseLvObject* defocusLvObj = NULL);

    /// <summary>
    /// Used to set the button label for this object or NULL for no button label
    /// </summary>
    /// <param name="buttonLabel">The button label object for this gui object</param>
    void setButtonLabel(ButtonLabel* buttonLabel);

    void refreshCB(lv_event_t* event);

    void exitButtonCB(lv_event_t* event);
};

#endif