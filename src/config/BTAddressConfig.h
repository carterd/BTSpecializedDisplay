#ifndef  _BT_ADDRESS_CONFIG_H
#define  _BT_ADDRESS_CONFIG_H

#include <Arduino.h>
#include <unordered_map>

struct BTAddressesConfig {
    std::unordered_map<String, String> btAddressMap;
    void clear() {
        this->btAddressMap.clear();
    }
    void addBTAddress(const char* address, const char* display) {
        String addressString(address);
        String displayString(display);
        this->addBTAddress(addressString, displayString);
    }
    void addBTAddress(const String& addressString, const String& displayString) {
        this->btAddressMap[addressString] = displayString;
    }
    void removeBTAddress(const char* address) {
        String addressString(address);
        this->removeBTAddress(addressString);
    }
    void removeBTAddress(const String& addressString) {
        this->btAddressMap.erase(addressString);
    }
    bool containsBTAddress(const char* address) {
        String addressString(address);
        return this->containsBTAddress(addressString);
    }
    bool containsBTAddress(const String& addressString) {
        return this->btAddressMap.count(addressString);
    }
    String* getBTAddressDisplayString(const char* address) {
        String addressString(address);
        return this->getBTAddressDisplayString(addressString);
    }
    String* getBTAddressDisplayString(const String& addressString) {
        if (this->btAddressMap.count(addressString)) {
            return &(this->btAddressMap[addressString]);
        }
        return NULL;
    }
    int countBTAddresses() {
        return this->btAddressMap.size();
    }
};

#endif