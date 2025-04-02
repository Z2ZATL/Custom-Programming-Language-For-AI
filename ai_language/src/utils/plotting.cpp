#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "../../include/utils/plotting.h"

namespace ai_language {

void generateLearningCurves(int epochs, const std::string& outputPath) {
    // สร้างข้อมูลจำลอง
    std::vector<double> x(epochs);
    std::vector<double> loss(epochs);
    std::vector<double> accuracy(epochs);

    for (int i = 0; i < epochs; i++) {
        x[i] = i + 1;
        loss[i] = 0.82 - 0.77 * (1 - std::exp(-(i+1)/30.0));
        accuracy[i] = 0.65 + 0.3 * (1 - std::exp(-(i+1)/25.0));
    }

    // สร้างโฟลเดอร์สำหรับข้อมูล
    std::string mkdir_cmd = "mkdir -p '" + outputPath + "'";
    int mkdir_result = system(mkdir_cmd.c_str());
    if (mkdir_result != 0) {
        std::cerr << "Warning: Could not create directory for data." << std::endl;
    }

    // บันทึกข้อมูลเป็นไฟล์ CSV สำหรับการแสดงผลด้วย C++ ทั่วไป
    std::string dataPath = outputPath + "/learning_curves_data.csv";
    std::ofstream dataFile(dataPath);

    if (!dataFile.is_open()) {
        std::cerr << "Error: Could not open file for writing: " << dataPath << std::endl;
        return;
    }

    // เขียน header และข้อมูล
    dataFile << "Epoch,Loss,Accuracy" << std::endl;
    for (int i = 0; i < epochs; i++) {
        dataFile << x[i] << "," << loss[i] << "," << accuracy[i] << std::endl;
    }
    dataFile.close();
    
    // ใช้ไฟล์ plot_generator.py ที่มีอยู่แล้วในการสร้างกราฟ
    std::string scriptPath = "./src/utils/plot_generator.py";
    
    // รันคำสั่ง Python เพื่อสร้างกราฟ
    std::string command = "python3 " + scriptPath + " \"" + dataPath + "\" \"" + outputPath + "\"";
    int result = system(command.c_str());
    
    if (result == 0) {
        std::cout << "ข้อมูลได้รับการบันทึกเป็นไฟล์ CSV: ai_language/" << dataPath << std::endl;
        std::cout << "กราฟถูกสร้างและบันทึกเป็นไฟล์ PNG: ai_language/" << outputPath << "/learning_curves.png" << std::endl;
    } else {
        std::cerr << "Error: Failed to generate plot. Make sure matplotlib and pandas are installed." << std::endl;
        std::cout << "ข้อมูลได้รับการบันทึกเป็นไฟล์ CSV: " << dataPath << std::endl;
        std::cout << "คำแนะนำ: ติดตั้ง matplotlib และ pandas ด้วยคำสั่ง 'pip install matplotlib pandas'" << std::endl;
    }
}

} // namespace ai_language