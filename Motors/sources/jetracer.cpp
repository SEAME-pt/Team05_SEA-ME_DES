#include "jetracer.hpp"

JetCar::JetCar(int servo_addr_ess, int motor_addr_ess) {
    // std::cout << "[DEBUG] In JetCar constructor\n";
    servo_addr_ = servo_addr_ess;
    motor_addr_ = motor_addr_ess;
    running_ = false;

    servo_bus_fd_ = open("/dev/i2c-1", O_RDWR);
    motor_bus_fd_ = open("/dev/i2c-1", O_RDWR);

    if (servo_bus_fd_ < 0 || motor_bus_fd_ < 0) {
        throw std::runtime_error("Failed to open I2C device");
    }

    if (ioctl(servo_bus_fd_, I2C_SLAVE, servo_addr_) < 0 || ioctl(motor_bus_fd_, I2C_SLAVE, motor_addr_) < 0) {
        throw std::runtime_error("Failed to set I2C address");
    }

    init_servo();
    init_motors();
    // std::cout << "[DEBUG] JetCar initialized\n";
}

JetCar::~JetCar() {
    stop();
    close(servo_bus_fd_);
    close(motor_bus_fd_);
}

void JetCar::init_servo() {
    // std::cout << "[DEBUG] In init_servo\n";
    write_byte_data(servo_bus_fd_, 0x00, 0x06);
    usleep(100000);

    write_byte_data(servo_bus_fd_, 0x00, 0x10);
    usleep(100000);

    write_byte_data(servo_bus_fd_, 0xFE, 0x79);
    usleep(100000);

    write_byte_data(servo_bus_fd_, 0x01, 0x04);
    usleep(100000);

    write_byte_data(servo_bus_fd_, 0x00, 0x20);
    usleep(100000);
}

void JetCar::init_motors() {
    // std::cout << "[DEBUG] In init_motors\n";
    write_byte_data(motor_bus_fd_, 0x00, 0x20);

    int prescale = static_cast<int>(std::floor(25000000.0 / 4096.0 / 100 - 1));
    int oldmode = read_byte_data(motor_bus_fd_, 0x00);
    int newmode = (oldmode & 0x7F) | 0x10;

    write_byte_data(motor_bus_fd_, 0x00, newmode);
    write_byte_data(motor_bus_fd_, 0xFE, prescale);
    write_byte_data(motor_bus_fd_, 0x00, oldmode);
    usleep(5000);
    write_byte_data(motor_bus_fd_, 0x00, oldmode | 0xa1);
}

void JetCar::set_steering(int angle) {
    // std::cout << "[DEBUG] In set_steering\n";
    angle = std::max(-MAX_ANGLE_, std::min(MAX_ANGLE_, angle));

    int pwm = 0;
    if (angle < 0) {
        pwm = SERVO_CENTER_PWM_ + (angle / (float)MAX_ANGLE_) * (SERVO_CENTER_PWM_ - SERVO_LEFT_PWM_);
    }
    else if (angle > 0) {
        pwm = SERVO_CENTER_PWM_ + (angle / (float)MAX_ANGLE_) * (SERVO_RIGHT_PWM_ - SERVO_CENTER_PWM_);
    }
    else {
        pwm = SERVO_CENTER_PWM_;
    }

    set_servo_pwm(steering_channel_, 0, pwm);
    current_angle_ = angle;
}

void JetCar::set_servo_pwm(int channel, int on_value, int off_value) {
    // std::cout << "[DEBUG] In set_servo_pwm\n";
    int base_reg = 0x06 + (channel * 4);
    write_byte_data(servo_bus_fd_, base_reg, on_value & 0xFF);
    write_byte_data(servo_bus_fd_, base_reg + 1, on_value >> 8);
    write_byte_data(servo_bus_fd_, base_reg + 2, off_value & 0xFF);
    write_byte_data(servo_bus_fd_, base_reg + 3, off_value >> 8);
}

void JetCar::set_motor_pwm(int channel, int value) {
    // std::cout << "[DEBUG] In set_motor_pwm\n";
    value = std::max(0, std::min(value, 4095));
    int base_reg = 0x06 + (4 * channel);
    write_byte_data(motor_bus_fd_, base_reg, 0);
    write_byte_data(motor_bus_fd_, base_reg + 1, 0);
    write_byte_data(motor_bus_fd_, base_reg + 2, value & 0xFF);
    write_byte_data(motor_bus_fd_, base_reg + 3, value >> 8);
}
void JetCar::set_speed(float speed) {
    // std::cout << "[DEBUG] In set_speed\n";
    speed = std::max(-100.0f, std::min(100.0f, speed));
    int pwm_value = (int)(std::abs(speed) / 100.0f * 4095);

    if (speed > 0) {
        set_motor_pwm(0, pwm_value);
        set_motor_pwm(1, 0);        
        set_motor_pwm(2, pwm_value);
        set_motor_pwm(5, pwm_value);
        set_motor_pwm(6, 0);        
        set_motor_pwm(7, pwm_value);
    }
    else if (speed < 0) {
        set_motor_pwm(0, pwm_value);
        set_motor_pwm(1, pwm_value);
        set_motor_pwm(2, 0);        
        set_motor_pwm(6, pwm_value);
        set_motor_pwm(7, pwm_value);
        set_motor_pwm(8, 0);        
    }
    else {
        for (int channel = 0; channel < 9; ++channel) {
            set_motor_pwm(channel, 0);
        }
    }

    current_speed_ = speed;
}

void JetCar::process_joystick() {
    // std::cout << "[DEBUG] In process_joystick\n";

    if (SDL_Init(SDL_INIT_JOYSTICK) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return;
    }

    SDL_Joystick* joystick = SDL_JoystickOpen(0);
    if (!joystick) {
        std::cerr << "Failed to open joystick: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return;
    }

    while (running_) {
        SDL_JoystickUpdate();

        int left_joystick_y = SDL_JoystickGetAxis(joystick, 1);
        int right_joystick_x = SDL_JoystickGetAxis(joystick, 2);

        set_speed(-left_joystick_y / 32767.0 * 100);
        set_steering(right_joystick_x / 32767.0 * MAX_ANGLE_);

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    SDL_JoystickClose(joystick);
    SDL_Quit();
}

void JetCar::start() {
    // std::cout << "[DEBUG] In start\n";
    running_ = true;
    std::thread joystick_thread(&JetCar::process_joystick, this);
    joystick_thread.detach();
}

void JetCar::stop() {
    // std::cout << "[DEBUG] In stop\n";
    running_ = false;
    set_speed(0);
    set_steering(0);
}

void JetCar::write_byte(int addr, uint8_t reg, uint8_t value) {
    // std::cout << "[DEBUG] In write_byte\n";
    int file;
    char *filename = (char*)"/dev/i2c-1";
    if ((file = open(filename, O_RDWR)) < 0) {
        throw std::runtime_error("Failed to open the i2c bus");
    }
    if (ioctl(file, I2C_SLAVE, addr) < 0) {
        throw std::runtime_error("Failed to acquire bus access and/or talk to slave");
    }

    uint8_t buffer[2] = {reg, value};
    if (write(file, buffer, 2) != 2) {
        throw std::runtime_error("Failed to write to the i2c bus");
    }

    close(file);
}

uint8_t JetCar::read_byte(int addr, uint8_t reg) {
    // std::cout << "[DEBUG] In read_byte\n";
    int file;
    char *filename = (char*)"/dev/i2c-1";
    if ((file = open(filename, O_RDWR)) < 0) {
        throw std::runtime_error("Failed to open the i2c bus");
    }
    if (ioctl(file, I2C_SLAVE, addr) < 0) {
        throw std::runtime_error("Failed to acquire bus access and/or talk to slave");
    }

    if (write(file, &reg, 1) != 1) {
        throw std::runtime_error("Failed to write to the i2c bus");
    }

    uint8_t value;
    if (read(file, &value, 1) != 1) {
        throw std::runtime_error("Failed to read from the i2c bus");
    }

    close(file);
    return value;
}

void JetCar::delay(int ms) {
    // std::cout << "[DEBUG] In delay\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

bool JetCar::is_running_() const {
    return running_.load();
}

void JetCar::write_byte_data(int fd, int reg, int value) {
    uint8_t buffer[2] = {static_cast<uint8_t>(reg), static_cast<uint8_t>(value)};
    if (write(fd, buffer, 2) != 2) {
        throw std::runtime_error("Failed to write to the i2c bus");
    }
}

int JetCar::read_byte_data(int fd, int reg) {
    if (write(fd, &reg, 1) != 1) {
        throw std::runtime_error("Failed to write to the i2c bus");
    }

    uint8_t value;
    if (read(fd, &value, 1) != 1) {
        throw std::runtime_error("Failed to read from the i2c bus");
    }

    return value;
}