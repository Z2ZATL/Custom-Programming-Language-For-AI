
#include "../include/DataManager.h"
#include "../include/DataProcessor.h"
#include "../include/Model.h"
#include <iostream>
#include <memory>

int main() {
    try {
        // 1. โหลดข้อมูล
        DataManager dataManager;
        DataSet dataset = dataManager.loadFromFile("data.csv", DataFormat::CSV, true);
        std::cout << "โหลดข้อมูลเรียบร้อย" << std::endl;
        
        // 2. แบ่งข้อมูลเป็นชุดฝึกและชุดทดสอบ
        auto [trainSet, testSet] = dataset.split(0.2); // 80% ฝึก, 20% ทดสอบ
        std::cout << "แบ่งข้อมูลเรียบร้อย: ชุดฝึก " << trainSet.dimensions().first 
                  << " แถว, ชุดทดสอบ " << testSet.dimensions().first << " แถว" << std::endl;
        
        // 3. ประมวลผลข้อมูล (Feature scaling)
        DataProcessor processor;
        processor.normalize(trainSet, NormalizationType::Z_SCORE);
        processor.normalize(testSet, NormalizationType::Z_SCORE);
        std::cout << "ปรับข้อมูลให้เป็นมาตรฐานเรียบร้อย" << std::endl;
        
        // 4. แยกคุณลักษณะ (Features) และเป้าหมาย (Target)
        int targetIndex = trainSet.dimensions().second - 1; // คอลัมน์สุดท้ายเป็นเป้าหมาย
        auto [trainFeatures, trainTarget] = processor.splitFeaturesTarget(trainSet, targetIndex);
        auto [testFeatures, testTarget] = processor.splitFeaturesTarget(testSet, targetIndex);
        std::cout << "แยกคุณลักษณะและเป้าหมายเรียบร้อย" << std::endl;
        
        // 5. สร้างและฝึกโมเดล
        std::unique_ptr<Model> model = ModelFactory::createModel("linear_regression");
        if (!model) {
            throw std::runtime_error("ไม่สามารถสร้างโมเดลได้");
        }
        
        std::cout << "เริ่มการฝึกโมเดล..." << std::endl;
        bool success = model->train(trainFeatures);
        if (!success) {
            throw std::runtime_error("การฝึกโมเดลล้มเหลว");
        }
        std::cout << "ฝึกโมเดลเสร็จสิ้น" << std::endl;
        
        // 6. ประเมินประสิทธิภาพโมเดล
        ModelMetrics metrics = model->evaluate(testFeatures, testTarget);
        std::cout << "ผลการประเมินโมเดล:" << std::endl;
        std::cout << metrics.toString() << std::endl;
        
        // 7. บันทึกโมเดล
        model->saveToFile("trained_model.dat");
        std::cout << "บันทึกโมเดลเรียบร้อย" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "เกิดข้อผิดพลาด: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
