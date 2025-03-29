/**
 * @file DataManager.h
 * @brief Header file for the DataManager class which handles data I/O operations
 */

#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <fstream>
#include <tuple>
#include <functional>

/**
 * @enum DataFormat
 * @brief Supported file formats for data I/O
 */
enum class DataFormat {
    CSV,
    JSON,
    TXT
};

/**
 * @class DataSet
 * @brief Class for storing and manipulating datasets
 * 
 * This class provides a general-purpose container for both labeled and unlabeled data
 * that can be used for various data processing and ML tasks.
 */
class DataSet {
public:
    using DataMatrix = std::vector<std::vector<double>>;
    using LabelVector = std::vector<double>;
    using FeatureNames = std::vector<std::string>;
    
    /**
     * @brief Default constructor
     */
    DataSet() = default;
    
    /**
     * @brief Constructor with data and optional labels
     * @param data Matrix containing features/values
     * @param labels Optional vector of labels for supervised learning
     * @param featureNames Optional vector of feature names
     */
    DataSet(DataMatrix data, 
            LabelVector labels = {}, 
            FeatureNames featureNames = {});
    
    /**
     * @brief Add a data row to the dataset
     * @param dataRow Row of features to add
     * @param label Optional label for this data point
     */
    void addDataRow(const std::vector<double>& dataRow, double label = 0.0);
    
    /**
     * @brief Get the data matrix
     * @return Const reference to the data matrix
     */
    const DataMatrix& getData() const { return m_data; }
    
    /**
     * @brief Get the labels vector
     * @return Const reference to the labels vector
     */
    const LabelVector& getLabels() const { return m_labels; }
    
    /**
     * @brief Check if the dataset has labels
     * @return True if the dataset has labels
     */
    bool hasLabels() const { return !m_labels.empty(); }
    
    /**
     * @brief Get feature names
     * @return Const reference to feature names
     */
    const FeatureNames& getFeatureNames() const { return m_featureNames; }
    
    /**
     * @brief Set feature names
     * @param names Vector of feature names
     */
    void setFeatureNames(const FeatureNames& names) { m_featureNames = names; }
    
    /**
     * @brief Get the dimensions of the dataset
     * @return Pair containing rows and columns
     */
    std::pair<size_t, size_t> dimensions() const;
    
    /**
     * @brief Split dataset into training and testing sets
     * @param testRatio Ratio of data to use for testing (0.0-1.0)
     * @return Pair of DataSets (training, testing)
     */
    std::pair<DataSet, DataSet> split(double testRatio = 0.2) const;
    
private:
    DataMatrix m_data;
    LabelVector m_labels;
    FeatureNames m_featureNames;
};

/**
 * @class DataManager
 * @brief Class for loading and saving datasets from/to files
 */
class DataManager {
public:
    /**
     * @brief Default constructor
     */
    DataManager() = default;
    
    /**
     * @brief Load data from a file
     * @param filename File path to load
     * @param format Format of the file
     * @param hasHeader Whether the file has a header row
     * @param delimiter Delimiter for CSV files
     * @return Loaded DataSet
     * @throws std::runtime_error if file cannot be loaded
     */
    DataSet loadFromFile(const std::string& filename, 
                         DataFormat format = DataFormat::CSV,
                         bool hasHeader = true,
                         char delimiter = ',');
    
    /**
     * @brief Save dataset to a file
     * @param dataset DataSet to save
     * @param filename File path to save to
     * @param format Format of the output file
     * @param includeHeader Whether to write a header row
     * @param delimiter Delimiter for CSV files
     * @return True if save was successful
     * @throws std::runtime_error if file cannot be saved
     */
    bool saveToFile(const DataSet& dataset,
                    const std::string& filename,
                    DataFormat format = DataFormat::CSV,
                    bool includeHeader = true,
                    char delimiter = ',');
    
private:
    // Helper functions for different file formats
    DataSet loadCSV(std::ifstream& file, bool hasHeader, char delimiter);
    DataSet loadJSON(std::ifstream& file);
    DataSet loadTXT(std::ifstream& file, char delimiter);
    
    bool saveCSV(const DataSet& dataset, std::ofstream& file, bool includeHeader, char delimiter);
    bool saveJSON(const DataSet& dataset, std::ofstream& file);
    bool saveTXT(const DataSet& dataset, std::ofstream& file, char delimiter);
    
    // JSON parsing helper function
    size_t findMatchingBracket(const std::string& str, size_t openPos);
};
