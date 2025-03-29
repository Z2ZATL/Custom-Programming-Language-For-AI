/**
 * @file Model.h
 * @brief Header file for machine learning model classes
 */

#pragma once

#include "DataManager.h"
#include <memory>
#include <random>
#include <algorithm>
#include <functional>
#include <unordered_map>
#include <string>

#ifdef USE_EIGEN
#include <Eigen/Dense>
#endif

/**
 * @enum ModelType
 * @brief Types of machine learning models supported
 */
enum class ModelType {
    LINEAR_REGRESSION,
    LOGISTIC_REGRESSION,
    K_MEANS,
    NONE
};

/**
 * @struct ModelMetrics
 * @brief Container for model evaluation metrics
 */
struct ModelMetrics {
    double accuracy = 0.0;
    double precision = 0.0;
    double recall = 0.0;
    double f1Score = 0.0;
    double meanSquaredError = 0.0;
    double r2Score = 0.0;
    std::unordered_map<std::string, double> customMetrics;
    
    /**
     * @brief Add a custom metric
     * @param name Name of the metric
     * @param value Value of the metric
     */
    void addMetric(const std::string& name, double value) {
        customMetrics[name] = value;
    }
    
    /**
     * @brief Get a string representation of all metrics
     * @return Formatted string of metrics
     */
    std::string toString() const;
};

/**
 * @class Model
 * @brief Abstract base class for all machine learning models
 */
class Model {
public:
    virtual ~Model() = default;
    
    /**
     * @brief Train the model using the provided dataset
     * @param dataset Training dataset
     * @return True if training was successful
     */
    virtual bool train(const DataSet& dataset) = 0;
    
    /**
     * @brief Make predictions using the trained model
     * @param features Features to make predictions for
     * @return DataSet containing predictions
     */
    virtual DataSet predict(const DataSet& features) const = 0;
    
    /**
     * @brief Evaluate model performance on a test dataset
     * @param testFeatures Features for testing
     * @param testLabels True labels for testing
     * @return ModelMetrics containing evaluation results
     */
    virtual ModelMetrics evaluate(const DataSet& testFeatures, 
                                 const DataSet& testLabels) const = 0;
    
    /**
     * @brief Save the model to a file
     * @param filename File to save the model to
     * @return True if save was successful
     */
    virtual bool saveToFile(const std::string& filename) const = 0;
    
    /**
     * @brief Load a model from a file
     * @param filename File to load the model from
     * @return True if load was successful
     */
    virtual bool loadFromFile(const std::string& filename) = 0;
    
    /**
     * @brief Get the type of this model
     * @return ModelType of this model
     */
    virtual ModelType getType() const = 0;
    
    /**
     * @brief Get model parameters as a string
     * @return String representation of model parameters
     */
    virtual std::string getParameters() const = 0;
};

#ifdef USE_EIGEN
/**
 * @class LinearRegressionModel
 * @brief Linear regression implementation using Eigen
 */
class LinearRegressionModel : public Model {
public:
    /**
     * @brief Constructor with optional learning rate and iterations
     * @param learningRate Learning rate for gradient descent
     * @param maxIterations Maximum number of iterations
     * @param useGradientDescent Whether to use gradient descent (otherwise uses normal equation)
     */
    LinearRegressionModel(double learningRate = 0.01, 
                         int maxIterations = 1000,
                         bool useGradientDescent = false);
    
    bool train(const DataSet& dataset) override;
    
    DataSet predict(const DataSet& features) const override;
    
    ModelMetrics evaluate(const DataSet& testFeatures, 
                         const DataSet& testLabels) const override;
    
    bool saveToFile(const std::string& filename) const override;
    
    bool loadFromFile(const std::string& filename) override;
    
    ModelType getType() const override { return ModelType::LINEAR_REGRESSION; }
    
    std::string getParameters() const override;
    
private:
    Eigen::VectorXd m_weights;
    double m_bias;
    double m_learningRate;
    int m_maxIterations;
    bool m_useGradientDescent;
    
    Eigen::MatrixXd datasetToEigenMatrix(const DataSet& dataset) const;
    Eigen::VectorXd labelsToEigenVector(const DataSet& dataset) const;
    void trainUsingNormalEquation(const Eigen::MatrixXd& X, const Eigen::VectorXd& y);
    void trainUsingGradientDescent(const Eigen::MatrixXd& X, const Eigen::VectorXd& y);
};

/**
 * @class LogisticRegressionModel
 * @brief Logistic regression implementation using Eigen
 */
class LogisticRegressionModel : public Model {
public:
    /**
     * @brief Constructor with optional learning rate and iterations
     * @param learningRate Learning rate for gradient descent
     * @param maxIterations Maximum number of iterations
     */
    LogisticRegressionModel(double learningRate = 0.01, 
                          int maxIterations = 1000);
    
    bool train(const DataSet& dataset) override;
    
    DataSet predict(const DataSet& features) const override;
    
    ModelMetrics evaluate(const DataSet& testFeatures, 
                         const DataSet& testLabels) const override;
    
    bool saveToFile(const std::string& filename) const override;
    
    bool loadFromFile(const std::string& filename) override;
    
    ModelType getType() const override { return ModelType::LOGISTIC_REGRESSION; }
    
    std::string getParameters() const override;
    
private:
    Eigen::VectorXd m_weights;
    double m_bias;
    double m_learningRate;
    int m_maxIterations;
    
    Eigen::MatrixXd datasetToEigenMatrix(const DataSet& dataset) const;
    Eigen::VectorXd labelsToEigenVector(const DataSet& dataset) const;
    double sigmoid(double x) const;
    Eigen::VectorXd sigmoid(const Eigen::VectorXd& x) const;
};

/**
 * @class KMeansModel
 * @brief K-means clustering implementation using Eigen
 */
class KMeansModel : public Model {
public:
    /**
     * @brief Constructor with number of clusters and max iterations
     * @param k Number of clusters
     * @param maxIterations Maximum number of iterations
     */
    KMeansModel(int k = 3, int maxIterations = 100);
    
    bool train(const DataSet& dataset) override;
    
    DataSet predict(const DataSet& features) const override;
    
    ModelMetrics evaluate(const DataSet& testFeatures, 
                         const DataSet& testLabels) const override;
    
    bool saveToFile(const std::string& filename) const override;
    
    bool loadFromFile(const std::string& filename) override;
    
    ModelType getType() const override { return ModelType::K_MEANS; }
    
    std::string getParameters() const override;
    
    /**
     * @brief Get the centroids of the clusters
     * @return DataSet containing the centroids
     */
    DataSet getCentroids() const;
    
private:
    int m_k;
    int m_maxIterations;
    Eigen::MatrixXd m_centroids;
    
    Eigen::MatrixXd datasetToEigenMatrix(const DataSet& dataset) const;
    Eigen::MatrixXd initializeCentroids(const Eigen::MatrixXd& data);
    Eigen::VectorXi assignToClusters(const Eigen::MatrixXd& data);
    Eigen::MatrixXd updateCentroids(const Eigen::MatrixXd& data, const Eigen::VectorXi& labels);
    double computeDistance(const Eigen::VectorXd& a, const Eigen::VectorXd& b) const;
};
#else
// Fallback implementations without Eigen
class LinearRegressionModel : public Model {
public:
    LinearRegressionModel(double learningRate = 0.01, 
                         int maxIterations = 1000,
                         bool useGradientDescent = false);
    
    bool train(const DataSet& dataset) override;
    DataSet predict(const DataSet& features) const override;
    ModelMetrics evaluate(const DataSet& testFeatures, 
                         const DataSet& testLabels) const override;
    bool saveToFile(const std::string& filename) const override;
    bool loadFromFile(const std::string& filename) override;
    ModelType getType() const override { return ModelType::LINEAR_REGRESSION; }
    std::string getParameters() const override;
    
private:
    std::vector<double> m_weights;
    double m_bias;
    double m_learningRate;
    int m_maxIterations;
    bool m_useGradientDescent;
};

class LogisticRegressionModel : public Model {
public:
    LogisticRegressionModel(double learningRate = 0.01, 
                          int maxIterations = 1000);
    
    bool train(const DataSet& dataset) override;
    DataSet predict(const DataSet& features) const override;
    ModelMetrics evaluate(const DataSet& testFeatures, 
                         const DataSet& testLabels) const override;
    bool saveToFile(const std::string& filename) const override;
    bool loadFromFile(const std::string& filename) override;
    ModelType getType() const override { return ModelType::LOGISTIC_REGRESSION; }
    std::string getParameters() const override;
    
private:
    std::vector<double> m_weights;
    double m_bias;
    double m_learningRate;
    int m_maxIterations;
    
    double sigmoid(double x) const;
};

class KMeansModel : public Model {
public:
    KMeansModel(int k = 3, int maxIterations = 100);
    
    bool train(const DataSet& dataset) override;
    DataSet predict(const DataSet& features) const override;
    ModelMetrics evaluate(const DataSet& testFeatures, 
                         const DataSet& testLabels) const override;
    bool saveToFile(const std::string& filename) const override;
    bool loadFromFile(const std::string& filename) override;
    ModelType getType() const override { return ModelType::K_MEANS; }
    std::string getParameters() const override;
    
    DataSet getCentroids() const;
    
private:
    int m_k;
    int m_maxIterations;
    std::vector<std::vector<double>> m_centroids;
};
#endif

/**
 * @class ModelFactory
 * @brief Factory class for creating model instances
 */
class ModelFactory {
public:
    /**
     * @brief Create a model by type
     * @param type Type of model to create
     * @return Unique pointer to the created model
     */
    static std::unique_ptr<Model> createModel(ModelType type);
    
    /**
     * @brief Create a model by name
     * @param name Name of the model type
     * @return Unique pointer to the created model
     */
    static std::unique_ptr<Model> createModel(const std::string& name);
    
private:
    static std::unordered_map<std::string, ModelType> initModelMap();
    static const std::unordered_map<std::string, ModelType> MODEL_NAME_MAP;
};
