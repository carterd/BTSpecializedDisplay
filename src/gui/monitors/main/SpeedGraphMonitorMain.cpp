#include "SpeedGraphMonitorMain.h"
#include "../../../themes/lv_theme.h"

static lv_style_t style_line(lv_coord_t width, bool white = true) {
    lv_style_t style_line;
    lv_style_init(&style_line);
    lv_style_set_line_width(&style_line, width);
    lv_style_set_line_color(&style_line, white ? lv_color_white() : lv_color_black());
    lv_style_set_line_rounded(&style_line, false);
    return style_line;
}

static lv_style_t style_line_dash(lv_coord_t width) {
    lv_style_t style_line;
    lv_style_init(&style_line);
    lv_style_set_line_width(&style_line, width);
    lv_style_set_line_color(&style_line, lv_color_white());
    lv_style_set_line_rounded(&style_line, false);
    lv_style_set_line_dash_gap(&style_line, 2);
    lv_style_set_line_dash_width(&style_line, 2);
    return style_line;
}

SpeedGraphMonitorMain::SpeedGraphMonitorMain(ConfigStore* configStore, SpeedMeterLogger* speedMeterLogger, bool minMaxMode, float graphSpeedMinLimit, float graphSpeedMaxLimit) {
    this->configStore = configStore;
    this->speedMeterLogger = speedMeterLogger;
    this->minMaxMode = minMaxMode;
    this->graphSpeedMinLimit = this->speedMeterLogger->convertKmph(graphSpeedMinLimit);
    this->graphSpeedMaxLimit = this->speedMeterLogger->convertKmph(graphSpeedMaxLimit);
    this->graphDisplayMultiplier = 0.0;
    this->speedMeterLoggerPeriodStartTimeTicks = 0;
    this->currentSpeed = 0;

    this->graphTicksLarge = this->speedMeterLogger->convertKmph(10);
    this->graphTicksSmall = this->graphTicksLarge / 2;

    this->graph_axis_line_style = style_line(1);
    this->graph_line_style = style_line(1);
    this->graph_dash_line_style = style_line_dash(1);
    this->graph_dash_line_background_style = style_line(1, false);
}

lv_obj_t* SpeedGraphMonitorMain::createLvObj(lv_obj_t* parent) {
    BaseMonitorMain::createLvObj(parent);
    
    this->leftAxisLabelsParent = lv_obj_create(this->this_obj);
    lv_obj_set_size(this->leftAxisLabelsParent, 4, GRAPH_HEIGHT);
    lv_obj_set_align(this->leftAxisLabelsParent, LV_ALIGN_RIGHT_MID);

    //  Create a lineand apply the new style
    this->graphLinesParent = lv_line_create(this->this_obj);
    lv_obj_set_size(this->graphLinesParent, GRAPH_WIDTH, GRAPH_HEIGHT);
    lv_obj_set_align(this->graphLinesParent, LV_ALIGN_LEFT_MID);

    for (int i = 0; i < GRAPH_WIDTH; i++) {
        lv_obj_t* graphMinMaxLine = lv_line_create(this->graphLinesParent);
        lv_obj_add_style(graphMinMaxLine, &this->graph_line_style, LV_PART_MAIN);
        lv_line_set_points(graphMinMaxLine, this->graph_bar_line_points[i], 2);
    }

    lv_obj_t* currentLineParent = lv_line_create(this->this_obj);
    lv_obj_set_size(currentLineParent, lv_obj_get_width(parent), GRAPH_HEIGHT);
    lv_obj_set_align(currentLineParent, LV_ALIGN_LEFT_MID);
    this->currentBackgroundLine = lv_line_create(currentLineParent);
    lv_obj_add_style(this->currentBackgroundLine, &this->graph_dash_line_background_style, LV_PART_MAIN);
    this->currentLine = lv_line_create(currentLineParent);
    lv_obj_add_style(this->currentLine, &this->graph_dash_line_style, LV_PART_MAIN);

    this->updateMultipler(0);
    this->updateAxisLabels();
    this->updateGraph();
    this->updateCurrent();

    return this->this_obj;
}

void SpeedGraphMonitorMain::statusUpdate()
{
    // Update the speedMeterLogger
    this->speedMeterLogger->addReading(this->bluetoothBike->getBikeState().getStateAttribute(BikeStateAttributeIndex::WHEEL_ROTATIONS_PER_MIN).bikeStateAttributeValue.valueFloat, this->bluetoothBike->getBikeState().getStateAttribute(BikeStateAttributeIndex::RIDER_POWER).lastFetchTimeTicks);
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

    this->graphTicksLarge = displayConfig.unitsMetric ? this->speedMeterLogger->convertKmph(10.0) : this->speedMeterLogger->convertKmph(16.0934);
    this->graphTicksSmall = this->graphTicksLarge / 2;
}

void SpeedGraphMonitorMain::updateLvObj() {
    bool updateGraph = false;
    bool updateCurrent = false;
    bool updateAxisLabels = false;
    uint16_t oldGraphDisplayMultiplier = this->graphDisplayMultiplier;
    // Only if the period has changed do we need to perform an update on the display due to period stats
    if (this->speedMeterLogger->getPeriodStartTimeTicks() != this->speedMeterLoggerPeriodStartTimeTicks) {
        uint32_t previousStartTimeTicks = this->speedMeterLoggerPeriodStartTimeTicks;
        this->speedMeterLoggerPeriodStartTimeTicks = this->speedMeterLogger->getPeriodStartTimeTicks();

        // Attempt to recalculate the display axis
        uint16_t loggerMaxReading = this->speedMeterLogger->getMaxPeriodReading(GRAPH_WIDTH * this->speedMeterLogger->getPeriodLengthTimeTicks());
        this->updateMultipler(loggerMaxReading);

        // The graph needs to be redrawn because there's more stats
        updateGraph = true;
    }

    if (this->currentWheelRotationsPerMin != this->bluetoothBike->getBikeState().getStateAttribute(BikeStateAttributeIndex::WHEEL_ROTATIONS_PER_MIN).bikeStateAttributeValue.valueFloat) {
        this->currentWheelRotationsPerMin = this->bluetoothBike->getBikeState().getStateAttribute(BikeStateAttributeIndex::WHEEL_ROTATIONS_PER_MIN).bikeStateAttributeValue.valueFloat;
        this->currentSpeed = this->speedMeterLogger->convert(this->currentWheelRotationsPerMin);
        if (this->currentSpeed > this->graphDisplayMaxSpeed) {
            this->updateMultipler(this->currentSpeed);
        }
        // Current changed identify current needs update
        updateCurrent = true;
    }

    if (oldGraphDisplayMultiplier != this->graphDisplayMultiplier) {
        // If we identify that multipler is different re-draw everything
        updateGraph = updateCurrent = updateAxisLabels = true;
    }

    if (updateGraph) this->updateGraph();
    if (updateCurrent) this->updateCurrent();
    if (updateAxisLabels) this->updateAxisLabels();
}

void SpeedGraphMonitorMain::updateAxisLabels() {
    int smallTicks = 1;
    int maxAxisStep = this->graphTicksSmall;
    uint16_t graphDisplayMax = GRAPH_HEIGHT / this->graphDisplayMultiplier;
    // count the number of ticks including zero tick
    int maxAxisLabels = (graphDisplayMax / (maxAxisStep)) + 1;
    if (maxAxisLabels > GRAPH_MAX_NO_AXIS_TICKS) {
        smallTicks = 0;
        maxAxisStep = this->graphTicksLarge;
        // count the number of ticks including zero tick
        maxAxisLabels = (graphDisplayMax / (maxAxisStep)) + 1;
    }
    // Restrict the number of ticks to the max
    if (maxAxisLabels > GRAPH_MAX_NO_AXIS_TICKS) maxAxisLabels = GRAPH_MAX_NO_AXIS_TICKS;
    int existingLabelCount = lv_obj_get_child_cnt(this->leftAxisLabelsParent);

    // Zero tick is graph bottom
    for (int i = 0; i < maxAxisLabels || i < existingLabelCount; i++) {
        // Need to create the label
        if (i < maxAxisLabels) {
            // Need to use existing label or create more
            lv_obj_t* axisLine;
            if (existingLabelCount <= i) {
                axisLine = lv_line_create(this->leftAxisLabelsParent);
                lv_obj_add_style(axisLine, &this->graph_axis_line_style, LV_PART_MAIN);
            }
            else {
                axisLine = lv_obj_get_child(this->leftAxisLabelsParent, i);
            }
            uint16_t yTickPos = this->graphDisplayMultiplier * i * maxAxisStep;
            this->graph_axis_line_points[i][0] = { (lv_coord_t)(0),                      (lv_coord_t)(GRAPH_BOTTOM - yTickPos) };
            this->graph_axis_line_points[i][1] = { (lv_coord_t)(i & smallTicks ? 1 : 4), (lv_coord_t)(GRAPH_BOTTOM - yTickPos) };
            lv_line_set_points(axisLine, this->graph_axis_line_points[i], 2);
        }
        else
        {
            // Removal of too many ticks
            lv_obj_del(lv_obj_get_child(this->leftAxisLabelsParent, -1));
        }
    }
}

void SpeedGraphMonitorMain::updateGraph() {
    std::deque<SpeedMeterLogger::PeriodReading>* periodReadings = this->speedMeterLogger->getPeriodReadings();
    uint32_t periodGraphStartTimeTicks = this->speedMeterLogger->getPeriodStartTimeTicks();
    uint32_t periodGraphLengthTimeTicks = this->speedMeterLogger->getPeriodLengthTimeTicks();
    auto it = periodReadings->crbegin();
    lv_coord_t prevAvCoord = (it != periodReadings->crend()) ? (*it).average * this->graphDisplayMultiplier : 0;
    lv_coord_t max = 0;
    lv_coord_t min = 0;
    for (int i = GRAPH_WIDTH - 1; i >= 0; i--) {
        lv_obj_t* graph_line_obj = lv_obj_get_child(this->graphLinesParent, i);
        if (it != periodReadings->crend()) {
            // Keep calculating graph positions
            int readingStartTimeTicks = (*it).periodStartTimeTicks;
            int periodGraphEndTimeTicks = periodGraphStartTimeTicks;
            periodGraphStartTimeTicks -= periodGraphLengthTimeTicks;
            if (readingStartTimeTicks >= periodGraphStartTimeTicks && readingStartTimeTicks < periodGraphEndTimeTicks) {
                if (this->minMaxMode) {
                    max = ((*it).max * this->graphDisplayMultiplier);
                    min = ((*it).min * this->graphDisplayMultiplier);
                }
                else {
                    lv_coord_t newAvCoord = (*it).average * this->graphDisplayMultiplier;
                    max = min = newAvCoord;
                    if (prevAvCoord > newAvCoord) max = prevAvCoord - 1;
                    if (prevAvCoord < newAvCoord) min = prevAvCoord + 1;
                    prevAvCoord = newAvCoord;
                }
                if (max >= GRAPH_BOTTOM) max = GRAPH_BOTTOM;
                if (min >= GRAPH_BOTTOM) min = GRAPH_BOTTOM;
                ++it;
            }
        }
        else {
            max = min = GRAPH_HEIGHT;
        }
        this->graph_bar_line_points[i][0] = { (lv_coord_t)(i), (lv_coord_t)(GRAPH_BOTTOM - max) };
        this->graph_bar_line_points[i][1] = { (lv_coord_t)(i), (lv_coord_t)(GRAPH_BOTTOM + 1 - min) };
        lv_line_set_points(graph_line_obj, this->graph_bar_line_points[i], 2);
    }
}

void SpeedGraphMonitorMain::updateCurrent() {
    lv_coord_t current = this->currentSpeed * this->graphDisplayMultiplier;
    if (current >= GRAPH_HEIGHT) current = GRAPH_BOTTOM;

    this->graph_current_line_points[0] = { 0, (lv_coord_t)(GRAPH_BOTTOM - current) };
    this->graph_current_line_points[1] = { 63, (lv_coord_t)(GRAPH_BOTTOM - current) };
    lv_line_set_points(this->currentBackgroundLine, this->graph_current_line_points, 2);
    lv_line_set_points(this->currentLine, this->graph_current_line_points, 2);
}

void SpeedGraphMonitorMain::updateMultipler(uint16_t maxReading) {
    // Find a max for the display as a multiple of small ticks
    uint16_t newGraphDisplayMax = (((maxReading / this->graphTicksSmall) + 1) * this->graphTicksSmall) + (this->graphTicksSmall / 2);
    // Limit the graphDisplay Max
    if (newGraphDisplayMax > this->graphSpeedMaxLimit) newGraphDisplayMax = this->graphSpeedMaxLimit;
    if (newGraphDisplayMax < this->graphSpeedMinLimit) newGraphDisplayMax = this->graphSpeedMinLimit;
    // update the 
    this->graphDisplayMultiplier = (newGraphDisplayMax > 0 ? ((float)GRAPH_HEIGHT) / newGraphDisplayMax : 0.0);
    this->graphDisplayMaxSpeed = GRAPH_HEIGHT / this->graphDisplayMultiplier;
}
