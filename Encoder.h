#include "main.h"
#include "stm32f1xx_it.h"

typedef struct {
	float angle_motor; //the angle of the physical motor
	float angle_shaft, prev_angle_shaft; //the angle of the shaft after the reductor

	float turns; // number of completed turns
	float prev_turns;

	float shaft_rot_velocity; //speed of the shaft after the reductor
	float motor_rot_velocity;

	TIM_HandleTypeDef *htim; //the timer to which the encoder is connected

	int16_t Encoder_Clicks; //value for part of the process
	int16_t Gear_Ratio; // the gear ratio of the shaft to the motor
	int16_t prescaler; // prescaler needed to not overflow when the gear ratio is too big

	uint32_t counter; //value for part of the process
	int32_t count; //value for part of the process
	int16_t clicks; //value for part of the process
	int32_t x; //value for part of the process
	int16_t update_period; //milliseconds it takes for the program to update the speed
} Encoder;

//encoder_init should be written right after the initialization of the timers
	// clicks: amount of clicks that has the encoder per revolution
	// gear_ratio: Gear ratio of the shaft to the motor
	// prescaler: used in case of overflow
	// update_frequency: time in milliseconds that the update of the velocity is supposed to happen at
	// TIM_HandleTypeDef *htim: timer to which the encoder is connected
void encoder_init(Encoder *enc, int16_t clicks, int16_t gear_ratio, int16_t prescaler, int16_t update_period, TIM_HandleTypeDef *htim);

//update_position should be written in the function that handles the interrupt
void update_position(Encoder *enc);

//update_speed function needs to be called every millisecond for the math to work out, can be written in SysClicks
void update_speed(Encoder *enc);
