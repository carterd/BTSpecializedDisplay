#include "MemoryMonitorSmall.h"

#include <LvglThemes/lv_theme_binary.h>

#ifndef ARDUINO_WIN_EMULATION
#include <mbed.h>
#include <mbed_stats.h>
#include <mbed_mem_trace.h>
#endif

/*
void MemoryMonitorSmall::updateLvObj()
{
	char ticksString[32];
	long totalSeconds = millis() / 1000.0;
	int seconds = totalSeconds % 60;
	long totalMinutes = (totalSeconds - seconds) / 60;
	int minutes = totalMinutes % 60;
	int hours = (totalMinutes - minutes) / 60;
	sprintf(ticksString, "%02d:%02d:%02d", hours, minutes, seconds);
	lv_label_set_text(this->value_obj, ticksString);
}
*/

void MemoryMonitorSmall::updateLvObj()
{
	uint32_t valueUint32 = 0;
#ifndef ARDUINO_WIN_EMULATION
	// Grab the heap statistics
	mbed_stats_heap_t heap_stats;
	mbed_stats_heap_get(&heap_stats);
	valueUint32 = heap_stats.reserved_size - heap_stats.current_size;
#endif
   	// No need to update GUI if value hasn't changed
	char* previousLabel = lv_label_get_text(this->value_obj);
	if (*previousLabel != 0 && this->previousBikeStateAttribute.valueUint32 == valueUint32) return;

	char valueString[32];
	
	// Convert the value into a string
	sprintf(valueString, "%ld%s", valueUint32, attributeUnits);
	this->previousBikeStateAttribute.valueUint32 = valueUint32;

	lv_label_set_text(this->value_obj, valueString);
}