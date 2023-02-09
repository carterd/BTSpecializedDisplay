#include "MemoryMonitorSmall.h"

#include <LvglThemes/lv_theme_binary.h>

#ifdef ARDUINO_ARCH_MBED
#include <mbed.h>
#include <mbed_stats.h>
#include <mbed_mem_trace.h>
#endif




MemoryMonitorSmall::MemoryMonitorSmall(const char* attributeTitle, const char* attributeUnits)
{
	this->attributeTitle = attributeTitle;
	this->attributeUnits = (attributeUnits != NULL ? attributeUnits : "");
}

lv_obj_t* MemoryMonitorSmall::createLvObj(lv_obj_t* parent)
{
	// get the style we'll need for the bar
	theme_binary_styles_t* binary_styles = (theme_binary_styles_t*)lv_disp_get_theme(lv_obj_get_disp(parent))->user_data;
	lv_style_t* no_scrollbar = &(binary_styles->no_scrollbar);
	lv_style_t* card_style = &(binary_styles->card);

	lv_obj_update_layout(parent);
	this->this_obj = lv_obj_create(parent);
	lv_obj_set_size(this->this_obj, lv_obj_get_width(parent), lv_obj_get_height(parent));
	lv_obj_set_align(this->this_obj, LV_ALIGN_CENTER);

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

	char valueString[32];
	
	// Convert the value into a string
	sprintf(valueString, "%ld%s", valueUint32, attributeUnits);

	lv_label_set_text(this->value_obj, valueString);
}