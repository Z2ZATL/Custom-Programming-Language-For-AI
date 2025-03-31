
#include <iostream>
#include <ctime>
#include <chrono>
#include <iomanip>

int main() {
    // แปลง epoch time 1743341364 เป็นวันที่และเวลา
    std::time_t time = 1743341364;
    std::tm* timeinfo = std::localtime(&time);
    
    std::cout << "CREATED_TIME=" << time << " คือวันที่และเวลา:" << std::endl;
    std::cout << std::put_time(timeinfo, "%Y-%m-%d %H:%M:%S") << std::endl;
    
    return 0;
}
