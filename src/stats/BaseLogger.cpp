#include "BaseLogger.h"

template <typename T>
BaseLogger<T>::BaseLogger(uint32_t startTimeTicks, uint32_t periodLengthTimeTicks, int maxPeriodReadings)
{
	this->periodStartTimeTicks = startTimeTicks;
	this->periodLengthTimeTicks = periodLengthTimeTicks;
	this->maxPeriodReadings = maxPeriodReadings;
}

template <typename T>
void BaseLogger<T>::addReading(T reading, uint32_t lastFetchTimeTicks)
{
	if (lastFetchTimeTicks <= this->lastFetchTimeTicks) return;
	
    // If we have entered a new period process the old period
	if (lastFetchTimeTicks > this->periodStartTimeTicks + this->periodLengthTimeTicks) {
		this->processPeriodRead();
		this->meterReadings.clear();
        this->periodStartTimeTicks = lastFetchTimeTicks - (lastFetchTimeTicks % this->periodLengthTimeTicks);
	}
    // Record a new meter reading
	this->lastFetchTimeTicks = lastFetchTimeTicks;
	MeterReading meterReading;
	meterReading.value = reading;
	meterReading.lastFetchTimeTicks = lastFetchTimeTicks;
	meterReadings.push_back(meterReading);
}

template <typename T>
void BaseLogger<T>::initLogger(uint32_t startTimeTicks)
{
	this->meterReadings.clear();
	this->periodReadings.clear();
	this->periodStartTimeTicks = startTimeTicks;
}

template <typename T>
void BaseLogger<T>::processPeriodRead()
{
	int meterReadingsSize = this->meterReadings.size();
	if (meterReadingsSize) {
		PeriodReading periodReading;
		periodReading.min = this->meterReadings[0].value;
		periodReading.max = periodReading.min;
		float average = 0;
		uint32_t previousFetchTimeTicks = this->periodStartTimeTicks;
		for (int i = 0; i < meterReadingsSize; i++) {
			uint16_t value = this->meterReadings[i].value;
			uint32_t fetchTimeTicks = this->meterReadings[i].lastFetchTimeTicks;
			if (value > periodReading.max) periodReading.max = value;
			else if (value < periodReading.min) periodReading.min = value;
			average += value * (fetchTimeTicks - previousFetchTimeTicks);
			previousFetchTimeTicks = fetchTimeTicks;
		}
		periodReading.average = average / ( previousFetchTimeTicks - this->periodStartTimeTicks);
		periodReading.periodStartTimeTicks = this->periodStartTimeTicks;
		this->addPeriodReading(periodReading);
	}
}

template <typename T>
void BaseLogger<T>::addPeriodReading(PeriodReading& periodReading)
{
	if (this->periodReadings.size() >= this->maxPeriodReadings)
		this->periodReadings.pop_front();
	this->periodReadings.push_back(periodReading);
}

template <typename T>
T BaseLogger<T>::getMaxPeriodReading(uint32_t windowLength) {
    return this->getMaxPeriodReading(this->periodStartTimeTicks - windowLength, this->periodStartTimeTicks);
}

template <typename T>
bool BaseLogger<T>::checkWindowStartEnd(uint32_t windowStart, uint32_t windowEnd)
{
	// Ensure that the reading will be within dataset otherwise return 0
	if (this->periodReadings.size() == 0 ||
		this->periodReadings[0].periodStartTimeTicks > windowEnd ||
		this->periodReadings[this->periodReadings.size()-1].periodStartTimeTicks < windowStart) {
		return false;
	}
	return true;
}

template <typename T>
T BaseLogger<T>::getLimitPeriodReading(uint32_t windowStart, uint32_t windowEnd, bool getMax)
{
	if (!this->checkWindowStartEnd(windowStart, windowEnd)) return 0;
	bool startScan = false;
	uint16_t limitPeriodReading = (getMax ? 0 : UINT16_MAX);
	for (auto it = this->periodReadings.crbegin(); it != this->periodReadings.crend(); ++it) {
		if (!startScan && (*it).periodStartTimeTicks < windowEnd) startScan = true;
		if (startScan) {
			if ((*it).periodStartTimeTicks < windowStart) break;
			if (getMax && (*it).max > limitPeriodReading) limitPeriodReading = (*it).max;
			if (!getMax && (*it).min < limitPeriodReading) limitPeriodReading = (*it).min;
		}
	}
	return limitPeriodReading;
}

template <typename T>
T BaseLogger<T>::getMaxPeriodReading(uint32_t windowStart, uint32_t windowEnd) {	
    return this->getLimitPeriodReading(windowStart, windowEnd, true);
}

template <typename T>
T BaseLogger<T>::getMinPeriodReading(uint32_t windowLength)
{
	return getMinPeriodReading(this->periodStartTimeTicks - windowLength, this->periodStartTimeTicks);
}

template <typename T>
T BaseLogger<T>::getMinPeriodReading(uint32_t windowStart, uint32_t windowEnd)
{
	return this->getLimitPeriodReading(windowStart, windowEnd, false);
}
