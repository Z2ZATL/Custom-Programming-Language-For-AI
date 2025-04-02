
#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include "../../include/utils/plotting.h"

// Matplotlib-cpp header
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;

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
    
    // สร้างโฟลเดอร์สำหรับรูปภาพ
    std::string mkdir_cmd = "mkdir -p '" + outputPath + "'";
    int mkdir_result = system(mkdir_cmd.c_str());
    if (mkdir_result != 0) {
        std::cerr << "Warning: Could not create directory for pictures." << std::endl;
    }
    
    // กำหนดขนาด figure
    plt::figure_size(1200, 500);
    
    // สร้างกราฟ loss
    plt::subplot(1, 2, 1);
    plt::plot(x, loss, "r-");
    plt::title("Loss vs. Epochs");
    plt::xlabel("Epochs");
    plt::ylabel("Loss");
    plt::grid(true);
    
    // สร้างกราฟ accuracy
    plt::subplot(1, 2, 2);
    plt::plot(x, accuracy, "b-");
    plt::title("Accuracy vs. Epochs");
    plt::xlabel("Epochs");
    plt::ylabel("Accuracy");
    plt::grid(true);
    
    // บันทึกกราฟ
    std::string fullPath = outputPath + "/learning_curves.png";
    plt::save(fullPath);
    plt::close();
    
    std::cout << "Graph saved as '" << fullPath << "'" << std::endl;
}

} // namespace ai_language
