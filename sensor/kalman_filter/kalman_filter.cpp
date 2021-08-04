#include <iostream>
#include "kalman_filter.h"
#define MEASUREMENTS    1
#define STATES          3
#define DELTAT          1.0 / 30
using namespace poc_autosar;

KalmanFilter::KalmanFilter(double dt, const KalmanMatrices& kalman_matrices)    
    : A(kalman_matrices.A), C(kalman_matrices.C), Q(kalman_matrices.Q), R(kalman_matrices.R), P0(kalman_matrices.P),
    m(C.rows()), n(A.rows()), dt(dt), initialized(false),
    I(n, n), x_hat(n), x_hat_new(n)
{
    I.setIdentity();
}

KalmanFilter::KalmanFilter() :initialized(false)
{    
    dt = DELTAT;
    n = STATES;
    m = MEASUREMENTS;
    A = Eigen::MatrixXd(n, n); // System dynamics matrix
    C = Eigen::MatrixXd(m, n); // Output matrix
    Q = Eigen::MatrixXd(n, n); // Process noise covariance
    R = Eigen::MatrixXd(m, m); // Measurement noise covariance
    P = Eigen::MatrixXd(n, n); // Estimate error covariance
    I = Eigen::MatrixXd(n, n);
    x_hat = Eigen::VectorXd(n);
    x_hat_new = Eigen::VectorXd(n);
    A << 1, dt, 0, 0, 1, dt, 0, 0, 1;
    C << 1, 0, 0;

    // Reasonable covariance matrices
    Q << .05, .05, .0, .05, .05, .0, .0, .0, .0;
    R << 5;
    P << .1, .1, .1, .1, 10000, 10, .1, 10, 100;
    P0 = P;
    I.setIdentity();

}

void KalmanFilter::init(double starting_time, const Eigen::VectorXd& starting_value)
{
    x_hat = starting_value;
    P = P0;
    this->starting_time = starting_time;
    t = starting_time;
    initialized = true;
}

void KalmanFilter::predict()
{
    x_hat_new = A * x_hat;
    P = A * P * A.transpose() + Q;
}

void KalmanFilter::init()
{
    Eigen::VectorXd starting_value(n);
    starting_value << 1, 0, -9.81;
    x_hat = starting_value;
    P = P0;
    starting_time = 0;
    t = starting_time;
    initialized = true;
}

void KalmanFilter::update(const Eigen::VectorXd& y)
{
    K = P * C.transpose() * (C * P * C.transpose() + R).inverse();
    x_hat_new += K * (y - C * x_hat_new);
    P = (I - K * C) * P;
    x_hat = x_hat_new;

    t += dt;
}

void KalmanFilter::update(const Eigen::VectorXd& y, double dt, const Eigen::MatrixXd A)
{
    this->A = A;
    this->dt = dt;
    update(y);
}