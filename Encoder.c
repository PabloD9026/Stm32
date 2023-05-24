/*
 * Encoder.c
 *
 *  Created on: May 8, 2023
 *      Author: pablo
 */

#include "Encoder.h"

void encoder_init(Encoder *enc, int16_t clicks, int16_t gear_ratio, int16_t prescaler, int16_t update_period, TIM_HandleTypeDef *htim) {
    enc->Encoder_Clicks = clicks;
    enc->Gear_Ratio = gear_ratio;
    enc->prescaler = prescaler;
    enc->htim = htim;
    enc->update_period = update_period;
}

void update_position(Encoder *enc){
	enc->counter = __HAL_TIM_GET_COUNTER(enc->htim);
  //counter = TIM4 -> CNT;
	//we save the number so we can work with it
	
  enc->count = (int16_t)enc->counter;
  //makes the number go negative in case of other direction
	
  enc->clicks = enc->count/2;
  //filtering the total for total of clicks
  
	enc->angle_motor = 360*enc->clicks/(enc->Encoder_Clicks/enc->prescaler);
  //getting the angle of the motor, since it has 500 clicks, and was lowered by 22 in the settings
  
	enc->angle_shaft = enc->angle_motor/enc->Gear_Ratio;
  // adjusting for the reductor installed in the motor 1:22, and getting the resultive angle
  
  //there is a maximum amount of turns the motor can make before the count variable runs out,
	//the plan is to make a cycle at 360 degrees, where all the variables go to 0 and there will be one
	// variable that will count the turns that will never go to 0
	if(enc->angle_shaft > 360){
		__HAL_TIM_GET_COUNTER(enc->htim) = 0;
		enc->turns++;
	}
	if(enc->angle_shaft < -360){
		__HAL_TIM_GET_COUNTER(enc->htim) = 0;
		enc->turns--;
	}

}
void update_speed(Encoder *enc){
	enc->x++;
  //we add to this variable every millisecond
	if(enc->x>enc->update_period){
		if(enc->turns == enc->prev_turns){
			enc->shaft_rot_velocity = (enc->angle_shaft - enc->prev_angle_shaft)/(enc->update_period/1000);
      //we calculate the velocity using previous position and current, divided by the delay that we set
			//we have to check if the values have been reseted to not get a different speed when it completes a turn
			enc->shaft_rot_velocity = enc->shaft_rot_velocity*60/360;
      //we turn the speed from degrees per second to rotations per minute
		}
		enc->prev_angle_shaft = enc->angle_shaft;
    //let the previous angle be updated
    
		enc->motor_rot_velocity = enc->shaft_rot_velocity*enc->Gear_Ratio;
    //since they are linearly dependent we can just multiply the speed by the reduction value and get the speed of the motor
    
		enc->prev_turns = enc->turns;
		enc->x = 0;
    //delay over, we update the x so we can have the next delay
	}
}
