#ifndef _BLUETOOTH_BIKE_REQUEST_H
#define _BLUETOOTH_BIKE_REQUEST_H

#include <vector>

#include "BluetoothBikeState.h"

/// <summary>
/// This is a class which comprises the required commands to be sent for a specific bike status, as such can contain upto two
/// different attribute pairs whereby a single status attribute may require two different bluetooth characteristic attributes to 
/// be read or updated.
/// </summary>

class BluetoothBikeRequest {
public:
    /// <summary>
    /// Each request can contain upto 2 commands
    /// </summary>
    struct BluetoothBikeRequestCommand {
        EbikeStatusArea area;
        EbikeStatusAttribute attribute;
        BluetoothBikeRequestCommand() { area = EbikeStatusArea::INVALID; attribute.ebikeStatusBattery = EbikeStatusBattery::CAPACITY; }
        BluetoothBikeRequestCommand(EbikeStatusBattery ebikeStatusBattery) { area = EbikeStatusArea::BATTERY; attribute.ebikeStatusBattery = ebikeStatusBattery; }
        BluetoothBikeRequestCommand(EbikeStatusMotor ebikeStatusMotor) { area = EbikeStatusArea::MOTOR; attribute.ebikeStatusMotor = ebikeStatusMotor; }
        BluetoothBikeRequestCommand(EbikeStatusOther ebikeStatusOther) { area = EbikeStatusArea::OTHER; attribute.ebikeStatusOther = ebikeStatusOther; }
        bool isValid() { return area != EbikeStatusArea::INVALID; }
    };

private:
    /// <summary>
    /// The first request in for form of an instance of bluetoothRequestCommand
    /// </summary>
    std::vector<BluetoothBikeRequestCommand> requests;

public:
    /// <summary>
    /// Three command constructor for the BluetoothBikeRequest object
    /// </summary>
    BluetoothBikeRequest(BluetoothBikeRequestCommand request1, BluetoothBikeRequestCommand request2, BluetoothBikeRequestCommand request3) {
        this->requests.push_back(request1);
        this->requests.push_back(request2);
        this->requests.push_back(request3);
    }

    /// <summary>
    /// Two command constructor for the BluetoothBikeRequest object
    /// </summary>
    BluetoothBikeRequest(BluetoothBikeRequestCommand request1, BluetoothBikeRequestCommand request2) {
        this->requests.push_back(request1); 
        this->requests.push_back(request2);
    }

    /// <summary>
    /// Single command constructor for the BluetoothBikeRequest object
    /// </summary>
    BluetoothBikeRequest(BluetoothBikeRequestCommand request1) {
        this->requests.push_back(request1);
    }

    /// <summary>
    /// Default constructor for the BluetoothBikeRequest object
    /// </summary>
    BluetoothBikeRequest() { }

    /// <summary>
    /// Returns the numbers commands required to read the state
    /// </summary>
    /// <returns></returns>
    int getNumberOfCommands() { return this->requests.size(); }

    /// <summary>
    /// Returns the associated command indexed by.
    /// </summary>
    /// <param name="commandIndex">The command index of command requried</param>
    /// <returns>The associated command indexed by commandIndex</returns>
    BluetoothBikeRequestCommand getCommand(int commandIndex) {
        if (this->requests.size() <= commandIndex) {
            return BluetoothBikeRequestCommand();
        }
        return requests[commandIndex];
    }
};

/// <summary>
/// This class is an instance of a mapping between BikeStateAttributeIndex and BluetoothBikeRequest.
/// As such allows mapping 
/// </summary>

class BikeStateToBluetoothBikeRequest {
private:
    /// <summary>
    /// This is an map which is by default initialised with all the attributes of the bike
    /// </summary>
    BluetoothBikeRequest bluetoothBikeRequestMap[static_cast<int>(BikeStateAttributeIndex::BIKE_STATE_ATTRIBUTE_SIZE)];
public:
    /// <summary>
    /// Constructor for the BikeStateToBluetoothBikeRequest map therefore we should ensure we only create the one instance.
    /// </summary>
    BikeStateToBluetoothBikeRequest();

    /// <summary>
    /// Accessor to return the BluetoothBikeRequest for the given bikeStateAttributeIndex
    /// </summary>
    BluetoothBikeRequest getBluetoothBikeRequest(BikeStateAttributeIndex bikeStateAttributeIndex, BikeType bikeType);
};

#endif