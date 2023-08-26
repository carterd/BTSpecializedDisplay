#include <math.h>
#include "SpeedGraphMonitorMain.h"
#include "../../../themes/display_theme.h"

#define SECONDS_PER_HOUR 3600
#define SECONDS_PER_X_TICK 600
#define MAX_GRAPH_LINES 60
#define MAX_AXIS_TICKS_X 7
#define MAX_AXIS_TICKS_Y 15

SpeedGraphMonitorMain::SpeedGraphMonitorMain(ConfigStore* configStore, SpeedMeterLogger* speedMeterLogger, bool minMaxMode, int16_t graphMinLimit, int16_t graphMaxLimit) :
    speedGraphMonitorIterator(speedMeterLogger),
    graphMonitor(
        { 0,0 }, MAX_GRAPH_LINES, AxisType::XaxisBottomYaxisRight, 60, 600, MAX_AXIS_TICKS_X, MAX_AXIS_TICKS_Y, graphMinLimit, graphMaxLimit
    ) {
    this->configStore = configStore;
    this->speedMeterLogger = speedMeterLogger;
    this->minMaxMode = minMaxMode;
    this->LoggerPeriodStartTimeTicks = 0;
    this->currentMultipliedWheelRotationsPerMin = 0;
}

lv_obj_t* SpeedGraphMonitorMain::createLvObj(lv_obj_t* parent) {
    BaseMonitorMain::createLvObj(parent);

    display_theme_styles_t* display_theme_styles = (display_theme_styles_t*)lv_disp_get_theme(lv_obj_get_disp(parent))->user_data;
    lv_style_t* graph_line_style = &(display_theme_styles->main_battery_outline);
    lv_style_t* main_graph_style = &(display_theme_styles->main_graph_panel);
    lv_style_t* current_graph_style = &(display_theme_styles->current_graph_panel);
    lv_style_t* main_graph_axis_style = &(display_theme_styles->main_graph_axis);
    lv_point_t* main_graph_axis_large_ticks = &(display_theme_styles->main_graph_axis_large_ticks);
    lv_point_t* main_graph_axis_small_ticks = &(display_theme_styles->main_graph_axis_small_ticks);

    this->graphMonitor.setGraphLineStyles(main_graph_style, current_graph_style, main_graph_axis_style, main_graph_axis_large_ticks, main_graph_axis_small_ticks);
    this->graphMonitor.createLvObj(this->this_obj);
    this->graphMonitor.setXLimits(-SECONDS_PER_HOUR, 0);
    this->graphMonitor.setXTickSizes(SECONDS_PER_X_TICK, 0);
    this->graphMonitor.setCurrentVisiblitiy(true);
    
    return this->this_obj;
}

void SpeedGraphMonitorMain::statusUpdate()
{
    // Update the speedMeterLogger
    this->speedMeterLogger->addReading(this->bluetoothBike->getBikeState().getStateAttribute(BikeStateAttributeIndex::WHEEL_ROTATIONS_PER_MIN).bikeStateAttributeValue.valueFloat, this->bluetoothBike->getBikeState().getStateAttribute(BikeStateAttributeIndex::WHEEL_ROTATIONS_PER_MIN).lastFetchTimeTicks);
    
    // Update the screen
    MonitorLvObject::statusUpdate();
}

void SpeedGraphMonitorMain::initBluetoothStats()
{
    this->bluetoothBike->readBikeStateAttribute(BikeStateAttributeIndex::WHEEL_ROTATIONS_PER_MIN, MonitorAttributeType::EVERY_SECOND);
    this->bluetoothBike->readBikeStateAttribute(BikeStateAttributeIndex::WHEEL_CIRCUMFERENCE, MonitorAttributeType::ONCE);
    BikeConfig bikeConfig = this->configStore->getBikeConfig();
    DisplayConfig displayConfig = this->configStore->getDisplayConfig();
    this->speedMeterLogger->setWheelCircumference((bikeConfig.wheelCircumference.managed) ? bikeConfig.wheelCircumference.value : this->bluetoothBike->getBikeState().getStateAttribute(BikeStateAttributeIndex::WHEEL_CIRCUMFERENCE).bikeStateAttributeValue.valueUint16);
    this->bluetoothBike->readBikeStateAttribute(BikeStateAttributeIndex::WHEEL_ROTATIONS, MonitorAttributeType::EVERY_TEN_SECONDS);

    this->updateGraphMax();
    this->adjustGraphYTicks();
}

void SpeedGraphMonitorMain::updateLvObj() {
    // If there is currently no bike the update can be ignored
    if (!this->bluetoothBike) return;

    // Only if the period has changed do we need to perform an update on the display due to period stats
    if (this->speedMeterLogger->getPeriodStartTimeTicks() != this->LoggerPeriodStartTimeTicks) {
        this->LoggerPeriodStartTimeTicks = this->speedMeterLogger->getPeriodStartTimeTicks();

        // The graph needs to be redrawn because there's more stats
        this->updateGraph();
    }

    if (this->currentWheelRotationsPerMin != this->bluetoothBike->getBikeState().getStateAttribute(BikeStateAttributeIndex::WHEEL_ROTATIONS_PER_MIN).bikeStateAttributeValue.valueFloat) {
        this->currentWheelRotationsPerMin = this->bluetoothBike->getBikeState().getStateAttribute(BikeStateAttributeIndex::WHEEL_ROTATIONS_PER_MIN).bikeStateAttributeValue.valueFloat;
        this->currentMultipliedWheelRotationsPerMin = this->speedMeterLogger->convertWheelRotationsPerMinToMultipliedWheelRotations(this->currentWheelRotationsPerMin);
        if (this->currentMultipliedWheelRotationsPerMin > this->graphMonitor.getYMax()) {
            updateGraphMax();
        }
        // Current changed identify current needs update
        this->updateCurrent();
    }

    this->graphMonitor.updateLvObj();
}

void SpeedGraphMonitorMain::updateCurrent() {
    this->graphMonitor.updateCurrent(this->currentMultipliedWheelRotationsPerMin);
}

void SpeedGraphMonitorMain::updateGraph() {
    this->graphMonitor.updatePlot(&this->speedGraphMonitorIterator);

    this->maxGraphPlotYaxis = speedGraphMonitorIterator.maxGraphPlotYaxis;

    this->graphMonitor.setAxisXPos(speedGraphMonitorIterator.mintuesOffset);
    this->graphMonitor.setXLimits(speedGraphMonitorIterator.mintuesOffset - SECONDS_PER_HOUR, speedGraphMonitorIterator.mintuesOffset);

    // Ensure graph max is correct
    this->updateGraphMax();
}

void SpeedGraphMonitorMain::updateGraphMax() {

    this->adjustCurrentMaxMultipliedWheelRotationsPerMin(this->currentMultipliedWheelRotationsPerMin);
    this->adjustCurrentMaxMultipliedWheelRotationsPerMin(this->maxGraphPlotYaxis);
}

void SpeedGraphMonitorMain::adjustCurrentMaxMultipliedWheelRotationsPerMin(float compareValue) {
    int16_t currentYMax = this->graphMonitor.getYMax();
    // Now we're dealing in speed we need the wheel circumf
    if (currentYMax < compareValue) {
        DisplayConfig displayConfig = this->configStore->getDisplayConfig();
        if (displayConfig.unitsMetric) {
            float newKmph = 5 * (std::floor(this->speedMeterLogger->convertMultipliedWheelRotationsPerMinToKmph(compareValue) / 5) + 1);
            currentYMax = this->speedMeterLogger->convertKmphToMultipliedWheelRotationsPerMin(newKmph);
        }
        else {
            float newMph = (5 / KMPH_TO_MPH_MULTIPLIER) * (std::floor(this->speedMeterLogger->convertMultipliedWheelRotationsPerMinToKmph(compareValue) / (5.0 / KMPH_TO_MPH_MULTIPLIER)) + 1);
            currentYMax = this->speedMeterLogger->convertKmphToMultipliedWheelRotationsPerMin(newMph);
        }
        this->graphMonitor.setYLimits(0, currentYMax);
        this->adjustGraphYTicks();
    }
}

void SpeedGraphMonitorMain::adjustGraphYTicks() {
    // Now we're dealing in speed we need the wheel circumf
    DisplayConfig displayConfig = this->configStore->getDisplayConfig();
    float tickSize = (displayConfig.unitsMetric) ? this->speedMeterLogger->convertKmphToMultipliedWheelRotationsPerMin(5.0f) : this->speedMeterLogger->convertKmphToMultipliedWheelRotationsPerMin(5.0f / KMPH_TO_MPH_MULTIPLIER);
    this->graphMonitor.setYTickSizes(tickSize, 2);
}

