#include <iostream>
#include "kalman_filter.h"

KalmanFilter::KalmanFilter(
    double dt,
    const Eigen::MatrixXd& A,
    const Eigen::MatrixXd& C,
    const Eigen::MatrixXd& Q,
    const Eigen::MatrixXd& R,
    const Eigen::MatrixXd& P)
    : A(A), C(C), Q(Q), R(R), P0(P),
    m(C.rows()), n(A.rows()), dt(dt), initialized(false),
    I(n, n), x_hat(n), x_hat_new(n)
{
    I.setIdentity();
}

KalmanFilter::KalmanFilter() :initialized(false)
{
    dt = 1.0 / 30;
    n = 3;
    m = 1;
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

void KalmanFilter::init(double t0, const Eigen::VectorXd& x0)
{
    x_hat = x0;
    P = P0;
    this->t0 = t0;
    t = t0;
    initialized = true;
}

void KalmanFilter::predict()
{
    x_hat_new = A * x_hat;
    P = A * P * A.transpose() + Q;
}

void KalmanFilter::init()
{
    Eigen::VectorXd x0(n);
    x0 << 1, 0, -9.81;
    x_hat = x0;
    P = P0;
    t0 = 0;
    t = t0;
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