/*
 * kalman.h
 *
 *  Created on: May 8, 2023
 *      Author: pablo
 */

#ifndef INC_KALMAN_H_
#define INC_KALMAN_H_

#include "main.h"

typedef struct {
    double q; // process noise covariance
    double r; // measurement noise covariance
    double x; // estimated value
    double p; // estimation error covariance
    double k; // Kalman gain
} KalmanFilter;

void kalman_filter_init(KalmanFilter* kf, double q, double r, double x0, double p0);

double kalman_filter_update(KalmanFilter* kf, double measurement);



#endif /* INC_KALMAN_H_ */
