#include "MemoryMonitorSmall.h"
#include "../../../themes/display_theme.h"

#ifdef ARDUINO_ARCH_MBED
#include <mbed.h>
#include <mbed_stats.h>
#include <mbed_mem_trace.h>
#endif

#ifdef ARDUINO_ARCH_ESP32
#include <Esp.h>
#endif


MemoryMonitorSmall::MemoryMonitorSmall(const char* attributeTitle, const char* attributeUnits)
{
	this->attributeTitle = attributeTitle;
	this->attributeUnits = (attributeUnits != NULL ? attributeUnits : "");
}

lv_obj_t* MemoryMonitorSmall::createLvObj(lv_obj_t* parent)
{
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

void MemoryMonitorSmall::updateLvObj()
{
	uint32_t valueUint32 = 0;
#ifdef ARDUINO_ARCH_MBED
	// Grab the heap statistics
	mbed_stats_heap_t heap_stats;
	mbed_stats_heap_get(&heap_stats);
	valueUint32 = heap_stats.reserved_size - heap_stats.current_size;
#endif
#ifdef ARDUINO_ARCH_ESP32
  valueUint32 = ESP.getFreeHeap();
#endif

	char valueString[32];
	
	// Convert the value into a string
	sprintf(valueString, "%ld%s", valueUint32, attributeUnits);

	lv_label_set_text(this->value_obj, valueString);
}