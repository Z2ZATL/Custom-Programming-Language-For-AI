
#include <iostream>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <cstdlib> // สำหรับ getenv

int main() {
    // แปลง epoch time 1743341364 เป็นวันที่และเวลา
    std::time_t time = 1743341364;
    
    // แสดงเวลาปัจจุบัน (UTC)
    std::tm* timeinfo_utc = std::gmtime(&time);
    std::cout << "เวลา UTC: " << std::put_time(timeinfo_utc, "%Y-%m-%d %H:%M:%S") << std::endl;
    
    // แสดงเวลาตาม local time zone
    std::tm* timeinfo_local = std::localtime(&time);
    std::cout << "เวลาท้องถิ่น: " << std::put_time(timeinfo_local, "%Y-%m-%d %H:%M:%S") << std::endl;
    
    // แสดงเวลาตาม time zone ที่ผู้ใช้กำหนด
    const char* user_tz = std::getenv("TZ");
    if (user_tz) {
        std::cout << "Time zone ที่กำหนด: " << user_tz << std::endl;
    } else {
        std::cout << "ไม่ได้กำหนด Time zone" << std::endl;
    }
    
    // แสดงความแตกต่างระหว่างเวลา UTC และเวลาท้องถิ่น
    std::time_t now = std::time(nullptr);
    std::tm now_utc = *std::gmtime(&now);
    std::tm now_local = *std::localtime(&now);
    
    // คำนวณความแตกต่างเป็นชั่วโมง
    double diff_hours = std::difftime(std::mktime(&now_local), std::mktime(&now_utc)) / 3600.0;
    std::cout << "ความต่างเวลาจาก UTC: " << (diff_hours >= 0 ? "+" : "") << diff_hours << " ชั่วโมง" << std::endl;
    
    return 0;
}
