#include "BaseNumericMonitorMedium.h"

#include <LvglThemes/lv_theme_binary.h>

BaseNumericMonitorMedium::BaseNumericMonitorMedium(BikeStateAttributeIndex bikeStateAttributeIndex, MonitorAttributeType monitorAttributeType, const char* attributeTitle, const char* attributeUnits, const char* printFormat) {
	this->bikeStateAttributeIndex = bikeStateAttributeIndex;
	this->monitorAttributeType = monitorAttributeType;
	this->attributeTitle = attributeTitle;
	this->attributeUnits = (attributeUnits != NULL ? attributeUnits : "");
	this->printFormat = printFormat;
}

lv_obj_t* BaseNumericMonitorMedium::createLvObj(lv_obj_t* parent) {
	// get the style we'll need for the bar
	theme_binary_styles_t* binary_styles = (theme_binary_styles_t*)lv_disp_get_theme(lv_obj_get_disp(parent))->user_data;
	lv_style_t* no_scrollbar = &(binary_styles->no_scrollbar);
	lv_style_t* card_style = &(binary_styles->card);

	lv_obj_update_layout(parent);
	this->this_obj = lv_obj_create(parent);
	//lv_obj_set_size(this->this_obj, lv_obj_get_width(parent), lv_obj_get_height(parent));
	lv_obj_set_size(this->this_obj, lv_obj_get_width(parent), lv_obj_get_height(parent));
	lv_obj_set_align(this->this_obj, LV_ALIGN_CENTER);

	if (this->attributeUnits) {
		this->label_obj = lv_label_create(this->this_obj);
		lv_obj_align(this->label_obj, LV_ALIGN_CENTER, 0, 12);
		lv_label_set_text(this->label_obj, this->attributeUnits);
	}

	this->value_obj = lv_label_create(this->this_obj);
	//lv_obj_set_style_bg_color(this->value_obj, lv_color_black(), LV_PART_MAIN);
	//lv_obj_set_style_bg_opa(this->value_obj, LV_OPA_COVER, LV_PART_MAIN);
	// If there is no title place in the centre
	lv_obj_align(this->value_obj, LV_ALIGN_CENTER, 0, -7);
	lv_label_set_text(this->value_obj, "");
	lv_obj_set_style_text_font(this->value_obj, &lv_font_montserrat_26, LV_PART_MAIN);

	return this->this_obj;
}

void BaseNumericMonitorMedium::statusUpdate()
{
	BikeStateAttribute& bikeStateAttribute = this->bluetoothBikeController->getBikeState().getStateAttribute(this->bikeStateAttributeIndex);

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

void BaseNumericMonitorMedium::updateLvObj()
{
	char valueString[32];
	BikeStateAttribute& bikeStateAttribute = this->bluetoothBikeController->getBikeState().getStateAttribute(this->bikeStateAttributeIndex);

	// If not defined initalise the print formatting for this attribute
	if (this->printFormat == NULL) {
		switch (bikeStateAttribute.bikeStateAttributeType) {
		case BikeStateAttributeType::UINT32_T:
			printFormat = "%ld";
			break;
		case BikeStateAttributeType::UINT16_T:
			printFormat = "%d";
			break;
		case BikeStateAttributeType::UINT8_T:
			printFormat = "%d";
			break;
		case BikeStateAttributeType::FLOAT_T:
			printFormat = "%.1f";
			break;
		default:
			printFormat = "";
			break;
		}
	}

	// Convert the value into a string
	switch (bikeStateAttribute.bikeStateAttributeType) {
	case BikeStateAttributeType::UINT32_T:
		sprintf(valueString, printFormat, bikeStateAttribute.bikeStateAttributeValue.valueUint32);
		this->previousBikeStateAttribute.valueUint32 = bikeStateAttribute.bikeStateAttributeValue.valueUint32;
		break;
	case BikeStateAttributeType::UINT16_T:
		sprintf(valueString, printFormat, bikeStateAttribute.bikeStateAttributeValue.valueUint16);
		this->previousBikeStateAttribute.valueUint16 = bikeStateAttribute.bikeStateAttributeValue.valueUint16;
		break;
	case BikeStateAttributeType::UINT8_T:
		sprintf(valueString, printFormat, bikeStateAttribute.bikeStateAttributeValue.valueUint8);
		this->previousBikeStateAttribute.valueUint8 = bikeStateAttribute.bikeStateAttributeValue.valueUint8;
		break;
	case BikeStateAttributeType::FLOAT_T:
		sprintf(valueString, printFormat, bikeStateAttribute.bikeStateAttributeValue.valueFloat);
		this->previousBikeStateAttribute.valueFloat = bikeStateAttribute.bikeStateAttributeValue.valueFloat;
		break;
	}
	lv_label_set_text(this->value_obj, valueString);
	lv_label_set_text(this->label_obj, this->attributeUnits);
}

void BaseNumericMonitorMedium::initBluetoothStats()
{
	this->bluetoothBikeController->getConnectedBluetoothBike().readBikeStateAttribute(this->bikeStateAttributeIndex, this->monitorAttributeType);
}

void BaseNumericMonitorMedium::focusLvObj(BaseLvObject* defocusLvObj)
{
	// Ensure this item's state is in focus at this point
	MonitorLvObject::focusLvObj(defocusLvObj);
}