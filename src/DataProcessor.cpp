/**
 * @file DataProcessor.cpp
 * @brief Implementation of the DataProcessor class
 */

#include "DataProcessor.h"
#include "ErrorHandler.h"
#include <algorithm>
#include <cmath>
#include <numeric>
#include <unordered_set>
#include <unordered_map>
#include <sstream>
#include <iomanip>

DataSet& DataProcessor::normalize(DataSet& dataset, 
                                 NormalizationType type,
                                 const std::vector<size_t>& featureIndices) {
    switch (type) {
        case NormalizationType::MIN_MAX:
            minMaxNormalize(dataset, featureIndices);
            break;
        case NormalizationType::Z_SCORE:
            zScoreNormalize(dataset, featureIndices);
            break;
        case NormalizationType::ROBUST:
            robustNormalize(dataset, featureIndices);
            break;
    }
    return dataset;
}

DataSet& DataProcessor::filterOutliers(DataSet& dataset, 
                                      double threshold,
                                      const std::vector<size_t>& featureIndices) {
    const auto& data = dataset.getData();
    const auto& labels = dataset.getLabels();
    
    if (data.empty()) return dataset;
    
    // Determine which features to process
    std::vector<size_t> indices;
    if (featureIndices.empty()) {
        // Process all features
        indices.resize(data[0].size());
        std::iota(indices.begin(), indices.end(), 0);
    } else {
        indices = featureIndices;
    }
    
    // Calculate mean and standard deviation for each feature
    std::vector<double> means(indices.size(), 0.0);
    std::vector<double> stdDevs(indices.size(), 0.0);
    
    // Calculate means
    for (size_t i = 0; i < data.size(); ++i) {
        for (size_t j = 0; j < indices.size(); ++j) {
            size_t featureIdx = indices[j];
            if (featureIdx < data[i].size()) {
                means[j] += data[i][featureIdx];
            }
        }
    }
    
    for (auto& mean : means) {
        mean /= data.size();
    }
    
    // Calculate standard deviations
    for (size_t i = 0; i < data.size(); ++i) {
        for (size_t j = 0; j < indices.size(); ++j) {
            size_t featureIdx = indices[j];
            if (featureIdx < data[i].size()) {
                double diff = data[i][featureIdx] - means[j];
                stdDevs[j] += diff * diff;
            }
        }
    }
    
    for (auto& stdDev : stdDevs) {
        stdDev = std::sqrt(stdDev / data.size());
    }
    
    // Identify non-outlier rows
    std::vector<size_t> validRowIndices;
    validRowIndices.reserve(data.size());
    
    for (size_t i = 0; i < data.size(); ++i) {
        bool isOutlier = false;
        
        for (size_t j = 0; j < indices.size(); ++j) {
            size_t featureIdx = indices[j];
            if (featureIdx < data[i].size()) {
                double zScore = std::abs((data[i][featureIdx] - means[j]) / stdDevs[j]);
                if (zScore > threshold) {
                    isOutlier = true;
                    break;
                }
            }
        }
        
        if (!isOutlier) {
            validRowIndices.push_back(i);
        }
    }
    
    // Create new dataset without outliers
    DataSet::DataMatrix newData;
    DataSet::LabelVector newLabels;
    
    newData.reserve(validRowIndices.size());
    if (!labels.empty()) {
        newLabels.reserve(validRowIndices.size());
    }
    
    for (size_t idx : validRowIndices) {
        newData.push_back(data[idx]);
        if (!labels.empty() && idx < labels.size()) {
            newLabels.push_back(labels[idx]);
        }
    }
    
    // Replace dataset contents
    DataSet newDataset(newData, newLabels, dataset.getFeatureNames());
    dataset = newDataset;
    
    return dataset;
}

DataSet& DataProcessor::fillMissingValues(DataSet& dataset,
                                         const std::string& strategy,
                                         double constantValue,
                                         const std::vector<size_t>& featureIndices) {
    auto& data = const_cast<DataSet::DataMatrix&>(dataset.getData());
    
    if (data.empty()) return dataset;
    
    // Determine which features to process
    std::vector<size_t> indices;
    if (featureIndices.empty()) {
        // Process all features
        indices.resize(data[0].size());
        std::iota(indices.begin(), indices.end(), 0);
    } else {
        indices = featureIndices;
    }
    
    // For each feature, compute the fill value and replace NaN or Inf
    for (size_t featureIdx : indices) {
        double fillValue = constantValue;
        
        if (strategy == "mean") {
            double sum = 0.0;
            size_t count = 0;
            
            for (const auto& row : data) {
                if (featureIdx < row.size() && std::isfinite(row[featureIdx])) {
                    sum += row[featureIdx];
                    count++;
                }
            }
            
            fillValue = (count > 0) ? sum / count : 0.0;
        } else if (strategy == "median") {
            std::vector<double> values;
            values.reserve(data.size());
            
            for (const auto& row : data) {
                if (featureIdx < row.size() && std::isfinite(row[featureIdx])) {
                    values.push_back(row[featureIdx]);
                }
            }
            
            if (!values.empty()) {
                std::sort(values.begin(), values.end());
                if (values.size() % 2 == 0) {
                    fillValue = (values[values.size() / 2 - 1] + values[values.size() / 2]) / 2.0;
                } else {
                    fillValue = values[values.size() / 2];
                }
            }
        } else if (strategy == "zero") {
            fillValue = 0.0;
        }
        // For "constant" strategy, use the provided constantValue
        
        // Fill missing values
        for (auto& row : data) {
            if (featureIdx < row.size() && !std::isfinite(row[featureIdx])) {
                row[featureIdx] = fillValue;
            }
        }
    }
    
    return dataset;
}

DataSet& DataProcessor::transform(DataSet& dataset,
                                 std::function<double(double)> transformer,
                                 const std::vector<size_t>& featureIndices) {
    auto& data = const_cast<DataSet::DataMatrix&>(dataset.getData());
    
    if (data.empty()) return dataset;
    
    // Determine which features to process
    std::vector<size_t> indices;
    if (featureIndices.empty()) {
        // Process all features
        indices.resize(data[0].size());
        std::iota(indices.begin(), indices.end(), 0);
    } else {
        indices = featureIndices;
    }
    
    // Apply transformation
    for (auto& row : data) {
        for (size_t featureIdx : indices) {
            if (featureIdx < row.size()) {
                row[featureIdx] = transformer(row[featureIdx]);
            }
        }
    }
    
    return dataset;
}

DataSet DataProcessor::oneHotEncode(const DataSet& dataset,
                                   const std::vector<size_t>& categoricalFeatureIndices) {
    const auto& data = dataset.getData();
    const auto& labels = dataset.getLabels();
    const auto& featureNames = dataset.getFeatureNames();
    
    // Return original dataset if data is empty or no categorical features specified
    if (data.empty() || categoricalFeatureIndices.empty()) {
        return dataset;
    }
    
    // Verify data consistency - check if all rows have the same number of features
    size_t numFeatures = 0;
    for (const auto& row : data) {
        if (numFeatures == 0) {
            numFeatures = row.size();
        } else if (row.size() != numFeatures) {
            // Inconsistent data - rows have different feature counts
            // In this case, we'll use the maximum size found
            numFeatures = std::max(numFeatures, row.size());
        }
    }
    
    // Find unique values for each categorical feature
    std::vector<std::unordered_map<double, size_t>> valueToIndexMaps;
    std::vector<std::vector<double>> uniqueValues;
    valueToIndexMaps.resize(categoricalFeatureIndices.size());
    uniqueValues.resize(categoricalFeatureIndices.size());
    
    for (size_t i = 0; i < categoricalFeatureIndices.size(); ++i) {
        size_t featureIdx = categoricalFeatureIndices[i];
        std::unordered_set<double> uniqueSet;
        
        // Collect all unique values for this feature
        for (const auto& row : data) {
            if (featureIdx < row.size()) {
                uniqueSet.insert(row[featureIdx]);
            }
        }
        
        // Special case: If no values were found, add a default value of 0
        if (uniqueSet.empty()) {
            uniqueSet.insert(0.0);
        }
        
        // Convert set to vector and sort for consistent ordering
        uniqueValues[i].insert(uniqueValues[i].end(), uniqueSet.begin(), uniqueSet.end());
        std::sort(uniqueValues[i].begin(), uniqueValues[i].end());
        
        // Create mapping from value to index for one-hot encoding
        for (size_t j = 0; j < uniqueValues[i].size(); ++j) {
            valueToIndexMaps[i][uniqueValues[i][j]] = j;
        }
    }
    
    // Calculate the size of the new dataset
    size_t totalNewFeatures = 0;
    for (const auto& values : uniqueValues) {
        totalNewFeatures += values.size();
    }
    
    // Create new feature names
    std::vector<std::string> newFeatureNames;
    std::vector<size_t> nonCategoricalIndices;
    
    // Collect non-categorical feature indices
    for (size_t i = 0; i < numFeatures; ++i) {
        if (std::find(categoricalFeatureIndices.begin(), categoricalFeatureIndices.end(), i) 
            == categoricalFeatureIndices.end()) {
            nonCategoricalIndices.push_back(i);
        }
    }
    
    // Add non-categorical feature names
    for (size_t idx : nonCategoricalIndices) {
        if (idx < featureNames.size()) {
            newFeatureNames.push_back(featureNames[idx]);
        } else {
            newFeatureNames.push_back("Feature_" + std::to_string(idx));
        }
    }
    
    // Add categorical feature names
    for (size_t i = 0; i < categoricalFeatureIndices.size(); ++i) {
        size_t featureIdx = categoricalFeatureIndices[i];
        std::string baseName = (featureIdx < featureNames.size()) ? 
                              featureNames[featureIdx] : "Feature_" + std::to_string(featureIdx);
        
        for (double value : uniqueValues[i]) {
            // Use integer conversion for the value if it represents a whole number
            if (std::fabs(value - std::round(value)) < 1e-10) {
                newFeatureNames.push_back(baseName + "_" + std::to_string(static_cast<int>(value)));
            } else {
                // Use stringstream to control precision for floating point values
                std::stringstream ss;
                ss << std::fixed << std::setprecision(2) << value;
                newFeatureNames.push_back(baseName + "_" + ss.str());
            }
        }
    }
    
    // Create new data matrix
    DataSet::DataMatrix newData;
    newData.reserve(data.size());
    
    for (const auto& row : data) {
        std::vector<double> newRow;
        newRow.reserve(nonCategoricalIndices.size() + totalNewFeatures);
        
        // Add non-categorical features
        for (size_t idx : nonCategoricalIndices) {
            if (idx < row.size()) {
                newRow.push_back(row[idx]);
            } else {
                // Feature is missing for this row, use default value
                newRow.push_back(0.0);
            }
        }
        
        // Add one-hot encoded features
        for (size_t i = 0; i < categoricalFeatureIndices.size(); ++i) {
            size_t featureIdx = categoricalFeatureIndices[i];
            double value = (featureIdx < row.size()) ? row[featureIdx] : 0.0;
            
            std::vector<double> oneHotValues(uniqueValues[i].size(), 0.0);
            if (valueToIndexMaps[i].count(value) > 0) {
                oneHotValues[valueToIndexMaps[i][value]] = 1.0;
            } else {
                // If value not found in map (shouldn't happen normally), use first category
                if (!oneHotValues.empty()) {
                    oneHotValues[0] = 1.0;
                }
            }
            
            newRow.insert(newRow.end(), oneHotValues.begin(), oneHotValues.end());
        }
        
        newData.push_back(newRow);
    }
    
    return DataSet(newData, labels, newFeatureNames);
}

std::pair<DataSet, DataSet> DataProcessor::splitFeaturesTarget(const DataSet& dataset, 
                                                             size_t targetColumnIndex) {
    const auto& data = dataset.getData();
    const auto& featureNames = dataset.getFeatureNames();
    
    if (data.empty()) {
        return {dataset, DataSet()};
    }
    
    // Create feature dataset
    DataSet::DataMatrix featureData;
    featureData.reserve(data.size());
    
    std::vector<std::string> newFeatureNames;
    if (!featureNames.empty()) {
        for (size_t i = 0; i < featureNames.size(); ++i) {
            if (i != targetColumnIndex) {
                newFeatureNames.push_back(featureNames[i]);
            }
        }
    }
    
    // Create target dataset
    DataSet::DataMatrix targetData;
    targetData.reserve(data.size());
    
    std::vector<std::string> targetFeatureName;
    if (targetColumnIndex < featureNames.size()) {
        targetFeatureName.push_back(featureNames[targetColumnIndex]);
    }
    
    for (const auto& row : data) {
        std::vector<double> featureRow;
        std::vector<double> targetRow(1);
        
        for (size_t i = 0; i < row.size(); ++i) {
            if (i == targetColumnIndex) {
                targetRow[0] = row[i];
            } else {
                featureRow.push_back(row[i]);
            }
        }
        
        featureData.push_back(featureRow);
        targetData.push_back(targetRow);
    }
    
    return {
        DataSet(featureData, {}, newFeatureNames),
        DataSet(targetData, {}, targetFeatureName)
    };
}

std::unordered_map<std::string, double> DataProcessor::calculateStatistics(
    const DataSet& dataset, int featureIndex) {
    
    const auto& data = dataset.getData();
    std::unordered_map<std::string, double> stats;
    
    if (data.empty()) {
        return stats;
    }
    
    // If featureIndex is -1, calculate stats for all features
    std::vector<int> featureIndices;
    if (featureIndex == -1) {
        for (size_t i = 0; i < data[0].size(); ++i) {
            featureIndices.push_back(static_cast<int>(i));
        }
    } else {
        featureIndices.push_back(featureIndex);
    }
    
    for (int idx : featureIndices) {
        std::string prefix = "feature_" + std::to_string(idx) + "_";
        
        // Extract values for this feature
        std::vector<double> values;
        values.reserve(data.size());
        
        for (const auto& row : data) {
            if (static_cast<size_t>(idx) < row.size()) {
                values.push_back(row[idx]);
            }
        }
        
        if (values.empty()) {
            continue;
        }
        
        // Calculate mean
        double mean = calculateMean(values);
        stats[prefix + "mean"] = mean;
        
        // Calculate median
        double median = calculateMedian(values);
        stats[prefix + "median"] = median;
        
        // Calculate min and max
        auto minmax = std::minmax_element(values.begin(), values.end());
        stats[prefix + "min"] = *minmax.first;
        stats[prefix + "max"] = *minmax.second;
        
        // Calculate standard deviation
        double stdDev = calculateStdDev(values, mean);
        stats[prefix + "std_dev"] = stdDev;
        
        // Calculate quartiles
        auto q1q3 = calculateQuantiles(values, 0.25, 0.75);
        stats[prefix + "q1"] = q1q3.first;
        stats[prefix + "q3"] = q1q3.second;
        stats[prefix + "iqr"] = q1q3.second - q1q3.first;
    }
    
    return stats;
}

// Private helper methods

void DataProcessor::minMaxNormalize(DataSet& dataset, const std::vector<size_t>& featureIndices) {
    // We need direct access to modify the data
    auto& data = const_cast<DataSet::DataMatrix&>(dataset.getData());
    
    if (data.empty()) return;
    
    // Determine which features to process
    std::vector<size_t> indices;
    if (featureIndices.empty()) {
        // Process all features
        indices.resize(data[0].size());
        std::iota(indices.begin(), indices.end(), 0);
    } else {
        indices = featureIndices;
    }
    
    // Find min and max for each feature
    std::vector<double> minValues(indices.size(), std::numeric_limits<double>::max());
    std::vector<double> maxValues(indices.size(), std::numeric_limits<double>::lowest());
    
    // First pass: find min and max values
    for (const auto& row : data) {
        for (size_t i = 0; i < indices.size(); ++i) {
            size_t featureIdx = indices[i];
            if (featureIdx < row.size()) {
                minValues[i] = std::min(minValues[i], row[featureIdx]);
                maxValues[i] = std::max(maxValues[i], row[featureIdx]);
            }
        }
    }
    
    // Second pass: normalize each feature
    for (auto& row : data) {
        for (size_t i = 0; i < indices.size(); ++i) {
            size_t featureIdx = indices[i];
            if (featureIdx < row.size()) {
                double range = maxValues[i] - minValues[i];
                if (std::abs(range) > 1e-10) { // Check for near-zero range to avoid division by zero
                    row[featureIdx] = (row[featureIdx] - minValues[i]) / range;
                } else {
                    // All values are the same - set to 0.5 as a middle value
                    row[featureIdx] = 0.5;
                }
            }
        }
    }
}

void DataProcessor::zScoreNormalize(DataSet& dataset, const std::vector<size_t>& featureIndices) {
    auto& data = const_cast<DataSet::DataMatrix&>(dataset.getData());
    
    if (data.empty()) return;
    
    // Determine which features to process
    std::vector<size_t> indices;
    if (featureIndices.empty()) {
        // Process all features
        indices.resize(data[0].size());
        std::iota(indices.begin(), indices.end(), 0);
    } else {
        indices = featureIndices;
    }
    
    // Calculate mean and standard deviation for each feature
    std::vector<double> means(indices.size(), 0.0);
    std::vector<double> stdDevs(indices.size(), 0.0);
    std::vector<int> counts(indices.size(), 0); // Track valid values for each feature
    
    // Calculate means
    for (const auto& row : data) {
        for (size_t i = 0; i < indices.size(); ++i) {
            size_t featureIdx = indices[i];
            if (featureIdx < row.size()) {
                means[i] += row[featureIdx];
                counts[i]++;
            }
        }
    }
    
    // Divide by actual number of elements for each feature
    for (size_t i = 0; i < means.size(); ++i) {
        if (counts[i] > 0) {
            means[i] /= counts[i];
        }
    }
    
    // Reset counts for standard deviation calculation
    std::fill(counts.begin(), counts.end(), 0);
    
    // Calculate standard deviations
    for (const auto& row : data) {
        for (size_t i = 0; i < indices.size(); ++i) {
            size_t featureIdx = indices[i];
            if (featureIdx < row.size()) {
                double diff = row[featureIdx] - means[i];
                stdDevs[i] += diff * diff;
                counts[i]++;
            }
        }
    }
    
    // Divide by actual number of elements for each feature
    for (size_t i = 0; i < stdDevs.size(); ++i) {
        if (counts[i] > 0) {
            stdDevs[i] = std::sqrt(stdDevs[i] / counts[i]);
        }
    }
    
    // Normalize each feature
    for (auto& row : data) {
        for (size_t i = 0; i < indices.size(); ++i) {
            size_t featureIdx = indices[i];
            if (featureIdx < row.size()) {
                // Use a small epsilon to avoid division by zero
                if (std::abs(stdDevs[i]) > 1e-10) {
                    row[featureIdx] = (row[featureIdx] - means[i]) / stdDevs[i];
                } else {
                    // All values are the same or very close
                    row[featureIdx] = 0.0;
                }
            }
        }
    }
}

void DataProcessor::robustNormalize(DataSet& dataset, const std::vector<size_t>& featureIndices) {
    auto& data = const_cast<DataSet::DataMatrix&>(dataset.getData());
    
    if (data.empty()) return;
    
    // Determine which features to process
    std::vector<size_t> indices;
    if (featureIndices.empty()) {
        // Process all features
        indices.resize(data[0].size());
        std::iota(indices.begin(), indices.end(), 0);
    } else {
        indices = featureIndices;
    }
    
    // Calculate median and IQR for each feature
    std::vector<double> medians(indices.size(), 0.0);
    std::vector<double> iqrs(indices.size(), 0.0);
    
    for (size_t i = 0; i < indices.size(); ++i) {
        size_t featureIdx = indices[i];
        std::vector<double> values;
        values.reserve(data.size());
        
        // Collect all valid values for this feature
        for (const auto& row : data) {
            if (featureIdx < row.size()) {
                values.push_back(row[featureIdx]);
            }
        }
        
        if (values.empty()) {
            continue; // Skip this feature if no valid values
        }
        
        // Sort values to calculate median and quartiles
        std::sort(values.begin(), values.end());
        
        // Calculate median
        if (values.size() % 2 == 0) {
            medians[i] = (values[values.size() / 2 - 1] + values[values.size() / 2]) / 2.0;
        } else {
            medians[i] = values[values.size() / 2];
        }
        
        // Calculate IQR (Q3 - Q1) with proper index calculation
        // Make sure we have enough values to calculate quartiles
        if (values.size() >= 4) {
            // Calculate correct quartile indices based on actual data size
            size_t q1Idx = std::max(0UL, (values.size() - 1) / 4);
            size_t q3Idx = std::min(values.size() - 1, (values.size() - 1) * 3 / 4);
            
            double q1 = values[q1Idx];
            double q3 = values[q3Idx];
            iqrs[i] = q3 - q1;
        } else {
            // Not enough data for quartiles, use range as a fallback
            iqrs[i] = values.back() - values.front();
        }
    }
    
    // Normalize each feature
    for (auto& row : data) {
        for (size_t i = 0; i < indices.size(); ++i) {
            size_t featureIdx = indices[i];
            if (featureIdx < row.size()) {
                // Use small epsilon to avoid division by near-zero IQR
                if (std::abs(iqrs[i]) > 1e-10) {
                    row[featureIdx] = (row[featureIdx] - medians[i]) / iqrs[i];
                } else {
                    // All values are the same or insufficient variation
                    row[featureIdx] = 0.0;
                }
            }
        }
    }
}

double DataProcessor::calculateMean(const std::vector<double>& values) {
    if (values.empty()) return 0.0;
    double sum = std::accumulate(values.begin(), values.end(), 0.0);
    return sum / values.size();
}

double DataProcessor::calculateMedian(std::vector<double> values) {
    if (values.empty()) return 0.0;
    
    std::sort(values.begin(), values.end());
    if (values.size() % 2 == 0) {
        return (values[values.size() / 2 - 1] + values[values.size() / 2]) / 2.0;
    } else {
        return values[values.size() / 2];
    }
}

double DataProcessor::calculateStdDev(const std::vector<double>& values, double mean) {
    if (values.empty()) return 0.0;
    
    double variance = 0.0;
    for (double val : values) {
        double diff = val - mean;
        variance += diff * diff;
    }
    
    return std::sqrt(variance / values.size());
}

std::pair<double, double> DataProcessor::calculateQuantiles(std::vector<double> values, double q1, double q2) {
    if (values.empty()) return {0.0, 0.0};
    
    std::sort(values.begin(), values.end());
    
    auto getQuantile = [&values](double q) -> double {
        double pos = q * (values.size() - 1);
        size_t idx = static_cast<size_t>(pos);
        double frac = pos - idx;
        
        if (idx + 1 < values.size()) {
            return values[idx] + frac * (values[idx + 1] - values[idx]);
        } else {
            return values[idx];
        }
    };
    
    return {getQuantile(q1), getQuantile(q2)};
}
