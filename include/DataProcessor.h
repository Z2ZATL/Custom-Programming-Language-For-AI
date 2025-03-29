/**
 * @file DataProcessor.h
 * @brief Header file for the DataProcessor class which handles data preprocessing and transformation
 */

#pragma once

#include "DataManager.h"
#include <functional>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <unordered_map>

/**
 * @enum NormalizationType
 * @brief Different normalization strategies for data preprocessing
 */
enum class NormalizationType {
    MIN_MAX,     // Scale data to range [0,1]
    Z_SCORE,     // Standardize to mean=0, std=1
    ROBUST       // Scale using median and interquartile range
};

/**
 * @class DataProcessor
 * @brief Class for preprocessing and transforming datasets
 */
class DataProcessor {
public:
    /**
     * @brief Default constructor
     */
    DataProcessor() = default;
    
    /**
     * @brief Normalize data according to specified strategy
     * @param dataset Dataset to normalize (modified in place)
     * @param type Normalization strategy to use
     * @param featureIndices Optional indices of features to normalize (all if empty)
     * @return Reference to modified dataset
     */
    DataSet& normalize(DataSet& dataset, 
                       NormalizationType type = NormalizationType::Z_SCORE,
                       const std::vector<size_t>& featureIndices = {});
    
    /**
     * @brief Filter out outliers from the dataset
     * @param dataset Dataset to process (modified in place)
     * @param threshold Threshold for outlier detection (e.g., Z-score threshold)
     * @param featureIndices Optional indices of features to check for outliers
     * @return Reference to modified dataset
     */
    DataSet& filterOutliers(DataSet& dataset, 
                            double threshold = 3.0,
                            const std::vector<size_t>& featureIndices = {});
    
    /**
     * @brief Fill missing values in the dataset
     * @param dataset Dataset to process (modified in place)
     * @param strategy Strategy for filling ("mean", "median", "zero", "constant")
     * @param constantValue Value to use when strategy is "constant"
     * @param featureIndices Optional indices of features to process
     * @return Reference to modified dataset
     */
    DataSet& fillMissingValues(DataSet& dataset,
                               const std::string& strategy = "mean",
                               double constantValue = 0.0,
                               const std::vector<size_t>& featureIndices = {});
    
    /**
     * @brief Apply a custom transformation to each data point
     * @param dataset Dataset to transform (modified in place)
     * @param transformer Function to apply to each value
     * @param featureIndices Optional indices of features to transform
     * @return Reference to modified dataset
     */
    DataSet& transform(DataSet& dataset,
                       std::function<double(double)> transformer,
                       const std::vector<size_t>& featureIndices = {});
    
    /**
     * @brief Apply one-hot encoding to categorical features
     * @param dataset Dataset to process (modified in place)
     * @param categoricalFeatureIndices Indices of categorical features to encode
     * @return New DataSet with encoded features
     */
    DataSet oneHotEncode(const DataSet& dataset,
                         const std::vector<size_t>& categoricalFeatureIndices);
    
    /**
     * @brief Split dataset into features and target
     * @param dataset Input dataset
     * @param targetColumnIndex Index of the target column
     * @return Pair of DataSets (features, target)
     */
    std::pair<DataSet, DataSet> splitFeaturesTarget(const DataSet& dataset, 
                                                   size_t targetColumnIndex);
    
    /**
     * @brief Calculate basic statistics for a dataset
     * @param dataset Dataset to analyze
     * @param featureIndex Feature index to analyze (or -1 for all)
     * @return Map of statistics (mean, median, min, max, std)
     */
    std::unordered_map<std::string, double> calculateStatistics(
        const DataSet& dataset, int featureIndex = -1);
    
private:
    // Helper methods for different normalization strategies
    void minMaxNormalize(DataSet& dataset, const std::vector<size_t>& featureIndices);
    void zScoreNormalize(DataSet& dataset, const std::vector<size_t>& featureIndices);
    void robustNormalize(DataSet& dataset, const std::vector<size_t>& featureIndices);
    
    // Helper methods for statistics calculations
    double calculateMean(const std::vector<double>& values);
    double calculateMedian(std::vector<double> values);
    double calculateStdDev(const std::vector<double>& values, double mean);
    std::pair<double, double> calculateQuantiles(std::vector<double> values, double q1, double q2);
};
