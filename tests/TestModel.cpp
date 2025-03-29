/**
 * @file TestModel.cpp
 * @brief Unit tests for the Model classes
 */

#include "Model.h"
#include "gtest/gtest.h"
#include <cmath>
#include <memory>
#include <filesystem>
#include <string>

class ModelTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create a simple dataset for linear relationship: y = 2x + 1
        linearData = {
            {1.0}, {2.0}, {3.0}, {4.0}, {5.0},
            {6.0}, {7.0}, {8.0}, {9.0}, {10.0}
        };
        
        linearLabels = {
            3.0, 5.0, 7.0, 9.0, 11.0,
            13.0, 15.0, 17.0, 19.0, 21.0
        };
        
        linearDataset = DataSet(linearData, linearLabels);
        
        // Create a dataset for logistic regression (binary classification)
        // Points with x < 5 are class 0, points with x >= 5 are class 1
        logisticData = linearData;
        logisticLabels = {
            0.0, 0.0, 0.0, 0.0, 0.0,
            1.0, 1.0, 1.0, 1.0, 1.0
        };
        
        logisticDataset = DataSet(logisticData, logisticLabels);
        
        // Create a dataset for clustering
        clusteringData = {
            {1.0, 2.0}, {1.5, 1.8}, {1.2, 2.2}, // Cluster 1
            {5.0, 6.0}, {5.5, 5.8}, {5.2, 6.2}, // Cluster 2
            {9.0, 10.0}, {9.5, 9.8}, {9.2, 10.2} // Cluster 3
        };
        
        clusteringDataset = DataSet(clusteringData);
    }

    void TearDown() override {
        // Remove model files if they exist
        if (std::filesystem::exists("linear_model_test.dat")) {
            std::filesystem::remove("linear_model_test.dat");
        }
        if (std::filesystem::exists("logistic_model_test.dat")) {
            std::filesystem::remove("logistic_model_test.dat");
        }
        if (std::filesystem::exists("kmeans_model_test.dat")) {
            std::filesystem::remove("kmeans_model_test.dat");
        }
    }

    DataSet::DataMatrix linearData;
    DataSet::LabelVector linearLabels;
    DataSet linearDataset;
    
    DataSet::DataMatrix logisticData;
    DataSet::LabelVector logisticLabels;
    DataSet logisticDataset;
    
    DataSet::DataMatrix clusteringData;
    DataSet clusteringDataset;
};

// Test linear regression model
TEST_F(ModelTest, LinearRegression) {
    LinearRegressionModel model;
    
    // Train the model
    EXPECT_TRUE(model.train(linearDataset));
    
    // Make predictions
    DataSet predictions = model.predict(linearDataset);
    
    // Check predictions (should be close to 2x + 1)
    ASSERT_EQ(predictions.getData().size(), linearData.size());
    
    for (size_t i = 0; i < predictions.getData().size(); ++i) {
        double x = linearData[i][0];
        double expected = 2.0 * x + 1.0;
        double predicted = predictions.getData()[i][0];
        
        EXPECT_NEAR(predicted, expected, 0.5); // Allow some tolerance
    }
    
    // Evaluate the model
    ModelMetrics metrics = model.evaluate(linearDataset, DataSet({{0}}, linearLabels));
    
    // Should have a very good R² score
    EXPECT_GT(metrics.r2Score, 0.95);
    
    // Test save and load
    EXPECT_TRUE(model.saveToFile("linear_model_test.dat"));
    
    LinearRegressionModel loadedModel;
    EXPECT_TRUE(loadedModel.loadFromFile("linear_model_test.dat"));
    
    // Loaded model should make similar predictions
    DataSet loadedPredictions = loadedModel.predict(linearDataset);
    
    for (size_t i = 0; i < predictions.getData().size(); ++i) {
        EXPECT_NEAR(loadedPredictions.getData()[i][0], predictions.getData()[i][0], 1e-6);
    }
}

#ifdef USE_EIGEN
// Test logistic regression model
TEST_F(ModelTest, LogisticRegression) {
    LogisticRegressionModel model;
    
    // Train the model
    EXPECT_TRUE(model.train(logisticDataset));
    
    // Make predictions
    DataSet predictions = model.predict(logisticDataset);
    
    // Check predictions (should classify early points as 0, later points as 1)
    ASSERT_EQ(predictions.getData().size(), logisticData.size());
    
    // Points with x < 5 should have predictions closer to 0
    // Points with x >= 5 should have predictions closer to 1
    for (size_t i = 0; i < predictions.getData().size(); ++i) {
        double x = logisticData[i][0];
        double predicted = predictions.getData()[i][0];
        
        if (x < 5.0) {
            EXPECT_LT(predicted, 0.5); // Should predict class 0
        } else {
            EXPECT_GE(predicted, 0.5); // Should predict class 1
        }
    }
    
    // Evaluate the model
    ModelMetrics metrics = model.evaluate(logisticDataset, DataSet({{0}}, logisticLabels));
    
    // Should have good accuracy
    EXPECT_GT(metrics.accuracy, 0.9);
    
    // Test save and load
    EXPECT_TRUE(model.saveToFile("logistic_model_test.dat"));
    
    LogisticRegressionModel loadedModel;
    EXPECT_TRUE(loadedModel.loadFromFile("logistic_model_test.dat"));
    
    // Loaded model should make similar predictions
    DataSet loadedPredictions = loadedModel.predict(logisticDataset);
    
    for (size_t i = 0; i < predictions.getData().size(); ++i) {
        EXPECT_NEAR(loadedPredictions.getData()[i][0], predictions.getData()[i][0], 1e-6);
    }
}

// Test K-means model
TEST_F(ModelTest, KMeans) {
    KMeansModel model(3); // 3 clusters
    
    // Train the model
    EXPECT_TRUE(model.train(clusteringDataset));
    
    // Make predictions (get cluster assignments)
    DataSet clusterAssignments = model.predict(clusteringDataset);
    
    // Check that we have 3 clusters
    std::set<int> uniqueClusters;
    for (const auto& row : clusterAssignments.getData()) {
        uniqueClusters.insert(static_cast<int>(row[0]));
    }
    
    EXPECT_EQ(uniqueClusters.size(), 3);
    
    // Get the centroids
    DataSet centroids = model.getCentroids();
    EXPECT_EQ(centroids.getData().size(), 3); // 3 centroids
    
    // Test save and load
    EXPECT_TRUE(model.saveToFile("kmeans_model_test.dat"));
    
    KMeansModel loadedModel;
    EXPECT_TRUE(loadedModel.loadFromFile("kmeans_model_test.dat"));
    
    // Loaded model should assign same clusters
    DataSet loadedAssignments = loadedModel.predict(clusteringDataset);
    
    // The cluster IDs might be different after loading, since K-means
    // doesn't guarantee the same cluster ordering. Instead, test that
    // points that were in the same cluster before are still in the
    // same cluster after loading.
    
    // This logic checks that if points i and j had the same cluster before,
    // they should have the same cluster after loading
    for (size_t i = 0; i < clusterAssignments.getData().size(); ++i) {
        for (size_t j = i + 1; j < clusterAssignments.getData().size(); ++j) {
            bool sameClusterBefore = 
                clusterAssignments.getData()[i][0] == clusterAssignments.getData()[j][0];
            bool sameClusterAfter = 
                loadedAssignments.getData()[i][0] == loadedAssignments.getData()[j][0];
            
            EXPECT_EQ(sameClusterBefore, sameClusterAfter);
        }
    }
}
#endif

// Test model factory
TEST_F(ModelTest, ModelFactory) {
    // Create models by type
    auto linearModel = ModelFactory::createModel(ModelType::LINEAR_REGRESSION);
    EXPECT_EQ(linearModel->getType(), ModelType::LINEAR_REGRESSION);
    
    auto logisticModel = ModelFactory::createModel(ModelType::LOGISTIC_REGRESSION);
    EXPECT_EQ(logisticModel->getType(), ModelType::LOGISTIC_REGRESSION);
    
    auto kmeansModel = ModelFactory::createModel(ModelType::K_MEANS);
    EXPECT_EQ(kmeansModel->getType(), ModelType::K_MEANS);
    
    // Create models by name
    auto linearByName = ModelFactory::createModel("linear_regression");
    EXPECT_EQ(linearByName->getType(), ModelType::LINEAR_REGRESSION);
    
    auto logisticByName = ModelFactory::createModel("logistic_regression");
    EXPECT_EQ(logisticByName->getType(), ModelType::LOGISTIC_REGRESSION);
    
    auto kmeansByName = ModelFactory::createModel("k_means");
    EXPECT_EQ(kmeansByName->getType(), ModelType::K_MEANS);
    
    // Invalid name should return nullptr
    auto invalidModel = ModelFactory::createModel("invalid_model_name");
    EXPECT_EQ(invalidModel, nullptr);
}
