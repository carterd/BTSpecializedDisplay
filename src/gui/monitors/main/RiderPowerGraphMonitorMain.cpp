#include "RiderPowerGraphMonitorMain.h"
#include "../../../themes/display_theme.h"

#define SECONDS_PER_HOUR 3600
#define SECONDS_PER_MIN 60
#define SECONDS_PER_X_TICK 600
#define MAX_GRAPH_LINES 60
#define MAX_AXIS_TICKS_X 7
#define MAX_AXIS_TICKS_Y 15
#define MILLI_PER_SECOND 1000

RiderPowerGraphMonitorMain::RiderPowerGraphMonitorMain(PowerMeterLogger* powerMeterLogger, bool minMaxMode, int16_t graphPowerMinLimit, int16_t graphPowerMaxLimit) :
    riderPowerMonitorIterator(powerMeterLogger),
    graphMonitor({ 0,0 }, MAX_GRAPH_LINES, AxisType::XaxisBottomYaxisRight, 60, 600, MAX_AXIS_TICKS_X, MAX_AXIS_TICKS_Y, graphPowerMinLimit, graphPowerMaxLimit
    ) {
    this->powerMeterLogger = powerMeterLogger;
    this->minMaxMode = minMaxMode;
    this->LoggerPeriodStartTimeTicks = 0;
    this->currentPower = 0;
}

lv_obj_t* RiderPowerGraphMonitorMain::createLvObj(lv_obj_t* parent) {
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

void RiderPowerGraphMonitorMain::statusUpdate()
{
    // Update the powerMeterLogger
    this->powerMeterLogger->addReading(this->bluetoothBike->getBikeState().getStateAttribute(BikeStateAttributeIndex::RIDER_POWER).bikeStateAttributeValue.valueUint16, this->bluetoothBike->getBikeState().getStateAttribute(BikeStateAttributeIndex::RIDER_POWER).lastFetchTimeTicks);
    // Update the screen
    MonitorLvObject::statusUpdate();
}

void RiderPowerGraphMonitorMain::initBluetoothStats()
{
	this->bluetoothBike->readBikeStateAttribute(BikeStateAttributeIndex::RIDER_POWER, MonitorAttributeType::EVERY_SECOND);
}

void RiderPowerGraphMonitorMain::updateLvObj() {
    // Only if the period has changed do we need to perform an update on the display due to period stats
    if (this->powerMeterLogger->getPeriodStartTimeTicks() != this->LoggerPeriodStartTimeTicks) {
        this->LoggerPeriodStartTimeTicks = this->powerMeterLogger->getPeriodStartTimeTicks();

        // The graph needs to be redrawn because there's more stats
        this->updateGraph();
    }

    if (this->currentPower != this->bluetoothBike->getBikeState().getStateAttribute(BikeStateAttributeIndex::RIDER_POWER).bikeStateAttributeValue.valueUint16) {
        this->currentPower = this->bluetoothBike->getBikeState().getStateAttribute(BikeStateAttributeIndex::RIDER_POWER).bikeStateAttributeValue.valueUint16;
        if (this->currentPower > this->graphMonitor.getYMax()) {
            updateGraphMax();
        }
        // Current changed identify current needs update
        this->updateCurrent();
    }

    this->graphMonitor.updateLvObj();
}

void RiderPowerGraphMonitorMain::updateCurrent() {
    this->graphMonitor.updateCurrent(this->currentPower);
}

void RiderPowerGraphMonitorMain::updateGraph() {
    this->graphMonitor.updatePlot(&this->riderPowerMonitorIterator);
    this->maxGraphPlotYaxis = riderPowerMonitorIterator.maxGraphPlotYaxis;

    this->graphMonitor.setAxisXPos(riderPowerMonitorIterator.mintuesOffset);
    this->graphMonitor.setXLimits(riderPowerMonitorIterator.mintuesOffset - SECONDS_PER_HOUR, riderPowerMonitorIterator.mintuesOffset);

    // Ensure graph max is correct
    this->updateGraphMax();

    // Ensure graph max is correct
    this->updateGraphMax();

}

void RiderPowerGraphMonitorMain::updateGraphMax()
{
    this->adjustCurrentMaxPower(this->currentPower);
    this->adjustCurrentMaxPower(this->maxGraphPlotYaxis);
}

void RiderPowerGraphMonitorMain::adjustCurrentMaxPower(float compareValue)
{
    int16_t currentYMax = this->graphMonitor.getYMax();
    // Now we're dealing in speed we need the wheel circumf
    if (currentYMax < compareValue) {

        float newPower = 10 * (std::floor(compareValue / 10) + 1);
        this->graphMonitor.setYLimits(0, newPower);
        this->adjustGraphYTicks();
    }
}

void RiderPowerGraphMonitorMain::adjustGraphYTicks() {
    // Now we're dealing in speed we need the wheel circumf
    this->graphMonitor.setYTickSizes(50, 2);
}
