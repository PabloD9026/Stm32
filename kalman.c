#include "kalman.h"

void kalman_filter_init(KalmanFilter* kf, double q, double r, double x0, double p0) {
    kf->q = q;
    kf->r = r;
    kf->x = x0;
    kf->p = p0;
}

double kalman_filter_update(KalmanFilter* kf, double measurement) {
    // prediction update
    kf->p = kf->p + kf->q;

    // measurement update
    kf->k = kf->p / (kf->p + kf->r);
    kf->x = kf->x + kf->k * (measurement - kf->x);
    kf->p = (1 - kf->k) * kf->p;

    return kf->x;
}
