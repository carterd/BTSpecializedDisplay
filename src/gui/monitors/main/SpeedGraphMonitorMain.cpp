#include <math.h>
#include "SpeedGraphMonitorMain.h"
#include "../../../themes/display_theme.h"

#define MILLI_PER_MIN 60000
#define MAX_CURRENT_GRAPH_LINES 1
#define MAX_GRAPH_LINES 60
#define POINTS_PER_LINE 2
#define MAX_AXIS_TICKS_X 7
#define MAX_AXIS_TICKS_Y 15

SpeedGraphMonitorMain::SpeedGraphMonitorMain(ConfigStore* configStore, SpeedMeterLogger* speedMeterLogger, bool minMaxMode, float graphMinLimit, float graphMaxLimit) :
    plotGraph(MAX_GRAPH_LINES, POINTS_PER_LINE),
    currentGraph(MAX_CURRENT_GRAPH_LINES, POINTS_PER_LINE),
    graphAxis(&this->plotGraph, MAX_AXIS_TICKS_X, MAX_AXIS_TICKS_Y, AxisType::XaxisBottomYaxisRight) {
    this->configStore = configStore;
    this->speedMeterLogger = speedMeterLogger;
    this->minMaxMode = minMaxMode;
    this->graphMinLimit = graphMinLimit;
    this->graphMaxLimit = graphMaxLimit;
    this->speedMeterLoggerPeriodStartTimeTicks = 0;
    this->currentMultipliedWheelRotationsPerMin = 0;

    this->graphAxis.setXMinorTickIncrement(10 * 60 * 1000);
    this->graphAxis.setYMinorTickIncrement(1000);
}

lv_obj_t* SpeedGraphMonitorMain::createLvObj(lv_obj_t* parent) {
    BaseMonitorMain::createLvObj(parent);
    
    // get the style we'll need for the bar
    display_theme_styles_t* display_theme_styles = (display_theme_styles_t*)lv_disp_get_theme(lv_obj_get_disp(parent))->user_data;
    lv_style_t* graph_line_style = &(display_theme_styles->main_battery_outline);
    lv_style_t* main_graph_style = &(display_theme_styles->main_graph_panel);
    lv_style_t* current_graph_style = &(display_theme_styles->current_graph_panel);
    lv_style_t* main_graph_axis_style = &(display_theme_styles->main_graph_axis);
    lv_point_t* main_graph_axis_large_ticks = &(display_theme_styles->main_graph_axis_large_ticks);
    lv_point_t* main_graph_axis_small_ticks = &(display_theme_styles->main_graph_axis_small_ticks);

    //  Create a lineand apply the new style
    this->graphPlotParent = lv_obj_create(this->this_obj);
    lv_obj_add_style(this->graphPlotParent, main_graph_style, LV_PART_MAIN);
    lv_obj_set_align(this->graphPlotParent, LV_ALIGN_TOP_LEFT);
    lv_obj_update_layout(this->graphPlotParent);
    // Configure Plot graph
    this->plotGraph.setGraphLineStyle(main_graph_style);
    this->plotGraph.createLvObj(this->graphPlotParent);
    this->plotGraph.setLimits(-MILLI_PER_MIN*MAX_GRAPH_LINES, 0, 0, 100);

    // Create a current graph
    this->graphCurrentLineParent = lv_obj_create(this->this_obj);
    lv_obj_add_style(this->graphCurrentLineParent, current_graph_style, LV_PART_MAIN);
    lv_obj_set_align(this->graphCurrentLineParent, LV_ALIGN_TOP_LEFT);
    lv_obj_update_layout(this->graphCurrentLineParent);
    // Configure Current graph Line
    this->currentGraph.setGraphLineStyle(current_graph_style);
    this->currentGraph.createLvObj(this->graphCurrentLineParent);
    this->currentGraph.setLimits(0, 1000, 0, 100);
    this->currentGraph.setLineVisible(0);

    // Create the axis
    this->graphAxisParent = lv_obj_create(this->this_obj);
    //lv_obj_set_size(this->graphAxisParent, 4, GRAPH_HEIGHT);
    lv_obj_add_style(this->graphAxisParent, main_graph_axis_style, LV_PART_MAIN);
    lv_obj_set_align(this->graphAxisParent, LV_ALIGN_TOP_LEFT);
    lv_obj_update_layout(this->graphAxisParent);
    // Configure GraphAxis
    this->graphAxis.setAxisLineStyle(main_graph_axis_style, main_graph_axis_large_ticks, main_graph_axis_small_ticks);
    this->graphAxis.createLvObj(this->graphAxisParent);
    this->graphAxis.setAxisXPos(this->plotGraph.getXMax());
    this->graphAxis.setAxisYPos(0);
 
    // We can't fill all the graphs details until we've obtained a wheel size for calculation of speed
    this->redraw = true;

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
    this->graphAxis.updateLvObj();
}

void SpeedGraphMonitorMain::updateLvObj() {
    // If there is currently no bike the update can be ignored
    if (!this->bluetoothBike) return;

    bool updateGraph = this->redraw;
    bool updateCurrent = this->redraw;
    // Only if the period has changed do we need to perform an update on the display due to period stats
    if (this->speedMeterLogger->getPeriodStartTimeTicks() != this->speedMeterLoggerPeriodStartTimeTicks) {
        //uint32_t previousStartTimeTicks = this->speedMeterLoggerPeriodStartTimeTicks;
        this->speedMeterLoggerPeriodStartTimeTicks = this->speedMeterLogger->getPeriodStartTimeTicks();

        // The graph needs to be redrawn because there's more stats
        updateGraph = true;
    }

    if (this->currentWheelRotationsPerMin != this->bluetoothBike->getBikeState().getStateAttribute(BikeStateAttributeIndex::WHEEL_ROTATIONS_PER_MIN).bikeStateAttributeValue.valueFloat) {
        this->currentWheelRotationsPerMin = this->bluetoothBike->getBikeState().getStateAttribute(BikeStateAttributeIndex::WHEEL_ROTATIONS_PER_MIN).bikeStateAttributeValue.valueFloat;
        this->currentMultipliedWheelRotationsPerMin = this->speedMeterLogger->convertWheelRotationsPerMinToMultipliedWheelRotations(this->currentWheelRotationsPerMin);
        if (this->currentMultipliedWheelRotationsPerMin > this->currentMaxMultipliedWheelRotationsPerMin) {
            // Update of the graph will also catch that the graph max needs updating
            updateGraph = true;
        }
        // Current changed identify current needs update
        updateCurrent = true;
    }

    //if (oldGraphDisplayMultiplier != this->graphDisplayMultiplier) {
        // If we identify that multipler is different re-draw everything
    //    updateGraph = updateCurrent = updateAxisLabels = true;
    //}

    if (updateGraph) this->updateGraph();
    if (updateCurrent) this->updateCurrent();

    this->redraw = false;
}

void SpeedGraphMonitorMain::updateCurrent() {
    this->currentGraph.setGraphPoint(0, 0, (uint32_t) 0, this->currentMultipliedWheelRotationsPerMin);
    this->currentGraph.setGraphPoint(0, 1, (uint32_t) 1000, this->currentMultipliedWheelRotationsPerMin);
    this->currentGraph.updateLvObj();
}

void SpeedGraphMonitorMain::adjustCurrentMaxMultipliedWheelRotationsPerMin(float compareValue) {
    // Now we're dealing in speed we need the wheel circumf
    if (this->currentMaxMultipliedWheelRotationsPerMin < compareValue) {
        DisplayConfig displayConfig = this->configStore->getDisplayConfig();
        if (displayConfig.unitsMetric) {
            float newKmph = 5 * (std::floor(this->speedMeterLogger->convertMultipliedWheelRotationsPerMinToKmph(compareValue) / 5) + 1);
            this->currentMaxMultipliedWheelRotationsPerMin = this->speedMeterLogger->convertKmphToMultipliedWheelRotationsPerMin(newKmph);
        }
        else {
            float newMph = (5 / KMPH_TO_MPH_MULTIPLIER) * (std::floor(this->speedMeterLogger->convertMultipliedWheelRotationsPerMinToKmph(compareValue) / (5.0 / KMPH_TO_MPH_MULTIPLIER)) + 1);
            this->currentMaxMultipliedWheelRotationsPerMin = this->speedMeterLogger->convertKmphToMultipliedWheelRotationsPerMin(newMph);
        }
        if (this->currentMaxMultipliedWheelRotationsPerMin > this->speedMeterLogger->convertKmphToMultipliedWheelRotationsPerMin(this->graphMaxLimit)) {
            this->currentMaxMultipliedWheelRotationsPerMin = this->speedMeterLogger->convertKmphToMultipliedWheelRotationsPerMin(this->graphMaxLimit);
        }
    }
}

void SpeedGraphMonitorMain::adjustGraphYTicks() {
    // Now we're dealing in speed we need the wheel circumf
    DisplayConfig displayConfig = this->configStore->getDisplayConfig();
    if (displayConfig.unitsMetric) {
        int32_t kmphTicks = (int32_t)this->speedMeterLogger->convertKmphToMultipliedWheelRotationsPerMin(5.0f);
        this->graphAxis.setYMinorTickIncrement(kmphTicks);
        this->graphAxis.setYMajorTickIncrement(kmphTicks * 2);
    }
    else {
        int32_t mphTicks = (int32_t)this->speedMeterLogger->convertKmphToMultipliedWheelRotationsPerMin(5.0f / KMPH_TO_MPH_MULTIPLIER);
        this->graphAxis.setYMinorTickIncrement(mphTicks);
        this->graphAxis.setYMajorTickIncrement(mphTicks * 2);
    }
}

void SpeedGraphMonitorMain::updateGraphMax() {
    this->adjustCurrentMaxMultipliedWheelRotationsPerMin(this->currentMultipliedWheelRotationsPerMin);
    this->adjustCurrentMaxMultipliedWheelRotationsPerMin(this->maxAverageMultipliedWheelRotations);
    this->adjustCurrentMaxMultipliedWheelRotationsPerMin(this->speedMeterLogger->convertKmphToMultipliedWheelRotationsPerMin(this->graphMinLimit));

    this->plotGraph.setYmax(this->currentMaxMultipliedWheelRotationsPerMin);
    this->currentGraph.setYmax(this->currentMaxMultipliedWheelRotationsPerMin);

    // Now we're dealing in speed we need the wheel circumf
    adjustGraphYTicks();

    // Update the XAxis too
    this->graphAxis.setAxisXPos(this->plotGraph.getXMax());
}

void SpeedGraphMonitorMain::updateGraph() {
    std::deque<SpeedMeterLogger::PeriodReading>* periodReadings = this->speedMeterLogger->getPeriodReadings();
    uint32_t periodGraphLengthTimeTicks = this->speedMeterLogger->getPeriodLengthTimeTicks();
    uint32_t periodGraphStartTimeTicks = this->speedMeterLogger->getPeriodStartTimeTicks() - periodGraphLengthTimeTicks;
    this->plotGraph.setXmin(periodGraphStartTimeTicks - periodGraphLengthTimeTicks * MAX_GRAPH_LINES);
    this->plotGraph.setXmax(periodGraphStartTimeTicks);
    this->maxAverageMultipliedWheelRotations = 0.0;

    auto it = periodReadings->crbegin();
    if (it != periodReadings->crend()) {
        // Current time is close enough to the timings of logs
        uint32_t currentTime = millis();
        uint32_t startTimeTicks = (*it).periodStartTimeTicks;
        float startAvgCoord = (*it).average;
        uint16_t lineIndex = 0;
        do {
            uint32_t timeTicks;
            float avgCoord;
            it++;
            if (it != periodReadings->crend() && (*it).periodStartTimeTicks > startTimeTicks - this->speedMeterLogger->getPeriodLengthTimeTicks() * 1.5) {
                timeTicks = (*it).periodStartTimeTicks;
                avgCoord = (*it).average;
            } else {
                timeTicks = startTimeTicks;
                avgCoord = startAvgCoord;
            }

            this->plotGraph.setGraphPoint(lineIndex, 0, timeTicks, (uint32_t) avgCoord);
            this->plotGraph.setGraphPoint(lineIndex, 1, startTimeTicks, (uint32_t) startAvgCoord);
            this->plotGraph.setLineVisible(lineIndex);

            if (this->maxAverageMultipliedWheelRotations < avgCoord) { this->maxAverageMultipliedWheelRotations = avgCoord; }
            lineIndex++;
            startAvgCoord = avgCoord;
            startTimeTicks = timeTicks;
        } while (it != periodReadings->crend() && lineIndex < MAX_GRAPH_LINES);
        // Ensure graph max is correct
        this->updateGraphMax();
        // Update the graph now it's points are updated
        this->plotGraph.updateLvObj();
        this->graphAxis.updateLvObj();
    }
}
