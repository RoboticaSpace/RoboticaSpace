//----------------------------------------------------------------------------
//
//  $Workfile: Kalman.cpp$
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
#include "Kalman.h"

KalmanFilter::KalmanFilter()
{
    /* We will set the variables like so, these can also be tuned by the user */
    mQAngle = 0.0012;
    mQBias = 14.1;
    mRMeasure = 0.015;

    mAngle = 0; // Reset the angle
    mBias = 0; // Reset bias

    mCovarianceMatrix[0][0] = 0; // Since we assume that the bias is 0 and we know the starting angle (use setAngle), the error covariance matrix is set like so - see: http://en.wikipedia.org/wiki/Kalman_filter#Example_application.2C_technical
    mCovarianceMatrix[0][1] = 0;
    mCovarianceMatrix[1][0] = 0;
    mCovarianceMatrix[1][1] = 0;
}

KalmanFilter::KalmanFilter(double qAngle, double qBias, double rMeasure)
{
    /* We will set the variables like so, these can also be tuned by the user */
    mQAngle = qAngle;
    mQBias = qBias;
    mRMeasure = rMeasure;

    mAngle = 0; // Reset the angle
    mBias = 0; // Reset bias

    mCovarianceMatrix[0][0] = 0; // Since we assume that the bias is 0 and we know the starting angle (use setAngle), the error covariance matrix is set like so - see: http://en.wikipedia.org/wiki/Kalman_filter#Example_application.2C_technical
    mCovarianceMatrix[0][1] = 0;
    mCovarianceMatrix[1][0] = 0;
    mCovarianceMatrix[1][1] = 0;
}


// The angle should be in degrees and the rate should be in degrees per second and the delta time in seconds
double KalmanFilter::getAngle(double newAngle, double newRate, double deltaTime)
{
    // KasBot V2  -  Kalman filter module - http://www.x-firm.com/?page_id=145
    // Modified by Kristian Lauszus
    // See my blog post for more information: http://blog.tkjelectronics.dk/2012/09/a-practical-approach-to-kalman-filter-and-how-to-implement-it

    // Discrete Kalman filter time update equations - Time Update ("Predict")
    // Update xhat - Project the state ahead
    /* Step 1 */
    mRate = newRate - mBias;
    mAngle += deltaTime * mRate;

    // Update estimation error covariance - Project the error covariance ahead
    /* Step 2 */
    mCovarianceMatrix[0][0] += deltaTime * (deltaTime*mCovarianceMatrix[1][1] -
        mCovarianceMatrix[0][1] -
        mCovarianceMatrix[1][0] + mQAngle);
    mCovarianceMatrix[0][1] -= deltaTime * mCovarianceMatrix[1][1];
    mCovarianceMatrix[1][0] -= deltaTime * mCovarianceMatrix[1][1];
    mCovarianceMatrix[1][1] += mQBias * deltaTime;

    // Calculate angle and bias - Update estimate with measurement zk (newAngle)
    /* Step 3 */
    mAngleDifference = newAngle - mAngle;

    // Discrete Kalman filter measurement update equations - Measurement Update ("Correct")
    // Calculate Kalman gain - Compute the Kalman gain
    /* Step 4 */
    mEstimateError = mCovarianceMatrix[0][0] + mRMeasure;

    /* Step 5 */
    mKalmanGain[0] = mCovarianceMatrix[0][0] / mEstimateError;
    mKalmanGain[1] = mCovarianceMatrix[1][0] / mEstimateError;

    /* Step 6 */
    mAngle += mKalmanGain[0] * mAngleDifference;
    mBias += mKalmanGain[1] * mAngleDifference;

    // Calculate estimation error covariance - Update the error covariance
    /* Step 7 */
    mCovarianceMatrix[0][0] -= mKalmanGain[0] * mCovarianceMatrix[0][0];
    mCovarianceMatrix[0][1] -= mKalmanGain[0] * mCovarianceMatrix[0][1];
    mCovarianceMatrix[1][0] -= mKalmanGain[1] * mCovarianceMatrix[0][0];
    mCovarianceMatrix[1][1] -= mKalmanGain[1] * mCovarianceMatrix[0][1];

    return mAngle;
}

void KalmanFilter::setAngle(double newAngle)
{
    mAngle = newAngle;
} // Used to set angle, this should be set as the starting angle

double KalmanFilter::getRate()
{
    return mRate;
} // Return the unbiased rate

/* These are used to tune the Kalman filter */
void KalmanFilter::setQangle(double newQ_angle)
{
    mQAngle = newQ_angle;
}

void KalmanFilter::setQbias(double newQ_bias)
{
    mQBias = newQ_bias;
}

void KalmanFilter::setRmeasure(double newR_measure)
{
    mRMeasure = newR_measure;
}

double KalmanFilter::getQangle()
{
    return mQAngle;
}

double KalmanFilter::getQbias()
{
    return mQBias;
}

double KalmanFilter::getRmeasure()
{
    return mRMeasure;
}
