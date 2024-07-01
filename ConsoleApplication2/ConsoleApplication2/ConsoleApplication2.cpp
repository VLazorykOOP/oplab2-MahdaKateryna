#include <iostream>
#include <thread>
#include <vector>
#include <cmath>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <mutex>

const int WIDTH = 100;
const int HEIGHT = 100;
const double V = 1.0;  // Швидкість
const int N = 3;       // Зміна напрямку кожні N секунд
const double R = 10.0; // Радіус кола для менеджерів
const int MAX_STEPS = 5; // Максимальна кількість кроків

std::mutex mtx;

void moveDeveloper(int id) {
    double x = rand() % WIDTH;
    double y = rand() % HEIGHT;
    double angle = (rand() % 360) * 3.14 / 180.0;
    int steps = 0;

    while (steps < MAX_STEPS) {
        std::this_thread::sleep_for(std::chrono::seconds(N));
        angle = (rand() % 360) * 3.14 / 180.0; // випадкова зміна напрямку
        double dx = V * cos(angle);
        double dy = V * sin(angle);

        std::lock_guard<std::mutex> guard(mtx);
        x += dx;
        y += dy;

        if (x < 0) x = 0;
        if (x >= WIDTH) x = WIDTH - 1;
        if (y < 0) y = 0;
        if (y >= HEIGHT) y = HEIGHT - 1;

        std::cout << "Developer " << id << " at (" << x << ", " << y << ") Step: " << steps + 1 << "\n";
        steps++;
    }
}

void moveManager(int id) {
    double x = WIDTH / 2.0;
    double y = HEIGHT / 2.0;
    double angle = 0.0;
    int steps = 0;

    while (steps < MAX_STEPS) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        angle += V / R;

        std::lock_guard<std::mutex> guard(mtx);
        double newX = x + R * cos(angle);
        double newY = y + R * sin(angle);

        std::cout << "Manager " << id << " at (" << newX << ", " << newY << ") Step: " << steps + 1 << "\n";
        steps++;
    }
}

int main() {
    srand(time(0));

    std::vector<std::thread> developers;
    std::vector<std::thread> managers;

    for (int i = 0; i < 5; ++i) {
        developers.push_back(std::thread(moveDeveloper, i));
    }

    for (int i = 0; i < 5; ++i) {
        managers.push_back(std::thread(moveManager, i));
    }

    for (auto& dev : developers) {
        dev.join();
    }

    for (auto& mgr : managers) {
        mgr.join();
    }

    return 0;
}
