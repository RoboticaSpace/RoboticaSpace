//----------------------------------------------------------------------------
//
//  $Workfile: Kalman.h$
//
//  $Revision: X$
//
//  Project:    Robot Network Data Objects
//
//                            Copyright (c) 2016
//                               James A Wright
//                            All Rights Reserved
//
//  Modification History:
//  $Log:
//  $
//
//----------------------------------------------------------------------------
#ifndef KALMAN_H
#define KALMAN_H

class KalmanFilter
{
public:
    /* Kalman filter variables */
    double mQAngle; // Process noise variance for the accelerometer
    double mQBias; // Process noise variance for the gyro bias
    double mRMeasure; // Measurement noise variance - this is actually the variance of the measurement noise

    double mAngle; // The angle calculated by the Kalman filter - part of the 2x1 state vector
    double mBias; // The gyro bias calculated by the Kalman filter - part of the 2x1 state vector
    double mRate; // Unbiased rate calculated from the rate and the calculated bias - you have to call getAngle to update the rate

    double mCovarianceMatrix[2][2]; // Error covariance matrix - This is a 2x2 matrix
    double mKalmanGain[2]; // Kalman gain - This is a 2x1 vector
    double mAngleDifference; // Angle difference
    double mEstimateError; // Estimate error

    KalmanFilter();
    KalmanFilter(double qAngle, double qBias, double rMeasure);

    // The angle should be in degrees and the rate should be in degrees per second and the delta time in seconds
    double getAngle(double newAngle, double newRate, double deltaTime);
    void setAngle(double newAngle);
    double getRate();

    /* These are used to tune the Kalman filter */
    void setQangle(double newQ_angle);
    void setQbias(double newQ_bias);
    void setRmeasure(double newR_measure);
    double getQangle();
    double getQbias();
    double getRmeasure();
};
#endif
