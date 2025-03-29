/**
 * @file DataManager.cpp
 * @brief Implementation of the DataManager class
 */

#include "DataManager.h"
#include "ErrorHandler.h"
#include "Utils.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <random>
#include <cmath>

// DataSet Implementation

DataSet::DataSet(DataMatrix data, LabelVector labels, FeatureNames featureNames)
    : m_data(std::move(data)), 
      m_labels(std::move(labels)), 
      m_featureNames(std::move(featureNames)) {}

void DataSet::addDataRow(const std::vector<double>& dataRow, double label) {
    m_data.push_back(dataRow);
    if (!m_labels.empty() || label != 0.0) {
        m_labels.push_back(label);
    }
}

std::pair<size_t, size_t> DataSet::dimensions() const {
    if (m_data.empty()) {
        return {0, 0};
    }
    return {m_data.size(), m_data[0].size()};
}

std::pair<DataSet, DataSet> DataSet::split(double testRatio) const {
    if (testRatio < 0.0 || testRatio > 1.0) {
        throw std::invalid_argument("Test ratio must be between 0.0 and 1.0");
    }
    
    // Create copies of the data and labels that we can shuffle
    DataMatrix dataCopy = m_data;
    LabelVector labelsCopy = m_labels;
    
    // Create index vector to shuffle
    std::vector<size_t> indices(dataCopy.size());
    std::iota(indices.begin(), indices.end(), 0);
    
    // Shuffle the indices
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(indices.begin(), indices.end(), g);
    
    // Calculate the split point
    size_t testSize = static_cast<size_t>(std::round(dataCopy.size() * testRatio));
    size_t trainSize = dataCopy.size() - testSize;
    
    // Create training and testing datasets
    DataMatrix trainData, testData;
    LabelVector trainLabels, testLabels;
    
    trainData.reserve(trainSize);
    testData.reserve(testSize);
    
    if (!labelsCopy.empty()) {
        trainLabels.reserve(trainSize);
        testLabels.reserve(testSize);
    }
    
    // Populate the training and testing datasets
    for (size_t i = 0; i < indices.size(); ++i) {
        size_t idx = indices[i];
        if (i < trainSize) {
            trainData.push_back(dataCopy[idx]);
            if (!labelsCopy.empty()) {
                trainLabels.push_back(labelsCopy[idx]);
            }
        } else {
            testData.push_back(dataCopy[idx]);
            if (!labelsCopy.empty()) {
                testLabels.push_back(labelsCopy[idx]);
            }
        }
    }
    
    return {
        DataSet(trainData, trainLabels, m_featureNames),
        DataSet(testData, testLabels, m_featureNames)
    };
}

// DataManager Implementation

DataSet DataManager::loadFromFile(const std::string& filename, 
                                 DataFormat format,
                                 bool hasHeader,
                                 char delimiter) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }
    
    switch (format) {
        case DataFormat::CSV:
            return loadCSV(file, hasHeader, delimiter);
        case DataFormat::JSON:
            return loadJSON(file);
        case DataFormat::TXT:
            return loadTXT(file, delimiter);
        default:
            throw std::invalid_argument("Unsupported file format");
    }
}

bool DataManager::saveToFile(const DataSet& dataset,
                            const std::string& filename,
                            DataFormat format,
                            bool includeHeader,
                            char delimiter) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file for writing: " + filename);
    }
    
    switch (format) {
        case DataFormat::CSV:
            return saveCSV(dataset, file, includeHeader, delimiter);
        case DataFormat::JSON:
            return saveJSON(dataset, file);
        case DataFormat::TXT:
            return saveTXT(dataset, file, delimiter);
        default:
            throw std::invalid_argument("Unsupported file format");
    }
}

DataSet DataManager::loadCSV(std::ifstream& file, bool hasHeader, char delimiter) {
    std::string line;
    std::vector<std::string> featureNames;
    DataSet::DataMatrix data;
    
    // Read header if present
    if (hasHeader && std::getline(file, line)) {
        featureNames = Utils::splitString(line, delimiter);
    }
    
    // Read data rows
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        std::vector<std::string> tokens = Utils::splitString(line, delimiter);
        std::vector<double> row;
        row.reserve(tokens.size());
        
        for (const auto& token : tokens) {
            row.push_back(Utils::stringToNumber<double>(token, 0.0));
        }
        
        if (!row.empty()) {
            data.push_back(row);
        }
    }
    
    return DataSet(data, {}, featureNames);
}

DataSet DataManager::loadJSON(std::ifstream& file) {
    // Simple JSON parsing - for production code, use a proper JSON library
    std::string content((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
    
    // Very basic JSON parsing logic - this is a simplification
    // In a real project, use a proper JSON library like nlohmann/json
    DataSet::DataMatrix data;
    std::vector<std::string> featureNames;
    
    // Find features array
    size_t featuresStart = content.find("\"features\":");
    if (featuresStart != std::string::npos) {
        size_t arrayStart = content.find('[', featuresStart);
        size_t arrayEnd = content.find(']', arrayStart);
        if (arrayStart != std::string::npos && arrayEnd != std::string::npos) {
            std::string featuresStr = content.substr(arrayStart + 1, arrayEnd - arrayStart - 1);
            std::vector<std::string> features = Utils::splitString(featuresStr, ',');
            for (auto& f : features) {
                f = Utils::trim(f);
                // Remove quotes
                if (f.size() >= 2 && f.front() == '"' && f.back() == '"') {
                    f = f.substr(1, f.size() - 2);
                }
                featureNames.push_back(f);
            }
        }
    }
    
    // Find data array
    size_t dataStart = content.find("\"data\":");
    if (dataStart != std::string::npos) {
        size_t arrayStart = content.find('[', dataStart);
        size_t arrayEnd = content.find(']', arrayStart);
        if (arrayStart != std::string::npos && arrayEnd != std::string::npos) {
            std::string dataStr = content.substr(arrayStart + 1, arrayEnd - arrayStart - 1);
            
            // Parse individual rows
            size_t pos = 0;
            while (pos < dataStr.length()) {
                size_t rowStart = dataStr.find('[', pos);
                if (rowStart == std::string::npos) break;
                
                size_t rowEnd = dataStr.find(']', rowStart);
                if (rowEnd == std::string::npos) break;
                
                std::string rowStr = dataStr.substr(rowStart + 1, rowEnd - rowStart - 1);
                std::vector<std::string> valueStrs = Utils::splitString(rowStr, ',');
                std::vector<double> row;
                
                for (const auto& valStr : valueStrs) {
                    std::string trimmed = Utils::trim(valStr);
                    row.push_back(Utils::stringToNumber<double>(trimmed, 0.0));
                }
                
                data.push_back(row);
                pos = rowEnd + 1;
            }
        }
    }
    
    return DataSet(data, {}, featureNames);
}

DataSet DataManager::loadTXT(std::ifstream& file, char delimiter) {
    std::string line;
    DataSet::DataMatrix data;
    
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue; // Skip empty lines and comments
        
        std::vector<std::string> tokens = Utils::splitString(line, delimiter);
        std::vector<double> row;
        row.reserve(tokens.size());
        
        for (const auto& token : tokens) {
            row.push_back(Utils::stringToNumber<double>(token, 0.0));
        }
        
        if (!row.empty()) {
            data.push_back(row);
        }
    }
    
    return DataSet(data);
}

bool DataManager::saveCSV(const DataSet& dataset, std::ofstream& file, bool includeHeader, char delimiter) {
    // Write header if requested and available
    if (includeHeader && !dataset.getFeatureNames().empty()) {
        file << Utils::joinStrings(dataset.getFeatureNames(), std::string(1, delimiter)) << '\n';
    }
    
    // Write data rows
    const auto& data = dataset.getData();
    const auto& labels = dataset.getLabels();
    bool hasLabels = dataset.hasLabels();
    
    for (size_t i = 0; i < data.size(); ++i) {
        const auto& row = data[i];
        std::vector<std::string> rowStr;
        rowStr.reserve(row.size() + (hasLabels ? 1 : 0));
        
        for (const auto& val : row) {
            rowStr.push_back(Utils::numberToString(val));
        }
        
        // Add label if present
        if (hasLabels && i < labels.size()) {
            rowStr.push_back(Utils::numberToString(labels[i]));
        }
        
        file << Utils::joinStrings(rowStr, std::string(1, delimiter)) << '\n';
    }
    
    return file.good();
}

bool DataManager::saveJSON(const DataSet& dataset, std::ofstream& file) {
    const auto& data = dataset.getData();
    const auto& labels = dataset.getLabels();
    const auto& featureNames = dataset.getFeatureNames();
    bool hasLabels = dataset.hasLabels();
    
    file << "{\n";
    
    // Write feature names if available
    if (!featureNames.empty()) {
        file << "  \"features\": [";
        for (size_t i = 0; i < featureNames.size(); ++i) {
            if (i > 0) file << ", ";
            file << "\"" << featureNames[i] << "\"";
        }
        if (hasLabels) {
            file << ", \"label\"";
        }
        file << "],\n";
    }
    
    // Write data
    file << "  \"data\": [\n";
    for (size_t i = 0; i < data.size(); ++i) {
        const auto& row = data[i];
        file << "    [";
        for (size_t j = 0; j < row.size(); ++j) {
            if (j > 0) file << ", ";
            file << row[j];
        }
        
        // Add label if present
        if (hasLabels && i < labels.size()) {
            if (!row.empty()) file << ", ";
            file << labels[i];
        }
        
        file << "]";
        if (i < data.size() - 1) file << ",";
        file << "\n";
    }
    
    file << "  ]\n";
    file << "}\n";
    
    return file.good();
}

bool DataManager::saveTXT(const DataSet& dataset, std::ofstream& file, char delimiter) {
    const auto& data = dataset.getData();
    const auto& labels = dataset.getLabels();
    bool hasLabels = dataset.hasLabels();
    
    for (size_t i = 0; i < data.size(); ++i) {
        const auto& row = data[i];
        
        for (size_t j = 0; j < row.size(); ++j) {
            if (j > 0) file << delimiter;
            file << row[j];
        }
        
        // Add label if present
        if (hasLabels && i < labels.size()) {
            if (!row.empty()) file << delimiter;
            file << labels[i];
        }
        
        file << '\n';
    }
    
    return file.good();
}
