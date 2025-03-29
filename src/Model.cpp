/**
 * @file Model.cpp
 * @brief Implementation of the machine learning model classes
 */

#include "Model.h"
#include "Utils.h"
#include <cmath>
#include <fstream>
#include <sstream>
#include <random>
#include <algorithm>
#include <limits>
#include <iostream>

// ModelMetrics implementation
std::string ModelMetrics::toString() const {
    std::stringstream ss;
    ss << "Model Metrics:" << std::endl;
    ss << "  Accuracy: " << accuracy << std::endl;
    ss << "  Precision: " << precision << std::endl;
    ss << "  Recall: " << recall << std::endl;
    ss << "  F1 Score: " << f1Score << std::endl;
    ss << "  Mean Squared Error: " << meanSquaredError << std::endl;
    ss << "  R² Score: " << r2Score << std::endl;
    
    if (!customMetrics.empty()) {
        ss << "  Custom Metrics:" << std::endl;
        for (const auto& [name, value] : customMetrics) {
            ss << "    " << name << ": " << value << std::endl;
        }
    }
    
    return ss.str();
}

// ModelFactory implementation
const std::unordered_map<std::string, ModelType> ModelFactory::MODEL_NAME_MAP = 
    ModelFactory::initModelMap();

std::unordered_map<std::string, ModelType> ModelFactory::initModelMap() {
    std::unordered_map<std::string, ModelType> map;
    map["linear_regression"] = ModelType::LINEAR_REGRESSION;
    map["logistic_regression"] = ModelType::LOGISTIC_REGRESSION;
    map["k_means"] = ModelType::K_MEANS;
    return map;
}

std::unique_ptr<Model> ModelFactory::createModel(ModelType type) {
    switch (type) {
        case ModelType::LINEAR_REGRESSION:
            return std::make_unique<LinearRegressionModel>();
        case ModelType::LOGISTIC_REGRESSION:
            return std::make_unique<LogisticRegressionModel>();
        case ModelType::K_MEANS:
            return std::make_unique<KMeansModel>();
        default:
            return nullptr;
    }
}

std::unique_ptr<Model> ModelFactory::createModel(const std::string& name) {
    std::string lowerName = Utils::toLower(name);
    auto it = MODEL_NAME_MAP.find(lowerName);
    if (it != MODEL_NAME_MAP.end()) {
        return createModel(it->second);
    }
    return nullptr;
}

#ifdef USE_EIGEN
// LinearRegressionModel implementation with Eigen

LinearRegressionModel::LinearRegressionModel(double learningRate, 
                                           int maxIterations,
                                           bool useGradientDescent)
    : m_weights(), 
      m_bias(0.0),
      m_learningRate(learningRate),
      m_maxIterations(maxIterations),
      m_useGradientDescent(useGradientDescent) {}

bool LinearRegressionModel::train(const DataSet& dataset) {
    const auto& data = dataset.getData();
    const auto& labels = dataset.getLabels();
    
    if (data.empty() || labels.empty() || data.size() != labels.size()) {
        return false;
    }
    
    Eigen::MatrixXd X = datasetToEigenMatrix(dataset);
    Eigen::VectorXd y = labelsToEigenVector(dataset);
    
    // Initialize model parameters
    m_weights = Eigen::VectorXd::Zero(X.cols());
    m_bias = 0.0;
    
    if (m_useGradientDescent) {
        trainUsingGradientDescent(X, y);
    } else {
        trainUsingNormalEquation(X, y);
    }
    
    return true;
}

DataSet LinearRegressionModel::predict(const DataSet& features) const {
    const auto& data = features.getData();
    
    if (data.empty() || m_weights.size() == 0) {
        return DataSet();
    }
    
    Eigen::MatrixXd X = datasetToEigenMatrix(features);
    Eigen::VectorXd predictions = X * m_weights + Eigen::VectorXd::Constant(X.rows(), m_bias);
    
    // Convert predictions to DataSet
    DataSet::DataMatrix predMatrix;
    predMatrix.reserve(predictions.size());
    
    for (int i = 0; i < predictions.size(); ++i) {
        predMatrix.push_back({predictions(i)});
    }
    
    return DataSet(predMatrix);
}

ModelMetrics LinearRegressionModel::evaluate(const DataSet& testFeatures, 
                                           const DataSet& testLabels) const {
    ModelMetrics metrics;
    
    // Make predictions
    DataSet predictions = predict(testFeatures);
    
    const auto& predData = predictions.getData();
    const auto& trueLabels = testLabels.getData();
    
    if (predData.empty() || trueLabels.empty() || predData.size() != trueLabels.size()) {
        return metrics;
    }
    
    // Calculate Mean Squared Error
    double mse = 0.0;
    double sumTrue = 0.0;
    double sumSquaredError = 0.0;
    double sumSquaredTotal = 0.0;
    double meanTrue = 0.0;
    
    // First pass - calculate mean of true values
    for (const auto& row : trueLabels) {
        if (!row.empty()) {
            sumTrue += row[0];
        }
    }
    meanTrue = sumTrue / trueLabels.size();
    
    // Second pass - calculate MSE and components for R² score
    for (size_t i = 0; i < predData.size(); ++i) {
        if (!predData[i].empty() && !trueLabels[i].empty()) {
            double pred = predData[i][0];
            double actual = trueLabels[i][0];
            double error = pred - actual;
            
            mse += error * error;
            sumSquaredError += error * error;
            sumSquaredTotal += (actual - meanTrue) * (actual - meanTrue);
        }
    }
    
    mse /= predData.size();
    metrics.meanSquaredError = mse;
    
    // Calculate R² score
    if (sumSquaredTotal > 0) {
        metrics.r2Score = 1.0 - (sumSquaredError / sumSquaredTotal);
    } else {
        metrics.r2Score = 0.0; // All true values are identical
    }
    
    return metrics;
}

bool LinearRegressionModel::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    // Save model type
    file << "MODEL_TYPE=LINEAR_REGRESSION\n";
    
    // Save hyperparameters
    file << "LEARNING_RATE=" << m_learningRate << "\n";
    file << "MAX_ITERATIONS=" << m_maxIterations << "\n";
    file << "USE_GRADIENT_DESCENT=" << (m_useGradientDescent ? "1" : "0") << "\n";
    
    // Save bias
    file << "BIAS=" << m_bias << "\n";
    
    // Save weights
    file << "WEIGHTS_COUNT=" << m_weights.size() << "\n";
    file << "WEIGHTS=";
    for (int i = 0; i < m_weights.size(); ++i) {
        file << m_weights(i);
        if (i < m_weights.size() - 1) {
            file << ",";
        }
    }
    file << "\n";
    
    return file.good();
}

bool LinearRegressionModel::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    std::string line;
    std::string modelType;
    int weightsCount = 0;
    
    while (std::getline(file, line)) {
        auto pos = line.find('=');
        if (pos == std::string::npos) continue;
        
        std::string key = line.substr(0, pos);
        std::string value = line.substr(pos + 1);
        
        if (key == "MODEL_TYPE") {
            modelType = value;
            if (modelType != "LINEAR_REGRESSION") {
                return false; // Wrong model type
            }
        } else if (key == "LEARNING_RATE") {
            m_learningRate = std::stod(value);
        } else if (key == "MAX_ITERATIONS") {
            m_maxIterations = std::stoi(value);
        } else if (key == "USE_GRADIENT_DESCENT") {
            m_useGradientDescent = (value == "1");
        } else if (key == "BIAS") {
            m_bias = std::stod(value);
        } else if (key == "WEIGHTS_COUNT") {
            weightsCount = std::stoi(value);
            m_weights.resize(weightsCount);
        } else if (key == "WEIGHTS") {
            std::vector<std::string> weightStrs = Utils::splitString(value, ',');
            if (weightStrs.size() != static_cast<size_t>(weightsCount)) {
                return false; // Wrong number of weights
            }
            
            for (int i = 0; i < weightsCount; ++i) {
                m_weights(i) = std::stod(weightStrs[i]);
            }
        }
    }
    
    return true;
}

std::string LinearRegressionModel::getParameters() const {
    std::stringstream ss;
    ss << "LinearRegressionModel Parameters:" << std::endl;
    ss << "  Learning Rate: " << m_learningRate << std::endl;
    ss << "  Max Iterations: " << m_maxIterations << std::endl;
    ss << "  Method: " << (m_useGradientDescent ? "Gradient Descent" : "Normal Equation") << std::endl;
    ss << "  Bias: " << m_bias << std::endl;
    ss << "  Weights: [";
    
    for (int i = 0; i < m_weights.size(); ++i) {
        ss << m_weights(i);
        if (i < m_weights.size() - 1) {
            ss << ", ";
        }
    }
    ss << "]" << std::endl;
    
    return ss.str();
}

Eigen::MatrixXd LinearRegressionModel::datasetToEigenMatrix(const DataSet& dataset) const {
    const auto& data = dataset.getData();
    
    if (data.empty()) {
        return Eigen::MatrixXd();
    }
    
    int rows = static_cast<int>(data.size());
    int cols = static_cast<int>(data[0].size());
    
    Eigen::MatrixXd matrix(rows, cols);
    
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            matrix(i, j) = data[i][j];
        }
    }
    
    return matrix;
}

Eigen::VectorXd LinearRegressionModel::labelsToEigenVector(const DataSet& dataset) const {
    const auto& labels = dataset.getLabels();
    
    int size = static_cast<int>(labels.size());
    Eigen::VectorXd vector(size);
    
    for (int i = 0; i < size; ++i) {
        vector(i) = labels[i];
    }
    
    return vector;
}

void LinearRegressionModel::trainUsingNormalEquation(const Eigen::MatrixXd& X, const Eigen::VectorXd& y) {
    // Normal Equation: θ = (X^T X)^(-1) X^T y
    m_weights = (X.transpose() * X).inverse() * X.transpose() * y;
    m_bias = 0.0; // In this implementation, bias is included in X
}

void LinearRegressionModel::trainUsingGradientDescent(const Eigen::MatrixXd& X, const Eigen::VectorXd& y) {
    int numSamples = X.rows();
    double prevCost = std::numeric_limits<double>::max();
    
    for (int iter = 0; iter < m_maxIterations; ++iter) {
        // Compute predictions: h = X * weights + bias
        Eigen::VectorXd h = X * m_weights + Eigen::VectorXd::Constant(numSamples, m_bias);
        
        // Compute gradients
        Eigen::VectorXd gradient = X.transpose() * (h - y) / numSamples;
        double biasGradient = (h - y).sum() / numSamples;
        
        // Update parameters
        m_weights -= m_learningRate * gradient;
        m_bias -= m_learningRate * biasGradient;
        
        // Check convergence
        Eigen::VectorXd newH = X * m_weights + Eigen::VectorXd::Constant(numSamples, m_bias);
        double cost = (newH - y).squaredNorm() / (2 * numSamples);
        
        if (std::abs(prevCost - cost) < 1e-6) {
            break;
        }
        
        prevCost = cost;
    }
}

// LogisticRegressionModel implementation with Eigen

LogisticRegressionModel::LogisticRegressionModel(double learningRate, int maxIterations)
    : m_weights(), 
      m_bias(0.0),
      m_learningRate(learningRate),
      m_maxIterations(maxIterations) {}

bool LogisticRegressionModel::train(const DataSet& dataset) {
    const auto& data = dataset.getData();
    const auto& labels = dataset.getLabels();
    
    if (data.empty() || labels.empty() || data.size() != labels.size()) {
        return false;
    }
    
    Eigen::MatrixXd X = datasetToEigenMatrix(dataset);
    Eigen::VectorXd y = labelsToEigenVector(dataset);
    
    // Initialize model parameters
    m_weights = Eigen::VectorXd::Zero(X.cols());
    m_bias = 0.0;
    
    int numSamples = X.rows();
    double prevCost = std::numeric_limits<double>::max();
    
    // Gradient descent for logistic regression
    for (int iter = 0; iter < m_maxIterations; ++iter) {
        // Compute predictions: p = sigmoid(X * weights + bias)
        Eigen::VectorXd z = X * m_weights + Eigen::VectorXd::Constant(numSamples, m_bias);
        Eigen::VectorXd p = sigmoid(z);
        
        // Compute gradients
        Eigen::VectorXd gradient = X.transpose() * (p - y) / numSamples;
        double biasGradient = (p - y).sum() / numSamples;
        
        // Update parameters
        m_weights -= m_learningRate * gradient;
        m_bias -= m_learningRate * biasGradient;
        
        // Compute cost
        Eigen::VectorXd newZ = X * m_weights + Eigen::VectorXd::Constant(numSamples, m_bias);
        Eigen::VectorXd newP = sigmoid(newZ);
        
        // Binary cross-entropy loss
        double cost = 0.0;
        for (int i = 0; i < numSamples; ++i) {
            double pi = newP(i);
            double yi = y(i);
            // Avoid log(0) by adding a small epsilon
            double epsilon = 1e-15;
            cost -= (yi * std::log(pi + epsilon) + (1 - yi) * std::log(1 - pi + epsilon));
        }
        cost /= numSamples;
        
        // Check convergence
        if (std::abs(prevCost - cost) < 1e-6) {
            break;
        }
        
        prevCost = cost;
    }
    
    return true;
}

DataSet LogisticRegressionModel::predict(const DataSet& features) const {
    const auto& data = features.getData();
    
    if (data.empty() || m_weights.size() == 0) {
        return DataSet();
    }
    
    Eigen::MatrixXd X = datasetToEigenMatrix(features);
    Eigen::VectorXd z = X * m_weights + Eigen::VectorXd::Constant(X.rows(), m_bias);
    Eigen::VectorXd probabilities = sigmoid(z);
    
    // Convert probabilities to DataSet
    DataSet::DataMatrix predMatrix;
    predMatrix.reserve(probabilities.size());
    
    for (int i = 0; i < probabilities.size(); ++i) {
        predMatrix.push_back({probabilities(i)});
    }
    
    return DataSet(predMatrix);
}

ModelMetrics LogisticRegressionModel::evaluate(const DataSet& testFeatures, 
                                               const DataSet& testLabels) const {
    ModelMetrics metrics;
    
    // Make predictions
    DataSet probDS = predict(testFeatures);
    
    const auto& probabilities = probDS.getData();
    const auto& trueLabels = testLabels.getData();
    
    if (probabilities.empty() || trueLabels.empty() || probabilities.size() != trueLabels.size()) {
        return metrics;
    }
    
    // Convert probabilities to binary predictions (threshold = 0.5)
    std::vector<int> predictions;
    predictions.reserve(probabilities.size());
    
    for (const auto& row : probabilities) {
        if (!row.empty()) {
            predictions.push_back(row[0] >= 0.5 ? 1 : 0);
        }
    }
    
    // Calculate metrics
    int tp = 0; // True positive
    int fp = 0; // False positive
    int tn = 0; // True negative
    int fn = 0; // False negative
    
    for (size_t i = 0; i < predictions.size(); ++i) {
        int pred = predictions[i];
        int actual = (trueLabels[i][0] >= 0.5) ? 1 : 0;
        
        if (pred == 1 && actual == 1) tp++;
        else if (pred == 1 && actual == 0) fp++;
        else if (pred == 0 && actual == 0) tn++;
        else if (pred == 0 && actual == 1) fn++;
    }
    
    // Calculate metrics
    metrics.accuracy = (tp + tn) / static_cast<double>(tp + tn + fp + fn);
    
    if (tp + fp > 0) {
        metrics.precision = tp / static_cast<double>(tp + fp);
    }
    
    if (tp + fn > 0) {
        metrics.recall = tp / static_cast<double>(tp + fn);
    }
    
    if (metrics.precision + metrics.recall > 0) {
        metrics.f1Score = 2 * metrics.precision * metrics.recall / (metrics.precision + metrics.recall);
    }
    
    return metrics;
}

bool LogisticRegressionModel::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    // Save model type
    file << "MODEL_TYPE=LOGISTIC_REGRESSION\n";
    
    // Save hyperparameters
    file << "LEARNING_RATE=" << m_learningRate << "\n";
    file << "MAX_ITERATIONS=" << m_maxIterations << "\n";
    
    // Save bias
    file << "BIAS=" << m_bias << "\n";
    
    // Save weights
    file << "WEIGHTS_COUNT=" << m_weights.size() << "\n";
    file << "WEIGHTS=";
    for (int i = 0; i < m_weights.size(); ++i) {
        file << m_weights(i);
        if (i < m_weights.size() - 1) {
            file << ",";
        }
    }
    file << "\n";
    
    return file.good();
}

bool LogisticRegressionModel::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    std::string line;
    std::string modelType;
    int weightsCount = 0;
    
    while (std::getline(file, line)) {
        auto pos = line.find('=');
        if (pos == std::string::npos) continue;
        
        std::string key = line.substr(0, pos);
        std::string value = line.substr(pos + 1);
        
        if (key == "MODEL_TYPE") {
            modelType = value;
            if (modelType != "LOGISTIC_REGRESSION") {
                return false; // Wrong model type
            }
        } else if (key == "LEARNING_RATE") {
            m_learningRate = std::stod(value);
        } else if (key == "MAX_ITERATIONS") {
            m_maxIterations = std::stoi(value);
        } else if (key == "BIAS") {
            m_bias = std::stod(value);
        } else if (key == "WEIGHTS_COUNT") {
            weightsCount = std::stoi(value);
            m_weights.resize(weightsCount);
        } else if (key == "WEIGHTS") {
            std::vector<std::string> weightStrs = Utils::splitString(value, ',');
            if (weightStrs.size() != static_cast<size_t>(weightsCount)) {
                return false; // Wrong number of weights
            }
            
            for (int i = 0; i < weightsCount; ++i) {
                m_weights(i) = std::stod(weightStrs[i]);
            }
        }
    }
    
    return true;
}

std::string LogisticRegressionModel::getParameters() const {
    std::stringstream ss;
    ss << "LogisticRegressionModel Parameters:" << std::endl;
    ss << "  Learning Rate: " << m_learningRate << std::endl;
    ss << "  Max Iterations: " << m_maxIterations << std::endl;
    ss << "  Bias: " << m_bias << std::endl;
    ss << "  Weights: [";
    
    for (int i = 0; i < m_weights.size(); ++i) {
        ss << m_weights(i);
        if (i < m_weights.size() - 1) {
            ss << ", ";
        }
    }
    ss << "]" << std::endl;
    
    return ss.str();
}

Eigen::MatrixXd LogisticRegressionModel::datasetToEigenMatrix(const DataSet& dataset) const {
    const auto& data = dataset.getData();
    
    if (data.empty()) {
        return Eigen::MatrixXd();
    }
    
    int rows = static_cast<int>(data.size());
    int cols = static_cast<int>(data[0].size());
    
    Eigen::MatrixXd matrix(rows, cols);
    
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            matrix(i, j) = data[i][j];
        }
    }
    
    return matrix;
}

Eigen::VectorXd LogisticRegressionModel::labelsToEigenVector(const DataSet& dataset) const {
    const auto& labels = dataset.getLabels();
    
    int size = static_cast<int>(labels.size());
    Eigen::VectorXd vector(size);
    
    for (int i = 0; i < size; ++i) {
        vector(i) = labels[i];
    }
    
    return vector;
}

double LogisticRegressionModel::sigmoid(double x) const {
    return 1.0 / (1.0 + std::exp(-x));
}

Eigen::VectorXd LogisticRegressionModel::sigmoid(const Eigen::VectorXd& x) const {
    Eigen::VectorXd result(x.size());
    for (int i = 0; i < x.size(); ++i) {
        result(i) = sigmoid(x(i));
    }
    return result;
}

// KMeansModel implementation with Eigen

KMeansModel::KMeansModel(int k, int maxIterations)
    : m_k(k), m_maxIterations(maxIterations), m_centroids() {}

bool KMeansModel::train(const DataSet& dataset) {
    const auto& data = dataset.getData();
    
    if (data.empty() || static_cast<int>(data.size()) < m_k) {
        return false;
    }
    
    Eigen::MatrixXd X = datasetToEigenMatrix(dataset);
    
    // Initialize centroids
    m_centroids = initializeCentroids(X);
    
    // Main K-means loop
    for (int iter = 0; iter < m_maxIterations; ++iter) {
        // Assign data points to closest centroids
        Eigen::VectorXi labels = assignToClusters(X);
        
        // Update centroids
        Eigen::MatrixXd newCentroids = updateCentroids(X, labels);
        
        // Check for convergence
        if ((m_centroids - newCentroids).norm() < 1e-6) {
            break;
        }
        
        m_centroids = newCentroids;
    }
    
    return true;
}

DataSet KMeansModel::predict(const DataSet& features) const {
    const auto& data = features.getData();
    
    if (data.empty() || m_centroids.rows() == 0) {
        return DataSet();
    }
    
    Eigen::MatrixXd X = datasetToEigenMatrix(features);
    Eigen::VectorXi labels = assignToClusters(X);
    
    // Convert labels to DataSet
    DataSet::DataMatrix predMatrix;
    predMatrix.reserve(labels.size());
    
    for (int i = 0; i < labels.size(); ++i) {
        predMatrix.push_back({static_cast<double>(labels(i))});
    }
    
    return DataSet(predMatrix);
}

ModelMetrics KMeansModel::evaluate(const DataSet& testFeatures, 
                                  const DataSet& testLabels) const {
    ModelMetrics metrics;
    
    // For clustering tasks, evaluation is typically measured with different metrics
    // than classification or regression tasks.
    // Here we'll compute inertia (sum of squared distances to the nearest centroid).
    
    const auto& data = testFeatures.getData();
    
    if (data.empty() || m_centroids.rows() == 0) {
        return metrics;
    }
    
    Eigen::MatrixXd X = datasetToEigenMatrix(testFeatures);
    Eigen::VectorXi labels = assignToClusters(X);
    
    // Calculate inertia
    double inertia = 0.0;
    
    for (int i = 0; i < X.rows(); ++i) {
        Eigen::VectorXd point = X.row(i);
        Eigen::VectorXd centroid = m_centroids.row(labels(i));
        inertia += computeDistance(point, centroid);
    }
    
    metrics.addMetric("inertia", inertia);
    
    // If true labels are provided, we can calculate purity
    if (!testLabels.getData().empty()) {
        // For simplicity, we'll use the first column of testLabels as the true labels
        std::vector<int> trueLabelIndices;
        trueLabelIndices.reserve(testLabels.getData().size());
        
        for (const auto& row : testLabels.getData()) {
            if (!row.empty()) {
                trueLabelIndices.push_back(static_cast<int>(row[0]));
            }
        }
        
        // Calculate purity (the percentage of correctly classified instances)
        if (!trueLabelIndices.empty()) {
            // Count the occurrences of each true label in each cluster
            std::vector<std::unordered_map<int, int>> clusterLabels(m_k);
            
            for (int i = 0; i < labels.size(); ++i) {
                int cluster = labels(i);
                int trueLabel = trueLabelIndices[i];
                clusterLabels[cluster][trueLabel]++;
            }
            
            // For each cluster, find the majority true label
            int correctlyClassified = 0;
            for (const auto& cluster : clusterLabels) {
                int maxCount = 0;
                for (const auto& [label, count] : cluster) {
                    maxCount = std::max(maxCount, count);
                }
                correctlyClassified += maxCount;
            }
            
            double purity = static_cast<double>(correctlyClassified) / trueLabelIndices.size();
            metrics.addMetric("purity", purity);
        }
    }
    
    return metrics;
}

bool KMeansModel::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    // Save model type
    file << "MODEL_TYPE=K_MEANS\n";
    
    // Save hyperparameters
    file << "K=" << m_k << "\n";
    file << "MAX_ITERATIONS=" << m_maxIterations << "\n";
    
    // Save centroids
    file << "CENTROID_ROWS=" << m_centroids.rows() << "\n";
    file << "CENTROID_COLS=" << m_centroids.cols() << "\n";
    
    file << "CENTROIDS=\n";
    for (int i = 0; i < m_centroids.rows(); ++i) {
        for (int j = 0; j < m_centroids.cols(); ++j) {
            file << m_centroids(i, j);
            if (j < m_centroids.cols() - 1) {
                file << ",";
            }
        }
        file << "\n";
    }
    
    return file.good();
}

bool KMeansModel::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    std::string line;
    std::string modelType;
    int centroidRows = 0;
    int centroidCols = 0;
    
    while (std::getline(file, line)) {
        auto pos = line.find('=');
        if (pos == std::string::npos) {
            // This might be a centroid data line
            if (centroidRows > 0 && centroidCols > 0 && m_centroids.rows() > 0) {
                int rowIdx = 0;
                while (rowIdx < m_centroids.rows() && std::getline(file, line)) {
                    std::vector<std::string> valueStrs = Utils::splitString(line, ',');
                    if (valueStrs.size() != static_cast<size_t>(centroidCols)) {
                        return false; // Wrong number of values
                    }
                    
                    for (int j = 0; j < centroidCols; ++j) {
                        m_centroids(rowIdx, j) = std::stod(valueStrs[j]);
                    }
                    
                    rowIdx++;
                }
            }
            continue;
        }
        
        std::string key = line.substr(0, pos);
        std::string value = line.substr(pos + 1);
        
        if (key == "MODEL_TYPE") {
            modelType = value;
            if (modelType != "K_MEANS") {
                return false; // Wrong model type
            }
        } else if (key == "K") {
            m_k = std::stoi(value);
        } else if (key == "MAX_ITERATIONS") {
            m_maxIterations = std::stoi(value);
        } else if (key == "CENTROID_ROWS") {
            centroidRows = std::stoi(value);
        } else if (key == "CENTROID_COLS") {
            centroidCols = std::stoi(value);
            if (centroidRows > 0 && centroidCols > 0) {
                m_centroids.resize(centroidRows, centroidCols);
            }
        } else if (key == "CENTROIDS") {
            // The centroids data follows on subsequent lines
            int rowIdx = 0;
            while (rowIdx < centroidRows && std::getline(file, line)) {
                std::vector<std::string> valueStrs = Utils::splitString(line, ',');
                if (valueStrs.size() != static_cast<size_t>(centroidCols)) {
                    return false; // Wrong number of values
                }
                
                for (int j = 0; j < centroidCols; ++j) {
                    m_centroids(rowIdx, j) = std::stod(valueStrs[j]);
                }
                
                rowIdx++;
            }
        }
    }
    
    return true;
}

std::string KMeansModel::getParameters() const {
    std::stringstream ss;
    ss << "KMeansModel Parameters:" << std::endl;
    ss << "  K: " << m_k << std::endl;
    ss << "  Max Iterations: " << m_maxIterations << std::endl;
    ss << "  Centroids: [" << m_centroids.rows() << " x " << m_centroids.cols() << "]" << std::endl;
    
    return ss.str();
}

DataSet KMeansModel::getCentroids() const {
    DataSet::DataMatrix centroidMatrix;
    centroidMatrix.reserve(m_centroids.rows());
    
    for (int i = 0; i < m_centroids.rows(); ++i) {
        std::vector<double> row;
        row.reserve(m_centroids.cols());
        
        for (int j = 0; j < m_centroids.cols(); ++j) {
            row.push_back(m_centroids(i, j));
        }
        
        centroidMatrix.push_back(row);
    }
    
    return DataSet(centroidMatrix);
}

Eigen::MatrixXd KMeansModel::datasetToEigenMatrix(const DataSet& dataset) const {
    const auto& data = dataset.getData();
    
    if (data.empty()) {
        return Eigen::MatrixXd();
    }
    
    int rows = static_cast<int>(data.size());
    int cols = static_cast<int>(data[0].size());
    
    Eigen::MatrixXd matrix(rows, cols);
    
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            matrix(i, j) = data[i][j];
        }
    }
    
    return matrix;
}

Eigen::MatrixXd KMeansModel::initializeCentroids(const Eigen::MatrixXd& data) {
    Eigen::MatrixXd centroids(m_k, data.cols());
    
    // K-means++ initialization
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, data.rows() - 1);
    
    // Choose the first centroid randomly
    int firstIdx = dist(gen);
    centroids.row(0) = data.row(firstIdx);
    
    // Choose remaining centroids
    for (int i = 1; i < m_k; ++i) {
        Eigen::VectorXd distances(data.rows());
        
        // Compute distance to closest existing centroid for each point
        for (int j = 0; j < data.rows(); ++j) {
            double minDist = std::numeric_limits<double>::max();
            for (int c = 0; c < i; ++c) {
                double dist = computeDistance(data.row(j), centroids.row(c));
                minDist = std::min(minDist, dist);
            }
            distances(j) = minDist;
        }
        
        // Choose the next centroid with probability proportional to squared distance
        double sum = distances.sum();
        std::uniform_real_distribution<double> realDist(0.0, sum);
        double value = realDist(gen);
        
        double cumulativeSum = 0.0;
        int nextIdx = 0;
        for (int j = 0; j < data.rows(); ++j) {
            cumulativeSum += distances(j);
            if (cumulativeSum >= value) {
                nextIdx = j;
                break;
            }
        }
        
        centroids.row(i) = data.row(nextIdx);
    }
    
    return centroids;
}

Eigen::VectorXi KMeansModel::assignToClusters(const Eigen::MatrixXd& data) {
    Eigen::VectorXi labels(data.rows());
    
    for (int i = 0; i < data.rows(); ++i) {
        double minDist = std::numeric_limits<double>::max();
        int bestCluster = 0;
        
        for (int j = 0; j < m_centroids.rows(); ++j) {
            double dist = computeDistance(data.row(i), m_centroids.row(j));
            if (dist < minDist) {
                minDist = dist;
                bestCluster = j;
            }
        }
        
        labels(i) = bestCluster;
    }
    
    return labels;
}

Eigen::MatrixXd KMeansModel::updateCentroids(const Eigen::MatrixXd& data, const Eigen::VectorXi& labels) {
    Eigen::MatrixXd newCentroids = Eigen::MatrixXd::Zero(m_k, data.cols());
    Eigen::VectorXi counts = Eigen::VectorXi::Zero(m_k);
    
    for (int i = 0; i < data.rows(); ++i) {
        int cluster = labels(i);
        newCentroids.row(cluster) += data.row(i);
        counts(cluster)++;
    }
    
    // Normalize by cluster size
    for (int i = 0; i < m_k; ++i) {
        if (counts(i) > 0) {
            newCentroids.row(i) /= counts(i);
        } else {
            // If a cluster is empty, keep its old centroid
            newCentroids.row(i) = m_centroids.row(i);
        }
    }
    
    return newCentroids;
}

double KMeansModel::computeDistance(const Eigen::VectorXd& a, const Eigen::VectorXd& b) const {
    return (a - b).squaredNorm(); // Squared Euclidean distance
}

#else
// Basic Linear Regression implementation without Eigen

LinearRegressionModel::LinearRegressionModel(double learningRate, 
                                           int maxIterations,
                                           bool useGradientDescent)
    : m_weights(), 
      m_bias(0.0),
      m_learningRate(learningRate),
      m_maxIterations(maxIterations),
      m_useGradientDescent(useGradientDescent) {}

bool LinearRegressionModel::train(const DataSet& dataset) {
    const auto& data = dataset.getData();
    const auto& labels = dataset.getLabels();
    
    if (data.empty() || labels.empty() || data.size() != labels.size()) {
        return false;
    }
    
    // Initialize weights (one per feature)
    size_t numFeatures = data[0].size();
    m_weights.resize(numFeatures, 0.0);
    m_bias = 0.0;
    
    if (m_useGradientDescent) {
        // Gradient descent
        for (int iter = 0; iter < m_maxIterations; ++iter) {
            double cost = 0.0;
            
            // Calculate gradients
            std::vector<double> gradients(numFeatures, 0.0);
            double biasGradient = 0.0;
            
            for (size_t i = 0; i < data.size(); ++i) {
                const auto& x = data[i];
                double y = labels[i];
                
                // Predict
                double prediction = m_bias;
                for (size_t j = 0; j < numFeatures; ++j) {
                    prediction += m_weights[j] * x[j];
                }
                
                // Error
                double error = prediction - y;
                cost += error * error;
                
                // Update gradients
                for (size_t j = 0; j < numFeatures; ++j) {
                    gradients[j] += error * x[j];
                }
                biasGradient += error;
            }
            
            // Average gradients
            for (auto& grad : gradients) {
                grad /= data.size();
            }
            biasGradient /= data.size();
            
            // Update weights and bias
            for (size_t j = 0; j < numFeatures; ++j) {
                m_weights[j] -= m_learningRate * gradients[j];
            }
            m_bias -= m_learningRate * biasGradient;
            
            // Check convergence
            cost /= data.size();
            if (iter > 0 && std::abs(cost) < 1e-6) {
                break;
            }
        }
    } else {
        // Normal Equation (simplified implementation without matrix operations)
        // This is not an efficient implementation of the normal equation
        // It's provided here only as a fallback when Eigen is not available
        
        // X'X
        std::vector<std::vector<double>> xtx(numFeatures, std::vector<double>(numFeatures, 0.0));
        for (size_t i = 0; i < data.size(); ++i) {
            for (size_t j = 0; j < numFeatures; ++j) {
                for (size_t k = 0; k < numFeatures; ++k) {
                    xtx[j][k] += data[i][j] * data[i][k];
                }
            }
        }
        
        // X'y
        std::vector<double> xty(numFeatures, 0.0);
        for (size_t i = 0; i < data.size(); ++i) {
            for (size_t j = 0; j < numFeatures; ++j) {
                xty[j] += data[i][j] * labels[i];
            }
        }
        
        // Solve for weights (simplified, not robust)
        // This is a very basic approach and would fail for ill-conditioned problems
        // A proper implementation would use matrix inversion or decomposition techniques
        m_weights = xty;
        for (size_t i = 0; i < numFeatures; ++i) {
            if (xtx[i][i] != 0) {
                m_weights[i] /= xtx[i][i];
            }
        }
    }
    
    return true;
}

DataSet LinearRegressionModel::predict(const DataSet& features) const {
    const auto& data = features.getData();
    
    if (data.empty() || m_weights.empty()) {
        return DataSet();
    }
    
    DataSet::DataMatrix predictions;
    predictions.reserve(data.size());
    
    for (const auto& x : data) {
        double pred = m_bias;
        for (size_t j = 0; j < m_weights.size() && j < x.size(); ++j) {
            pred += m_weights[j] * x[j];
        }
        predictions.push_back({pred});
    }
    
    return DataSet(predictions);
}

ModelMetrics LinearRegressionModel::evaluate(const DataSet& testFeatures, 
                                           const DataSet& testLabels) const {
    ModelMetrics metrics;
    
    DataSet predictions = predict(testFeatures);
    
    const auto& predData = predictions.getData();
    const auto& trueLabels = testLabels.getData();
    
    if (predData.empty() || trueLabels.empty() || predData.size() != trueLabels.size()) {
        return metrics;
    }
    
    double mse = 0.0;
    double sumTrue = 0.0;
    
    // Calculate mean of true values
    for (const auto& row : trueLabels) {
        if (!row.empty()) {
            sumTrue += row[0];
        }
    }
    double meanTrue = sumTrue / trueLabels.size();
    
    double sumSquaredError = 0.0;
    double sumSquaredTotal = 0.0;
    
    for (size_t i = 0; i < predData.size(); ++i) {
        if (!predData[i].empty() && !trueLabels[i].empty()) {
            double error = predData[i][0] - trueLabels[i][0];
            mse += error * error;
            
            sumSquaredError += error * error;
            sumSquaredTotal += std::pow(trueLabels[i][0] - meanTrue, 2);
        }
    }
    
    mse /= predData.size();
    metrics.meanSquaredError = mse;
    
    if (sumSquaredTotal > 0) {
        metrics.r2Score = 1.0 - (sumSquaredError / sumSquaredTotal);
    }
    
    return metrics;
}

bool LinearRegressionModel::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    file << "MODEL_TYPE=LINEAR_REGRESSION\n";
    file << "LEARNING_RATE=" << m_learningRate << "\n";
    file << "MAX_ITERATIONS=" << m_maxIterations << "\n";
    file << "USE_GRADIENT_DESCENT=" << (m_useGradientDescent ? "1" : "0") << "\n";
    file << "BIAS=" << m_bias << "\n";
    file << "WEIGHTS_COUNT=" << m_weights.size() << "\n";
    file << "WEIGHTS=";
    
    for (size_t i = 0; i < m_weights.size(); ++i) {
        file << m_weights[i];
        if (i < m_weights.size() - 1) {
            file << ",";
        }
    }
    file << "\n";
    
    return file.good();
}

bool LinearRegressionModel::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    std::string line;
    std::string modelType;
    size_t weightsCount = 0;
    
    while (std::getline(file, line)) {
        auto pos = line.find('=');
        if (pos == std::string::npos) continue;
        
        std::string key = line.substr(0, pos);
        std::string value = line.substr(pos + 1);
        
        if (key == "MODEL_TYPE") {
            modelType = value;
            if (modelType != "LINEAR_REGRESSION") {
                return false;
            }
        } else if (key == "LEARNING_RATE") {
            m_learningRate = std::stod(value);
        } else if (key == "MAX_ITERATIONS") {
            m_maxIterations = std::stoi(value);
        } else if (key == "USE_GRADIENT_DESCENT") {
            m_useGradientDescent = (value == "1");
        } else if (key == "BIAS") {
            m_bias = std::stod(value);
        } else if (key == "WEIGHTS_COUNT") {
            weightsCount = std::stoul(value);
            m_weights.resize(weightsCount);
        } else if (key == "WEIGHTS") {
            std::vector<std::string> weightStrs = Utils::splitString(value, ',');
            if (weightStrs.size() != weightsCount) {
                return false;
            }
            
            for (size_t i = 0; i < weightsCount; ++i) {
                m_weights[i] = std::stod(weightStrs[i]);
            }
        }
    }
    
    return true;
}

std::string LinearRegressionModel::getParameters() const {
    std::stringstream ss;
    ss << "LinearRegressionModel Parameters:" << std::endl;
    ss << "  Learning Rate: " << m_learningRate << std::endl;
    ss << "  Max Iterations: " << m_maxIterations << std::endl;
    ss << "  Method: " << (m_useGradientDescent ? "Gradient Descent" : "Normal Equation") << std::endl;
    ss << "  Bias: " << m_bias << std::endl;
    ss << "  Weights: [";
    
    for (size_t i = 0; i < m_weights.size(); ++i) {
        ss << m_weights[i];
        if (i < m_weights.size() - 1) {
            ss << ", ";
        }
    }
    ss << "]" << std::endl;
    
    return ss.str();
}
#endif
