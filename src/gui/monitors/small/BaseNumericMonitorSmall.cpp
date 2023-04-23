#include "BaseNumericMonitorSmall.h"
#include "../../../themes/lv_theme.h"

BaseNumericMonitorSmall::BaseNumericMonitorSmall(BikeStateAttributeIndex bikeStateAttributeIndex, MonitorAttributeType monitorAttributeType, const char* attributeTitle, const char* attributeUnits, const char* printFormat) {
	this->bikeStateAttributeIndex = bikeStateAttributeIndex;
	this->monitorAttributeType = monitorAttributeType;
	this->attributeTitle = attributeTitle;
	this->attributeUnits = (attributeUnits != NULL ? attributeUnits : "");
	this->printFormat = printFormat;
}

lv_obj_t* BaseNumericMonitorSmall::createLvObj(lv_obj_t* parent) {
	BaseMonitorSmall::createLvObj(parent);

	if (this->attributeTitle) {
		lv_obj_t* label_obj = lv_label_create(this->this_obj);
		lv_obj_set_align(label_obj, LV_ALIGN_LEFT_MID);
		lv_label_set_text(label_obj, this->attributeTitle);
	}

	this->value_obj = lv_label_create(this->this_obj);
	//lv_obj_set_style_bg_color(this->value_obj, lv_color_white(), LV_PART_MAIN);
	//lv_obj_set_style_bg_opa(this->value_obj, LV_OPA_COVER, LV_PART_MAIN);
	// If there is no title place in the centre
	lv_obj_set_align(this->value_obj, this->attributeTitle ? LV_ALIGN_RIGHT_MID : LV_ALIGN_CENTER);
	lv_label_set_text(this->value_obj, "");

	return this->this_obj;
}

void BaseNumericMonitorSmall::statusUpdate() 
{
	BikeStateAttribute& bikeStateAttribute = this->bluetoothBike->getBikeState().getStateAttribute(this->bikeStateAttributeIndex);

	char* previousLabel = lv_label_get_text(this->value_obj);
	if (*previousLabel != 0) {
		switch (bikeStateAttribute.bikeStateAttributeType) {
		case BikeStateAttributeType::UINT32_T:
			if (this->previousBikeStateAttribute.valueUint32 == bikeStateAttribute.bikeStateAttributeValue.valueUint32) return;
			break;
		case BikeStateAttributeType::UINT16_T:
			if (this->previousBikeStateAttribute.valueUint16 == bikeStateAttribute.bikeStateAttributeValue.valueUint16) return;
			break;
		case BikeStateAttributeType::UINT8_T:
			if (this->previousBikeStateAttribute.valueUint8 == bikeStateAttribute.bikeStateAttributeValue.valueUint8) return;
			break;
		case BikeStateAttributeType::FLOAT_T:
			if (this->previousBikeStateAttribute.valueFloat == bikeStateAttribute.bikeStateAttributeValue.valueFloat) return;
			break;
		}
	}

	MonitorLvObject::statusUpdate();
}

void BaseNumericMonitorSmall::updateLvObj()
{
	char valueString[32];
	BikeStateAttribute& bikeStateAttribute = this->bluetoothBike->getBikeState().getStateAttribute(this->bikeStateAttributeIndex);
	
	// If not defined initalise the print formatting for this attribute
	if (this->printFormat == NULL) {
		switch (bikeStateAttribute.bikeStateAttributeType) {
		case BikeStateAttributeType::UINT32_T:
			printFormat = "%ld%s";
			break;
		case BikeStateAttributeType::UINT16_T:
			printFormat = "%d%s";
			break;
		case BikeStateAttributeType::UINT8_T:
			printFormat = "%d%s";
			break;
		case BikeStateAttributeType::FLOAT_T:
			printFormat = "%.1f%s";
			break;
		default:
			printFormat = "";
			break;
		}
	}

	// Convert the value into a string
	switch (bikeStateAttribute.bikeStateAttributeType) {
	case BikeStateAttributeType::UINT32_T:
		sprintf(valueString, printFormat, bikeStateAttribute.bikeStateAttributeValue.valueUint32, attributeUnits);
		this->previousBikeStateAttribute.valueUint32 = bikeStateAttribute.bikeStateAttributeValue.valueUint32;
		break;
	case BikeStateAttributeType::UINT16_T:
		sprintf(valueString, printFormat, bikeStateAttribute.bikeStateAttributeValue.valueUint16, attributeUnits);
		this->previousBikeStateAttribute.valueUint16 = bikeStateAttribute.bikeStateAttributeValue.valueUint16;
		break;
	case BikeStateAttributeType::UINT8_T:
		sprintf(valueString, printFormat, bikeStateAttribute.bikeStateAttributeValue.valueUint8, attributeUnits);
		this->previousBikeStateAttribute.valueUint8 = bikeStateAttribute.bikeStateAttributeValue.valueUint8;
		break;
	case BikeStateAttributeType::FLOAT_T:
		sprintf(valueString, printFormat, bikeStateAttribute.bikeStateAttributeValue.valueFloat, attributeUnits);
		this->previousBikeStateAttribute.valueFloat = bikeStateAttribute.bikeStateAttributeValue.valueFloat;
		break;
	}
	lv_label_set_text(this->value_obj, valueString);
}

void BaseNumericMonitorSmall::initBluetoothStats()
{
	this->bluetoothBike->readBikeStateAttribute(this->bikeStateAttributeIndex, this->monitorAttributeType);
}

void BaseNumericMonitorSmall::focusLvObj(BaseLvObject* defocusLvObj)
{
	// Ensure this item's state is in focus at this point
	MonitorLvObject::focusLvObj(defocusLvObj);
}