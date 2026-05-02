/*
 * This file is part of INAV.
 *
 * INAV is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * INAV is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with INAV.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdbool.h>
#include <stdint.h>

#include <platform.h>

#include "common/axis.h"

#include "config/config_master.h"
#include "config/feature.h"

#include "drivers/sensor.h"
#include "drivers/pwm_esc_detect.h"
#include "drivers/pwm_output.h"
#include "drivers/pwm_mapping.h"
#include "drivers/serial.h"

#include "fc/rc_controls.h"

#include "flight/failsafe.h"
#include "flight/mixer.h"
#include "flight/pid.h"

#include "rx/rx.h"

#include "io/serial.h"

#include "sensors/acceleration.h"
#include "sensors/battery.h"
#include "sensors/compass.h"
#include "sensors/sensors.h"

#include "telemetry/telemetry.h"
#include "fc/fc_msp_box.h"
#include "fc/rc_modes.h"
#include "io/piniobox.h"

void targetConfiguration(void)
{
    pinioBoxConfigMutable()->permanentId[0] = BOX_PERMANENT_ID_USER1;
    pinioBoxConfigMutable()->permanentId[1] = BOX_PERMANENT_ID_USER2;
    
    serialConfigMutable()->portConfigs[findSerialPortIndexByIdentifier(SERIAL_PORT_USART2)].functionMask = FUNCTION_RX_SERIAL;
    
    serialConfigMutable()->portConfigs[findSerialPortIndexByIdentifier(SERIAL_PORT_USART4)].functionMask = FUNCTION_MSP;
    serialConfigMutable()->portConfigs[findSerialPortIndexByIdentifier(SERIAL_PORT_USART4)].msp_baudrateIndex = BAUD_115200;
    serialConfigMutable()->portConfigs[findSerialPortIndexByIdentifier(SERIAL_PORT_USART5)].functionMask = FUNCTION_GPS;

    modeActivationConditionsMutable(0)->modeId = BOXARM;
    modeActivationConditionsMutable(0)->auxChannelIndex = 0; // AUX1 / channel 5
    modeActivationConditionsMutable(0)->range.startStep = CHANNEL_VALUE_TO_STEP(1750);
    modeActivationConditionsMutable(0)->range.endStep = CHANNEL_VALUE_TO_STEP(2100);

    modeActivationConditionsMutable(1)->modeId = BOXANGLE;
    modeActivationConditionsMutable(1)->auxChannelIndex = 2; // AUX3 / channel 7
    modeActivationConditionsMutable(1)->range.startStep = CHANNEL_VALUE_TO_STEP(900);
    modeActivationConditionsMutable(1)->range.endStep = CHANNEL_VALUE_TO_STEP(2100);

    modeActivationConditionsMutable(2)->modeId = BOXNAVPOSHOLD;
    modeActivationConditionsMutable(2)->auxChannelIndex = 2; // AUX3 / channel 7
    modeActivationConditionsMutable(2)->range.startStep = CHANNEL_VALUE_TO_STEP(1700);
    modeActivationConditionsMutable(2)->range.endStep = CHANNEL_VALUE_TO_STEP(2100);

    modeActivationConditionsMutable(3)->modeId = BOXNAVALTHOLD;
    modeActivationConditionsMutable(3)->auxChannelIndex = 2; // AUX3 / channel 7
    modeActivationConditionsMutable(3)->range.startStep = CHANNEL_VALUE_TO_STEP(1300);
    modeActivationConditionsMutable(3)->range.endStep = CHANNEL_VALUE_TO_STEP(2100);

    //罗盘配置 需要根据罗盘安装位置调整
    compassConfigMutable()->magZero.raw[X] = 217;
    compassConfigMutable()->magZero.raw[Y] = 296;
    compassConfigMutable()->magZero.raw[Z] = -279;
    compassConfigMutable()->magGain[X] = 1021;
    compassConfigMutable()->magGain[Y] = 1069;
    compassConfigMutable()->magGain[Z] = 1004;
    compassConfigMutable()->mag_align = CW90_DEG_FLIP;
    compassConfigMutable()->rollDeciDegrees = -250;
    compassConfigMutable()->pitchDeciDegrees = 1800;
    compassConfigMutable()->yawDeciDegrees = 900;

    //三寸机加速计校准值 需要根据飞控安装位置调整
    accelerometerConfigMutable()->accZero.raw[X] = -1;
    accelerometerConfigMutable()->accZero.raw[Y] = -5;
    accelerometerConfigMutable()->accZero.raw[Z] = 6;
    accelerometerConfigMutable()->accGain.raw[X] = 4095;
    accelerometerConfigMutable()->accGain.raw[Y] = 4095;
    accelerometerConfigMutable()->accGain.raw[Z] = 4109;
}
