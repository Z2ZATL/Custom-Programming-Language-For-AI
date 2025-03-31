
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <random>
#include <cmath>
#include <Eigen/Dense>

// ใช้ library Eigen สำหรับคำนวณทางคณิตศาสตร์
using Eigen::MatrixXd;
using Eigen::VectorXd;

class LinearRegression {
private:
    VectorXd coefficients;
    double learning_rate;
    int max_iterations;
    
public:
    LinearRegression(double lr = 0.01, int max_iter = 1000) 
        : learning_rate(lr), max_iterations(max_iter) {}
    
    void fit(const MatrixXd& X, const VectorXd& y) {
        int n_samples = X.rows();
        int n_features = X.cols();
        
        // เริ่มต้นค่าสัมประสิทธิ์เป็น 0
        coefficients = VectorXd::Zero(n_features);
        
        // Gradient Descent
        for (int iter = 0; iter < max_iterations; iter++) {
            // คำนวณค่าทำนาย: y_pred = X * coefficients
            VectorXd y_pred = X * coefficients;
            
            // คำนวณค่าความผิดพลาด (error)
            VectorXd error = y_pred - y;
            
            // คำนวณ gradient: gradient = (X^T * error) / n_samples
            VectorXd gradient = (X.transpose() * error) / n_samples;
            
            // อัปเดตค่าสัมประสิทธิ์ด้วย gradient descent
            coefficients = coefficients - learning_rate * gradient;
            
            // แสดงค่า MSE ทุก 100 รอบ
            if (iter % 100 == 0) {
                double mse = error.squaredNorm() / n_samples;
                std::cout << "Iteration " << iter << ", MSE: " << mse << std::endl;
            }
        }
    }
    
    VectorXd predict(const MatrixXd& X) const {
        return X * coefficients;
    }
    
    VectorXd getCoefficients() const {
        return coefficients;
    }
};

// ฟังก์ชันสำหรับอ่านข้อมูลจากไฟล์ CSV
std::pair<MatrixXd, VectorXd> loadCSVData(const std::string& filename, bool has_header = true) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("ไม่สามารถเปิดไฟล์ได้: " + filename);
    }
    
    std::vector<std::vector<double>> data;
    std::string line;
    
    // ข้าม header ถ้ามี
    if (has_header && std::getline(file, line)) {
        // ข้าม header
    }
    
    // อ่านข้อมูลแต่ละบรรทัด
    while (std::getline(file, line)) {
        std::vector<double> row;
        std::stringstream ss(line);
        std::string cell;
        
        while (std::getline(ss, cell, ',')) {
            row.push_back(std::stod(cell));
        }
        
        if (!row.empty()) {
            data.push_back(row);
        }
    }
    
    file.close();
    
    // แปลงข้อมูลเป็น Eigen Matrix
    int n_samples = data.size();
    int n_features = data[0].size() - 1;  // คอลัมน์สุดท้ายเป็น target
    
    MatrixXd X(n_samples, n_features);
    VectorXd y(n_samples);
    
    for (int i = 0; i < n_samples; i++) {
        for (int j = 0; j < n_features; j++) {
            X(i, j) = data[i][j];
        }
        y(i) = data[i][n_features];
    }
    
    return {X, y};
}

// ฟังก์ชันสำหรับแบ่งข้อมูลเป็นชุดฝึกและชุดทดสอบ
std::tuple<MatrixXd, MatrixXd, VectorXd, VectorXd> trainTestSplit(
    const MatrixXd& X, const VectorXd& y, double test_size = 0.2) {
    
    int n_samples = X.rows();
    int n_test = static_cast<int>(n_samples * test_size);
    int n_train = n_samples - n_test;
    
    // สร้าง indices และสลับลำดับ
    std::vector<int> indices(n_samples);
    for (int i = 0; i < n_samples; i++) {
        indices[i] = i;
    }
    
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(indices.begin(), indices.end(), g);
    
    // สร้าง matrices สำหรับชุดฝึกและชุดทดสอบ
    MatrixXd X_train(n_train, X.cols());
    MatrixXd X_test(n_test, X.cols());
    VectorXd y_train(n_train);
    VectorXd y_test(n_test);
    
    // แบ่งข้อมูล
    for (int i = 0; i < n_train; i++) {
        X_train.row(i) = X.row(indices[i]);
        y_train(i) = y(indices[i]);
    }
    
    for (int i = 0; i < n_test; i++) {
        X_test.row(i) = X.row(indices[n_train + i]);
        y_test(i) = y(indices[n_train + i]);
    }
    
    return {X_train, X_test, y_train, y_test};
}

// ฟังก์ชันสำหรับคำนวณค่า Mean Squared Error
double calculateMSE(const VectorXd& y_true, const VectorXd& y_pred) {
    return (y_true - y_pred).squaredNorm() / y_true.size();
}

// ฟังก์ชันสำหรับคำนวณค่า R-squared
double calculateR2(const VectorXd& y_true, const VectorXd& y_pred) {
    double y_mean = y_true.mean();
    double ss_total = 0.0;
    double ss_residual = 0.0;
    
    for (int i = 0; i < y_true.size(); i++) {
        ss_total += std::pow(y_true(i) - y_mean, 2);
        ss_residual += std::pow(y_true(i) - y_pred(i), 2);
    }
    
    return 1.0 - (ss_residual / ss_total);
}

// ฟังก์ชันหลัก
int main() {
    try {
        std::cout << "กำลังโหลดข้อมูล..." << std::endl;
        auto [X, y] = loadCSVData("data.csv");
        std::cout << "โหลดข้อมูลสำเร็จ: " << X.rows() << " แถว, " << X.cols() << " คุณลักษณะ" << std::endl;
        
        // แบ่งข้อมูลเป็นชุดฝึกและชุดทดสอบ
        auto [X_train, X_test, y_train, y_test] = trainTestSplit(X, y, 0.2);
        std::cout << "แบ่งข้อมูล: ชุดฝึก " << X_train.rows() << " แถว, ชุดทดสอบ " << X_test.rows() << " แถว" << std::endl;
        
        // สร้างและฝึกโมเดล
        std::cout << "\n=== เริ่มการฝึกโมเดล Linear Regression ===" << std::endl;
        LinearRegression model(0.01, 1000);
        model.fit(X_train, y_train);
        
        // ทำนายผลและประเมินโมเดล
        VectorXd y_pred = model.predict(X_test);
        double mse = calculateMSE(y_test, y_pred);
        double r2 = calculateR2(y_test, y_pred);
        
        std::cout << "\n=== ผลการประเมินโมเดล ===" << std::endl;
        std::cout << "Mean Squared Error: " << mse << std::endl;
        std::cout << "R-squared: " << r2 << std::endl;
        
        // แสดงค่าสัมประสิทธิ์
        VectorXd coeffs = model.getCoefficients();
        std::cout << "\nค่าสัมประสิทธิ์:" << std::endl;
        for (int i = 0; i < coeffs.size(); i++) {
            std::cout << "Feature " << i << ": " << coeffs(i) << std::endl;
        }
        
        // บันทึกโมเดล (ในรูปแบบที่ง่าย)
        std::ofstream model_file("linear_model_coefficients.txt");
        if (model_file.is_open()) {
            for (int i = 0; i < coeffs.size(); i++) {
                model_file << coeffs(i) << std::endl;
            }
            model_file.close();
            std::cout << "\nบันทึกโมเดลเรียบร้อย" << std::endl;
        } else {
            std::cerr << "ไม่สามารถบันทึกโมเดลได้" << std::endl;
        }
        
    } catch (const std::exception& e) {
        std::cerr << "เกิดข้อผิดพลาด: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
