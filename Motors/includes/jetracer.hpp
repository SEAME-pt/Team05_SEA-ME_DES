/**
 * @file jetracer.hpp
 * @brief Header file for the JetCar class.
 *
 * This file contains the declaration of the JetCar class,
 * which provides control functionality for a robotic car using
 * servo motors, DC motors, and joystick input via SDL2.
 */

#ifndef JETRACER_HPP
#define JETRACER_HPP

#include <cmath>
#include <thread>
#include <iostream>
#include <chrono>
#include <atomic>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <stdexcept>
#include <SDL2/SDL.h>
#include <csignal>

/**
 * @class JetCar
 * @brief A class to control a robotic car using servos, motors, and a joystick.
 */
class JetCar {
public:
    /**
     * @brief Constructs a JetCar instance.
     * @param servo_addr_ess I2C address for the servo controller (default: 0x40).
     * @param motor_addr_ess I2C address for the motor controller (default: 0x60).
     */
    JetCar(int servo_addr_ess = 0x40, int motor_addr_ess = 0x60);

    /**
     * @brief Destroys the JetCar instance.
     */
    ~JetCar();

    /**
     * @brief Initializes the servo controller.
     */
    void init_servo();

    /**
     * @brief Initializes the motor controller.
     */
    void init_motors();

    /**
     * @brief Sets the steering angle of the car.
     * @param angle Steering angle in degrees (-180 to 180).
     */
    void set_steering(int angle);

    /**
     * @brief Sets the servo PWM values.
     * @param channel PWM channel.
     * @param on_value Start value of the PWM signal.
     * @param off_value End value of the PWM signal.
     */
    void set_servo_pwm(int channel, int on_value, int off_value);

    /**
     * @brief Sets the motor PWM values.
     * @param channel PWM channel.
     * @param value PWM value (0 to 4095).
     */
    void set_motor_pwm(int channel, int value);

    /**
     * @brief Sets the speed of the car.
     * @param speed Speed as a percentage (-100 to 100).
     */
    void set_speed(float speed);

    /**
     * @brief Processes joystick input for speed and steering control.
     */
    void process_joystick();

    /**
     * @brief Starts the car control system.
     */
    void start();

    /**
     * @brief Stops the car and halts all operations.
     */
    void stop();

    /**
     * @brief Checks if the car control system is running.
     * @return True if running, false otherwise.
     */
    bool is_running_() const;

private:
    int servo_addr_; ///< I2C address for the servo controller.
    int motor_addr_; ///< I2C address for the motor controller.
    int steering_channel_ = 0; ///< PWM channel for steering control.
    int MAX_ANGLE_ = 180; ///< Maximum steering angle in degrees.
    int SERVO_CENTER_PWM_ = 490; ///< PWM value for center steering position.
    int SERVO_LEFT_PWM_ = 310; ///< PWM value for full left steering.
    int SERVO_RIGHT_PWM_ = 620; ///< PWM value for full right steering.
    std::atomic<bool> running_; ///< Atomic flag indicating if the system is running.
    float current_speed_ = 0; ///< Current speed of the car.
    int current_angle_ = 0; ///< Current steering angle of the car.

    int servo_bus_fd_; ///< File descriptor for the servo I2C bus.
    int motor_bus_fd_; ///< File descriptor for the motor I2C bus.

    /**
     * @brief Writes a byte to the I2C bus.
     * @param addr I2C address.
     * @param reg Register address.
     * @param value Value to write.
     */
    void write_byte(int addr, uint8_t reg, uint8_t value);

    /**
     * @brief Reads a byte from the I2C bus.
     * @param addr I2C address.
     * @param reg Register address.
     * @return Value read from the register.
     */
    uint8_t read_byte(int addr, uint8_t reg);

    /**
     * @brief Delays execution for a specified time.
     * @param ms Delay in milliseconds.
     */
    void delay(int ms);

    /**
     * @brief Writes data to a register on the I2C bus.
     * @param fd File descriptor for the I2C bus.
     * @param reg Register address.
     * @param value Value to write.
     */
    void write_byte_data(int fd, int reg, int value);

    /**
     * @brief Reads data from a register on the I2C bus.
     * @param fd File descriptor for the I2C bus.
     * @param reg Register address.
     * @return Value read from the register.
     */
    int read_byte_data(int fd, int reg);
};

#endif // JETRACER_HPP
