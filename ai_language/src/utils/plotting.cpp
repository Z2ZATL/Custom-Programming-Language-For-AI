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

    // สร้างไฟล์ Python script สำหรับสร้างกราฟ
    std::string pythonScriptPath = outputPath + "/generate_plot.py";
    std::ofstream pythonScript(pythonScriptPath);
    
    if (!pythonScript.is_open()) {
        std::cerr << "Error: Could not create Python script for plotting" << std::endl;
        std::cout << "ข้อมูลได้รับการบันทึกเป็นไฟล์ CSV: " << dataPath << std::endl;
        return;
    }
    
    // เขียนโค้ด Python สำหรับสร้างกราฟ
    pythonScript << "import matplotlib.pyplot as plt\n";
    pythonScript << "import pandas as pd\n";
    pythonScript << "import os\n\n";
    pythonScript << "# อ่านข้อมูลจากไฟล์ CSV\n";
    pythonScript << "data = pd.read_csv('" << dataPath << "')\n\n";
    pythonScript << "# สร้างกราฟ\n";
    pythonScript << "plt.figure(figsize=(10, 6))\n";
    pythonScript << "plt.subplot(2, 1, 1)\n";
    pythonScript << "plt.plot(data['Epoch'], data['Loss'], 'r-', label='Loss')\n";
    pythonScript << "plt.title('Learning Curves')\n";
    pythonScript << "plt.ylabel('Loss')\n";
    pythonScript << "plt.grid(True)\n";
    pythonScript << "plt.legend()\n\n";
    pythonScript << "plt.subplot(2, 1, 2)\n";
    pythonScript << "plt.plot(data['Epoch'], data['Accuracy'], 'b-', label='Accuracy')\n";
    pythonScript << "plt.xlabel('Epoch')\n";
    pythonScript << "plt.ylabel('Accuracy')\n";
    pythonScript << "plt.grid(True)\n";
    pythonScript << "plt.legend()\n\n";
    pythonScript << "# บันทึกกราฟเป็นไฟล์ PNG\n";
    pythonScript << "plt.tight_layout()\n";
    pythonScript << "plt.savefig('" << outputPath << "/learning_curves.png')\n";
    pythonScript << "print('Plot saved as " << outputPath << "/learning_curves.png')\n";
    
    pythonScript.close();
    
    // รันไฟล์ Python script
    std::string command = "python3 " + pythonScriptPath;
    int result = system(command.c_str());
    
    if (result == 0) {
        std::cout << "ข้อมูลได้รับการบันทึกเป็นไฟล์ CSV: " << dataPath << std::endl;
        std::cout << "กราฟถูกสร้างและบันทึกเป็นไฟล์ PNG: " << outputPath << "/learning_curves.png" << std::endl;
    } else {
        std::cerr << "Error: Failed to generate plot. Make sure matplotlib and pandas are installed." << std::endl;
        std::cout << "ข้อมูลได้รับการบันทึกเป็นไฟล์ CSV: " << dataPath << std::endl;
        std::cout << "คำแนะนำ: ติดตั้ง matplotlib และ pandas ด้วยคำสั่ง 'pip install matplotlib pandas'" << std::endl;
    }
}

} // namespace ai_language