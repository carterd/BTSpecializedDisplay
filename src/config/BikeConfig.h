#ifndef  _BIKE_CONFIG_H
#define  _BIKE_CONFIG_H

#include <Arduino.h>

#include "..\dev\BluetoothBikeState.h"

enum class BikeConfigAttributeIndex { Beeper = 1, FakeChannel = 2, WheelCircumference = 3, SupportAssistEco = 4, SupportAssistTrail = 5, SupportAssistTurbo = 6, PeakPowerAssistEco = 7, PeakPowerAssistTrail = 8, PeakPowerAssistTurbo = 9, SupportAssistLevelsManaged = 10, PeakPowerAssistLevelsManaged = 11 };

template <typename T>
class BikeConfigAttr {
public:
    T value;
    bool managed;
    BikeConfigAttr(T value, bool managed) { this->value = value; this->managed = managed; }
    BikeConfigAttr(bool managed) { this->manged = managed; }
    BikeConfigAttr() { this->managed = false; }
    void setMangedValue(T value) { this->managed = true; this->value = value; }
    friend bool operator==(const BikeConfigAttr<T>& lhs, const BikeConfigAttr<T>& rhs) {
        return (lhs.managed == rhs.managed && lhs.value == rhs.value);
    }
};


struct BikeConfig {
    BikeConfigAttr<bool> beeper;                   // 0 No beeping, 1 beeps
    BikeConfigAttr<uint8_t> fakeChannel;            //
    BikeConfigAttr<uint16_t> wheelCircumference;    //
    BikeConfigAttr<AssistLevels> supportAssistLevels;      // Should be something like 5,20,25,100 et-al
    BikeConfigAttr<AssistLevels> peakPowerAssistLevels;       // Should be just 0-10
    BikeConfig(BikeConfigAttr<bool> beeper, BikeConfigAttr<uint8_t> fakeChannel, BikeConfigAttr<uint16_t> wheelCircumference, BikeConfigAttr<AssistLevels> supportAssistLevels, BikeConfigAttr<AssistLevels> peakPowerAssistLevels) {
        this->beeper = beeper;
        this->fakeChannel = fakeChannel;
        this->wheelCircumference = wheelCircumference;
        this->supportAssistLevels = supportAssistLevels;
        this->peakPowerAssistLevels = peakPowerAssistLevels;
    }
    BikeConfig() {};
    void setManagedValue(BikeConfigAttributeIndex bikeConfigAttributeIndex, bool managed) {
        switch (bikeConfigAttributeIndex) {
            case BikeConfigAttributeIndex::Beeper:
            this->beeper.managed = managed;
            break;
            case BikeConfigAttributeIndex::FakeChannel:
            this->fakeChannel.managed = managed;
            break;
            case BikeConfigAttributeIndex::WheelCircumference:
            this->wheelCircumference.managed = managed;
            break;
            case BikeConfigAttributeIndex::SupportAssistLevelsManaged:
            case BikeConfigAttributeIndex::SupportAssistEco:
            case BikeConfigAttributeIndex::SupportAssistTrail:
            case BikeConfigAttributeIndex::SupportAssistTurbo:
            this->supportAssistLevels.managed = managed;
            break;
            case BikeConfigAttributeIndex::PeakPowerAssistLevelsManaged:
            case BikeConfigAttributeIndex::PeakPowerAssistEco:
            case BikeConfigAttributeIndex::PeakPowerAssistTrail:
            case BikeConfigAttributeIndex::PeakPowerAssistTurbo:
            this->peakPowerAssistLevels.managed = managed;
            break;
        }
    }
    friend bool operator==(const BikeConfig& lhs, const BikeConfig& rhs) {
        return (lhs.beeper == rhs.beeper && lhs.fakeChannel == rhs.fakeChannel && lhs.wheelCircumference == rhs.wheelCircumference && lhs.peakPowerAssistLevels == rhs.peakPowerAssistLevels && lhs.supportAssistLevels == rhs.supportAssistLevels);
    }
    void setIntValue(BikeConfigAttributeIndex bikeConfigAttributeIndex, int newValue) {
        switch (bikeConfigAttributeIndex) {
            case BikeConfigAttributeIndex::Beeper:
            this->beeper.value = newValue;
            break;
            case BikeConfigAttributeIndex::FakeChannel:
            this->fakeChannel.value = newValue;
            break;
            case BikeConfigAttributeIndex::WheelCircumference:
            this->wheelCircumference.value = newValue;
            break;
            case BikeConfigAttributeIndex::SupportAssistEco:
            this->supportAssistLevels.value.eco = newValue;
            break;
            case BikeConfigAttributeIndex::SupportAssistTrail:
            this->supportAssistLevels.value.trail = newValue;
            break;
            case BikeConfigAttributeIndex::SupportAssistTurbo:
            this->supportAssistLevels.value.turbo = newValue;
            break;
            case BikeConfigAttributeIndex::PeakPowerAssistEco:
            this->peakPowerAssistLevels.value.eco = newValue;
            break;
            case BikeConfigAttributeIndex::PeakPowerAssistTrail:
            this->peakPowerAssistLevels.value.trail = newValue;
            break;
            case BikeConfigAttributeIndex::PeakPowerAssistTurbo:
            this->peakPowerAssistLevels.value.turbo = newValue;
            break;
            case BikeConfigAttributeIndex::SupportAssistLevelsManaged:
            this->supportAssistLevels.managed = newValue;
            break;
            case BikeConfigAttributeIndex::PeakPowerAssistLevelsManaged:
            this->peakPowerAssistLevels.managed = newValue;
            break;
        }
    };
    bool getManagedValue(BikeConfigAttributeIndex bikeConfigAttributeIndex) {
        bool managed = false;
        switch (bikeConfigAttributeIndex) {
            case BikeConfigAttributeIndex::Beeper:
            managed = this->beeper.managed;
            break;
            case BikeConfigAttributeIndex::FakeChannel:
            managed = this->fakeChannel.managed;
            break;
            case BikeConfigAttributeIndex::WheelCircumference:
            managed = this->wheelCircumference.managed;
            break;
            case BikeConfigAttributeIndex::SupportAssistLevelsManaged:
            case BikeConfigAttributeIndex::SupportAssistEco:
            case BikeConfigAttributeIndex::SupportAssistTrail:
            case BikeConfigAttributeIndex::SupportAssistTurbo:
            managed = this->supportAssistLevels.managed;
            break;
            case BikeConfigAttributeIndex::PeakPowerAssistLevelsManaged:
            case BikeConfigAttributeIndex::PeakPowerAssistEco:
            case BikeConfigAttributeIndex::PeakPowerAssistTrail:
            case BikeConfigAttributeIndex::PeakPowerAssistTurbo:
            managed = this->peakPowerAssistLevels.managed;
        }
        return managed;
    };

    int getIntValue(BikeConfigAttributeIndex bikeConfigAttributeIndex) {
        int initValue = 0;
        switch (bikeConfigAttributeIndex) {
            case BikeConfigAttributeIndex::Beeper:
            initValue = this->beeper.value;
            break;
            case BikeConfigAttributeIndex::FakeChannel:
            initValue = this->fakeChannel.value;
            break;
            case BikeConfigAttributeIndex::WheelCircumference:
            initValue = this->wheelCircumference.value;
            break;
            case BikeConfigAttributeIndex::SupportAssistEco:
            initValue = this->supportAssistLevels.value.eco;
            break;
            case BikeConfigAttributeIndex::SupportAssistTrail:
            initValue = this->supportAssistLevels.value.trail;
            break;
            case BikeConfigAttributeIndex::SupportAssistTurbo:
            initValue = this->supportAssistLevels.value.turbo;
            break;
            case BikeConfigAttributeIndex::PeakPowerAssistEco:
            initValue = this->peakPowerAssistLevels.value.eco;
            break;
            case BikeConfigAttributeIndex::PeakPowerAssistTrail:
            initValue = this->peakPowerAssistLevels.value.trail;
            break;
            case BikeConfigAttributeIndex::PeakPowerAssistTurbo:
            initValue = this->peakPowerAssistLevels.value.turbo;
            break;
            case BikeConfigAttributeIndex::SupportAssistLevelsManaged:
            initValue = this->supportAssistLevels.managed;
            break;
            case BikeConfigAttributeIndex::PeakPowerAssistLevelsManaged:
            initValue = this->peakPowerAssistLevels.managed;
            break;
        }
        return initValue;
    };

};

#endif