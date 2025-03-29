/**
 * @file TestDataProcessor.cpp
 * @brief Unit tests for the DataProcessor class
 */

#include "DataProcessor.h"
#include "gtest/gtest.h"
#include <cmath>
#include <algorithm>
#include <random>

class DataProcessorTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create a simple dataset for testing
        data = {
            {1.0, 2.0, 3.0},
            {4.0, 5.0, 6.0},
            {7.0, 8.0, 9.0},
            {10.0, 11.0, 12.0},
            {13.0, 14.0, 15.0}
        };
        
        labels = {1.0, 2.0, 3.0, 4.0, 5.0};
        
        featureNames = {"feature1", "feature2", "feature3"};
        
        dataset = DataSet(data, labels, featureNames);
    }

    DataProcessor processor;
    DataSet::DataMatrix data;
    DataSet::LabelVector labels;
    DataSet::FeatureNames featureNames;
    DataSet dataset;
};

// Test min-max normalization
TEST_F(DataProcessorTest, MinMaxNormalization) {
    DataSet normalizedData = dataset;
    processor.normalize(normalizedData, NormalizationType::MIN_MAX);
    
    // Check that values are in range [0,1]
    for (const auto& row : normalizedData.getData()) {
        for (double val : row) {
            EXPECT_GE(val, 0.0);
            EXPECT_LE(val, 1.0);
        }
    }
    
    // Check specific values
    // First feature should be (1-1)/(13-1) = 0 for first row, (13-1)/(13-1) = 1 for last row
    EXPECT_NEAR(normalizedData.getData()[0][0], 0.0, 1e-6);
    EXPECT_NEAR(normalizedData.getData()[4][0], 1.0, 1e-6);
    
    // Middle value for second feature should be (8-2)/(14-2) = 0.5
    EXPECT_NEAR(normalizedData.getData()[2][1], 0.5, 1e-6);
}

// Test Z-score normalization
TEST_F(DataProcessorTest, ZScoreNormalization) {
    DataSet normalizedData = dataset;
    processor.normalize(normalizedData, NormalizationType::Z_SCORE);
    
    // Calculate expected values
    // For feature1:
    // mean = (1+4+7+10+13)/5 = 7
    // std = sqrt(((1-7)²+(4-7)²+(7-7)²+(10-7)²+(13-7)²)/5) = sqrt(22) = 4.69
    // Normalized value for row[0][0] = (1-7)/4.69 = -1.28
    
    EXPECT_NEAR(normalizedData.getData()[0][0], -1.28, 0.1);
    
    // Mean of each feature should be approximately 0
    double sum1 = 0.0, sum2 = 0.0, sum3 = 0.0;
    for (const auto& row : normalizedData.getData()) {
        sum1 += row[0];
        sum2 += row[1];
        sum3 += row[2];
    }
    
    EXPECT_NEAR(sum1 / normalizedData.getData().size(), 0.0, 1e-6);
    EXPECT_NEAR(sum2 / normalizedData.getData().size(), 0.0, 1e-6);
    EXPECT_NEAR(sum3 / normalizedData.getData().size(), 0.0, 1e-6);
}

// Test outlier filtering
TEST_F(DataProcessorTest, FilterOutliers) {
    // Create a dataset with outliers
    DataSet::DataMatrix dataWithOutliers = {
        {1.0, 2.0, 3.0},
        {2.0, 3.0, 4.0},
        {2.0, 3.0, 4.0},
        {2.0, 3.0, 4.0},
        {100.0, 200.0, 300.0} // Outlier
    };
    
    DataSet datasetWithOutliers(dataWithOutliers);
    
    processor.filterOutliers(datasetWithOutliers, 2.0); // Z-score threshold of 2
    
    // Expect the outlier to be removed
    EXPECT_EQ(datasetWithOutliers.getData().size(), 4);
    
    // Check that the maximum values are reasonable
    double maxVal1 = 0.0, maxVal2 = 0.0, maxVal3 = 0.0;
    for (const auto& row : datasetWithOutliers.getData()) {
        maxVal1 = std::max(maxVal1, row[0]);
        maxVal2 = std::max(maxVal2, row[1]);
        maxVal3 = std::max(maxVal3, row[2]);
    }
    
    // Max should be much smaller than the outlier values
    EXPECT_LT(maxVal1, 50.0);
    EXPECT_LT(maxVal2, 100.0);
    EXPECT_LT(maxVal3, 150.0);
}

// Test fill missing values
TEST_F(DataProcessorTest, FillMissingValues) {
    // Create a dataset with NaN values
    DataSet::DataMatrix dataWithNaN = {
        {1.0, 2.0, 3.0},
        {4.0, std::nan(""), 6.0},
        {7.0, 8.0, std::nan("")}
    };
    
    DataSet datasetWithNaN(dataWithNaN);
    
    // Fill with mean
    processor.fillMissingValues(datasetWithNaN, "mean");
    
    // Check that NaN values are replaced
    for (const auto& row : datasetWithNaN.getData()) {
        for (double val : row) {
            EXPECT_FALSE(std::isnan(val));
        }
    }
    
    // Expected mean for second column: (2+8)/2 = 5
    EXPECT_NEAR(datasetWithNaN.getData()[1][1], 5.0, 1e-6);
    
    // Expected mean for third column: (3+6)/2 = 4.5
    EXPECT_NEAR(datasetWithNaN.getData()[2][2], 4.5, 1e-6);
}

// Test transform
TEST_F(DataProcessorTest, Transform) {
    DataSet transformedData = dataset;
    
    // Square all values
    processor.transform(transformedData, [](double x) { return x * x; });
    
    // Check specific values
    EXPECT_DOUBLE_EQ(transformedData.getData()[0][0], 1.0);
    EXPECT_DOUBLE_EQ(transformedData.getData()[1][1], 25.0);
    EXPECT_DOUBLE_EQ(transformedData.getData()[2][2], 81.0);
}

// Test one-hot encoding
TEST_F(DataProcessorTest, OneHotEncode) {
    // Create a dataset with categorical features
    DataSet::DataMatrix dataWithCategories = {
        {1.0, 1.0, 3.0},
        {2.0, 1.0, 4.0},
        {3.0, 2.0, 5.0},
        {1.0, 3.0, 6.0}
    };
    
    DataSet datasetWithCategories(dataWithCategories);
    
    // One-hot encode the first column
    DataSet encoded = processor.oneHotEncode(datasetWithCategories, {0});
    
    // Check dimensions
    // Should have columns: cat1_1.0, cat1_2.0, cat1_3.0, original_col2, original_col3
    EXPECT_EQ(encoded.getData()[0].size(), 5);
    
    // Check encoding
    // First row should have [1,0,0] for the encoded part (original value was 1.0)
    EXPECT_DOUBLE_EQ(encoded.getData()[0][0], 1.0);
    EXPECT_DOUBLE_EQ(encoded.getData()[0][1], 0.0);
    EXPECT_DOUBLE_EQ(encoded.getData()[0][2], 0.0);
    
    // Third row should have [0,0,1] for the encoded part (original value was 3.0)
    EXPECT_DOUBLE_EQ(encoded.getData()[2][0], 0.0);
    EXPECT_DOUBLE_EQ(encoded.getData()[2][1], 0.0);
    EXPECT_DOUBLE_EQ(encoded.getData()[2][2], 1.0);
}

// Test features-target split
TEST_F(DataProcessorTest, SplitFeaturesTarget) {
    auto [features, target] = processor.splitFeaturesTarget(dataset, 2);
    
    // Check dimensions
    EXPECT_EQ(features.getData()[0].size(), 2); // Two features left
    EXPECT_EQ(target.getData()[0].size(), 1);   // One target column
    
    // Check values
    EXPECT_DOUBLE_EQ(features.getData()[0][0], 1.0);
    EXPECT_DOUBLE_EQ(features.getData()[0][1], 2.0);
    
    EXPECT_DOUBLE_EQ(target.getData()[0][0], 3.0);
    EXPECT_DOUBLE_EQ(target.getData()[1][0], 6.0);
}

// Test statistics calculation
TEST_F(DataProcessorTest, CalculateStatistics) {
    auto stats = processor.calculateStatistics(dataset, 0);
    
    // Expected values for first feature
    // mean = (1+4+7+10+13)/5 = 7
    // median = 7
    // min = 1, max = 13
    
    EXPECT_NEAR(stats["feature_0_mean"], 7.0, 1e-6);
    EXPECT_NEAR(stats["feature_0_median"], 7.0, 1e-6);
    EXPECT_NEAR(stats["feature_0_min"], 1.0, 1e-6);
    EXPECT_NEAR(stats["feature_0_max"], 13.0, 1e-6);
}
