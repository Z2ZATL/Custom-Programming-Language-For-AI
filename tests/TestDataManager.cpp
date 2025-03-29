/**
 * @file TestDataManager.cpp
 * @brief Unit tests for the DataManager class
 */

#include "DataManager.h"
#include "gtest/gtest.h"
#include <fstream>
#include <sstream>
#include <cstdio>

class DataManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create test files
        createCsvTestFile();
        createJsonTestFile();
        createTxtTestFile();
    }

    void TearDown() override {
        // Clean up test files
        std::remove("test_data.csv");
        std::remove("test_data.json");
        std::remove("test_data.txt");
        std::remove("test_output.csv");
        std::remove("test_output.json");
        std::remove("test_output.txt");
    }

    void createCsvTestFile() {
        std::ofstream file("test_data.csv");
        file << "feature1,feature2,feature3\n";
        file << "1.0,2.0,3.0\n";
        file << "4.0,5.0,6.0\n";
        file << "7.0,8.0,9.0\n";
        file.close();
    }

    void createJsonTestFile() {
        std::ofstream file("test_data.json");
        file << "{\n";
        file << "  \"features\": [\"feature1\", \"feature2\", \"feature3\"],\n";
        file << "  \"data\": [\n";
        file << "    [1.0, 2.0, 3.0],\n";
        file << "    [4.0, 5.0, 6.0],\n";
        file << "    [7.0, 8.0, 9.0]\n";
        file << "  ]\n";
        file << "}\n";
        file.close();
    }

    void createTxtTestFile() {
        std::ofstream file("test_data.txt");
        file << "1.0 2.0 3.0\n";
        file << "4.0 5.0 6.0\n";
        file << "7.0 8.0 9.0\n";
        file.close();
    }

    DataManager dataManager;
};

// Test loading from CSV
TEST_F(DataManagerTest, LoadFromCSV) {
    DataSet dataset = dataManager.loadFromFile("test_data.csv", DataFormat::CSV, true);
    
    ASSERT_EQ(dataset.getData().size(), 3);
    ASSERT_EQ(dataset.getData()[0].size(), 3);
    ASSERT_EQ(dataset.getFeatureNames().size(), 3);
    
    EXPECT_EQ(dataset.getFeatureNames()[0], "feature1");
    EXPECT_EQ(dataset.getFeatureNames()[1], "feature2");
    EXPECT_EQ(dataset.getFeatureNames()[2], "feature3");
    
    EXPECT_DOUBLE_EQ(dataset.getData()[0][0], 1.0);
    EXPECT_DOUBLE_EQ(dataset.getData()[1][1], 5.0);
    EXPECT_DOUBLE_EQ(dataset.getData()[2][2], 9.0);
}

// Test loading from JSON
TEST_F(DataManagerTest, LoadFromJSON) {
    DataSet dataset = dataManager.loadFromFile("test_data.json", DataFormat::JSON);
    
    ASSERT_EQ(dataset.getData().size(), 3);
    ASSERT_EQ(dataset.getData()[0].size(), 3);
    ASSERT_EQ(dataset.getFeatureNames().size(), 3);
    
    EXPECT_EQ(dataset.getFeatureNames()[0], "feature1");
    EXPECT_EQ(dataset.getFeatureNames()[1], "feature2");
    EXPECT_EQ(dataset.getFeatureNames()[2], "feature3");
    
    EXPECT_DOUBLE_EQ(dataset.getData()[0][0], 1.0);
    EXPECT_DOUBLE_EQ(dataset.getData()[1][1], 5.0);
    EXPECT_DOUBLE_EQ(dataset.getData()[2][2], 9.0);
}

// Test loading from TXT
TEST_F(DataManagerTest, LoadFromTXT) {
    DataSet dataset = dataManager.loadFromFile("test_data.txt", DataFormat::TXT, false, ' ');
    
    ASSERT_EQ(dataset.getData().size(), 3);
    ASSERT_EQ(dataset.getData()[0].size(), 3);
    
    EXPECT_DOUBLE_EQ(dataset.getData()[0][0], 1.0);
    EXPECT_DOUBLE_EQ(dataset.getData()[1][1], 5.0);
    EXPECT_DOUBLE_EQ(dataset.getData()[2][2], 9.0);
}

// Test saving to CSV
TEST_F(DataManagerTest, SaveToCSV) {
    // Create a dataset
    DataSet::DataMatrix data = {
        {1.0, 2.0, 3.0},
        {4.0, 5.0, 6.0},
        {7.0, 8.0, 9.0}
    };
    std::vector<std::string> featureNames = {"col1", "col2", "col3"};
    DataSet dataset(data, {}, featureNames);
    
    // Save to CSV
    EXPECT_TRUE(dataManager.saveToFile(dataset, "test_output.csv", DataFormat::CSV, true));
    
    // Reload and verify
    DataSet reloaded = dataManager.loadFromFile("test_output.csv", DataFormat::CSV, true);
    
    ASSERT_EQ(reloaded.getData().size(), 3);
    ASSERT_EQ(reloaded.getData()[0].size(), 3);
    ASSERT_EQ(reloaded.getFeatureNames().size(), 3);
    
    EXPECT_EQ(reloaded.getFeatureNames()[0], "col1");
    EXPECT_EQ(reloaded.getFeatureNames()[1], "col2");
    EXPECT_EQ(reloaded.getFeatureNames()[2], "col3");
    
    EXPECT_DOUBLE_EQ(reloaded.getData()[0][0], 1.0);
    EXPECT_DOUBLE_EQ(reloaded.getData()[1][1], 5.0);
    EXPECT_DOUBLE_EQ(reloaded.getData()[2][2], 9.0);
}

// Test dataset split
TEST_F(DataManagerTest, DatasetSplit) {
    // Create a dataset with 100 samples
    DataSet::DataMatrix data;
    DataSet::LabelVector labels;
    
    for (int i = 0; i < 100; ++i) {
        data.push_back({static_cast<double>(i), static_cast<double>(i*2)});
        labels.push_back(static_cast<double>(i*3));
    }
    
    DataSet dataset(data, labels);
    
    // Split with 80/20 ratio
    auto [trainData, testData] = dataset.split(0.2);
    
    EXPECT_EQ(trainData.getData().size(), 80);
    EXPECT_EQ(testData.getData().size(), 20);
    EXPECT_EQ(trainData.getLabels().size(), 80);
    EXPECT_EQ(testData.getLabels().size(), 20);
}

// Test adding data rows
TEST_F(DataManagerTest, AddDataRow) {
    DataSet dataset;
    
    dataset.addDataRow({1.0, 2.0, 3.0}, 4.0);
    dataset.addDataRow({5.0, 6.0, 7.0}, 8.0);
    
    ASSERT_EQ(dataset.getData().size(), 2);
    ASSERT_EQ(dataset.getLabels().size(), 2);
    
    EXPECT_DOUBLE_EQ(dataset.getData()[0][0], 1.0);
    EXPECT_DOUBLE_EQ(dataset.getData()[1][1], 6.0);
    EXPECT_DOUBLE_EQ(dataset.getLabels()[0], 4.0);
    EXPECT_DOUBLE_EQ(dataset.getLabels()[1], 8.0);
}

// Test dimensions
TEST_F(DataManagerTest, Dimensions) {
    DataSet::DataMatrix data = {
        {1.0, 2.0, 3.0},
        {4.0, 5.0, 6.0}
    };
    DataSet dataset(data);
    
    auto [rows, cols] = dataset.dimensions();
    EXPECT_EQ(rows, 2);
    EXPECT_EQ(cols, 3);
}

// Test exception on file not found
TEST_F(DataManagerTest, FileNotFound) {
    EXPECT_THROW(dataManager.loadFromFile("nonexistent_file.csv"), std::runtime_error);
}
