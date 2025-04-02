
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
    
    // สร้างโฟลเดอร์สำหรับรูปภาพ
    std::string mkdir_cmd = "mkdir -p '" + outputPath + "'";
    int mkdir_result = system(mkdir_cmd.c_str());
    if (mkdir_result != 0) {
        std::cerr << "Warning: Could not create directory for pictures." << std::endl;
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
    
    // สร้างไฟล์ HTML/SVG สำหรับแสดงกราฟ
    std::string htmlPath = outputPath + "/learning_curves.html";
    std::ofstream htmlFile(htmlPath);
    
    if (!htmlFile.is_open()) {
        std::cerr << "Error: Could not open file for writing: " << htmlPath << std::endl;
        return;
    }
    
    // สร้างไฟล์ HTML ที่แสดงกราฟด้วย SVG
    htmlFile << "<!DOCTYPE html>\n";
    htmlFile << "<html lang=\"en\">\n";
    htmlFile << "<head>\n";
    htmlFile << "    <meta charset=\"UTF-8\">\n";
    htmlFile << "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
    htmlFile << "    <title>Learning Curves</title>\n";
    htmlFile << "    <style>\n";
    htmlFile << "        body { font-family: Arial, sans-serif; margin: 20px; }\n";
    htmlFile << "        .graph-container { display: flex; justify-content: space-between; }\n";
    htmlFile << "        .graph { width: 48%; border: 1px solid #ccc; padding: 10px; }\n";
    htmlFile << "        h2 { text-align: center; }\n";
    htmlFile << "    </style>\n";
    htmlFile << "</head>\n";
    htmlFile << "<body>\n";
    htmlFile << "    <h1>Learning Curves</h1>\n";
    htmlFile << "    <div class=\"graph-container\">\n";
    
    // กราฟ Loss
    htmlFile << "        <div class=\"graph\">\n";
    htmlFile << "            <h2>Loss vs. Epochs</h2>\n";
    htmlFile << "            <svg width=\"500\" height=\"300\" viewBox=\"0 0 500 300\">\n";
    htmlFile << "                <rect width=\"500\" height=\"300\" fill=\"#f8f8f8\"></rect>\n";
    
    // แกน X และ Y
    htmlFile << "                <line x1=\"50\" y1=\"250\" x2=\"450\" y2=\"250\" stroke=\"black\" stroke-width=\"2\"></line>\n";
    htmlFile << "                <line x1=\"50\" y1=\"50\" x2=\"50\" y2=\"250\" stroke=\"black\" stroke-width=\"2\"></line>\n";
    
    // คำอธิบายแกน
    htmlFile << "                <text x=\"250\" y=\"280\" text-anchor=\"middle\">Epochs</text>\n";
    htmlFile << "                <text x=\"20\" y=\"150\" text-anchor=\"middle\" transform=\"rotate(-90,20,150)\">Loss</text>\n";
    
    // เส้นตาราง
    for (int i = 1; i < 5; i++) {
        int y = 250 - 50 * i;
        htmlFile << "                <line x1=\"50\" y1=\"" << y << "\" x2=\"450\" y2=\"" << y 
                 << "\" stroke=\"#ccc\" stroke-width=\"1\"></line>\n";
        htmlFile << "                <text x=\"40\" y=\"" << y + 5 << "\" text-anchor=\"end\">" 
                 << 0.2 * i << "</text>\n";
    }
    
    // เส้นกราฟ Loss
    htmlFile << "                <polyline points=\"";
    for (int i = 0; i < epochs; i++) {
        double x_pos = 50 + (400.0 * i) / (epochs - 1);
        double y_pos = 250 - (loss[i] / 0.82) * 200;
        htmlFile << x_pos << "," << y_pos << " ";
    }
    htmlFile << "\" fill=\"none\" stroke=\"red\" stroke-width=\"2\"></polyline>\n";
    
    htmlFile << "            </svg>\n";
    htmlFile << "        </div>\n";
    
    // กราฟ Accuracy
    htmlFile << "        <div class=\"graph\">\n";
    htmlFile << "            <h2>Accuracy vs. Epochs</h2>\n";
    htmlFile << "            <svg width=\"500\" height=\"300\" viewBox=\"0 0 500 300\">\n";
    htmlFile << "                <rect width=\"500\" height=\"300\" fill=\"#f8f8f8\"></rect>\n";
    
    // แกน X และ Y
    htmlFile << "                <line x1=\"50\" y1=\"250\" x2=\"450\" y2=\"250\" stroke=\"black\" stroke-width=\"2\"></line>\n";
    htmlFile << "                <line x1=\"50\" y1=\"50\" x2=\"50\" y2=\"250\" stroke=\"black\" stroke-width=\"2\"></line>\n";
    
    // คำอธิบายแกน
    htmlFile << "                <text x=\"250\" y=\"280\" text-anchor=\"middle\">Epochs</text>\n";
    htmlFile << "                <text x=\"20\" y=\"150\" text-anchor=\"middle\" transform=\"rotate(-90,20,150)\">Accuracy</text>\n";
    
    // เส้นตาราง
    for (int i = 1; i < 5; i++) {
        int y = 250 - 50 * i;
        htmlFile << "                <line x1=\"50\" y1=\"" << y << "\" x2=\"450\" y2=\"" << y 
                 << "\" stroke=\"#ccc\" stroke-width=\"1\"></line>\n";
        htmlFile << "                <text x=\"40\" y=\"" << y + 5 << "\" text-anchor=\"end\">" 
                 << 0.2 * i << "</text>\n";
    }
    
    // เส้นกราฟ Accuracy
    htmlFile << "                <polyline points=\"";
    for (int i = 0; i < epochs; i++) {
        double x_pos = 50 + (400.0 * i) / (epochs - 1);
        double y_pos = 250 - (accuracy[i] / 0.95) * 200;
        htmlFile << x_pos << "," << y_pos << " ";
    }
    htmlFile << "\" fill=\"none\" stroke=\"blue\" stroke-width=\"2\"></polyline>\n";
    
    htmlFile << "            </svg>\n";
    htmlFile << "        </div>\n";
    htmlFile << "    </div>\n";
    htmlFile << "</body>\n";
    htmlFile << "</html>\n";
    
    htmlFile.close();
    
    // สร้างไฟล์ PNG ด้วย ImageMagick
    std::string pngPath = outputPath + "/learning_curves.png";
    
    // ใช้ ImageMagick เพื่อแปลง HTML เป็น PNG
    std::string convert_cmd = "mkdir -p \"" + outputPath + "\" && which convert > /dev/null 2>&1 && convert -quality 100 -resize 1200x800 \"" + htmlPath + "\" \"" + pngPath + "\"";
    int img_result = system(convert_cmd.c_str());
    
    if (img_result != 0) {
        // ถ้าไม่มี ImageMagick ให้สร้างไฟล์ PNG ง่ายๆ
        std::ofstream pngFile(pngPath);
        if (pngFile.is_open()) {
            pngFile << "<This is a placeholder for PNG image. Please open the HTML file for the actual graph.>";
            pngFile.close();
            std::cout << "ImageMagick not found. Created placeholder PNG file. For actual graph, please view the HTML file." << std::endl;
        } else {
            std::cerr << "Error: Could not create placeholder PNG file at " << pngPath << std::endl;
        }
    }
    
    std::cout << "Graph saved as '" << htmlPath << "'" << std::endl;
    std::cout << "Data saved as '" << dataPath << "'" << std::endl;
    std::cout << "PNG image saved as '" << pngPath << "'" << std::endl;
}

} // namespace ai_language
