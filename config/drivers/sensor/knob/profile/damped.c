/*
 * Copyright (c) 2023 XiNGRZ
 * SPDX-License-Identifier: MIT
 */

#define DT_DRV_COMPAT zmk_knob_profile_damped

#include <zephyr/device.h>
#include <zephyr/kernel.h>

#include <knob/math.h>
#include <knob/drivers/knob.h>
#include <knob/drivers/motor.h>
#include <knob/drivers/profile.h>

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(knob_damped, CONFIG_ZMK_LOG_LEVEL);

struct knob_damped_config {
	KNOB_PROFILE_CFG_ROM;
};

static int knob_damped_enable(const struct device *dev)
{
	const struct knob_damped_config *cfg = dev->config;

	motor_set_torque_limit(cfg->motor, KNOB_PROFILE_TORQUE_LIMIT);

#if KNOB_PROFILE_HAS_VELOCITY_PID
	motor_set_velocity_pid(cfg->motor, KNOB_PROFILE_VELOCITY_PID);
#endif /* KNOB_PROFILE_HAS_VELOCITY_PID */

#if KNOB_PROFILE_HAS_ANGLE_PID
	motor_set_angle_pid(cfg->motor, KNOB_PROFILE_ANGLE_PID);
#endif /* KNOB_PROFILE_HAS_ANGLE_PID */

	return 0;
}

static int knob_damped_update_params(const struct device *dev, struct knob_params params)
{
	ARG_UNUSED(dev);
	ARG_UNUSED(params);

	return 0;
}

static int knob_damped_tick(const struct device *dev, struct motor_control *mc)
{
	ARG_UNUSED(dev);

	mc->mode = VELOCITY;
	mc->target = 0.0f;

	return 0;
}

static int knob_damped_init(const struct device *dev)
{
	ARG_UNUSED(dev);

	return 0;
}

static const struct knob_profile_api knob_damped_api = {
	.enable = knob_damped_enable,
	.update_params = knob_damped_update_params,
	.tick = knob_damped_tick,
};

static const struct knob_damped_config knob_damped_cfg = { KNOB_PROFILE_CFG_INIT };

DEVICE_DT_INST_DEFINE(0, knob_damped_init, NULL, NULL, &knob_damped_cfg, POST_KERNEL,
		      CONFIG_SENSOR_INIT_PRIORITY, &knob_damped_api);
