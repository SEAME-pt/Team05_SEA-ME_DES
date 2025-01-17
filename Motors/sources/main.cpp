#include "jetracer.hpp"

JetCar* car_ptr = nullptr;

void signal_handler(int) {
    if (car_ptr) {
        car_ptr->stop();
    }
}

int main() {
    try {
        JetCar car;
        car_ptr = &car;
        signal(SIGINT, signal_handler);
        car.start();
        while (car.is_running_()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Gamepad error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}