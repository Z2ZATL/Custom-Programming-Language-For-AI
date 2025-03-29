/**
 * @file main.cpp
 * @brief Main entry point for the data analysis program
 */

#include "DataManager.h"
#include "DataProcessor.h"
#include "ErrorHandler.h"
#include "Logger.h"
#include "Model.h"
#include "Utils.h"

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <fstream>
#include <cstring>

// Function declarations
void configureLogger();
void showUsage(const std::string& programName);
void runExample();
void loadAndPreprocessData(const std::string& filename, bool hasHeader, char delimiter);
void trainAndEvaluateModel(const std::string& modelType, const std::string& dataFile);
void runPipeline(const std::string& inputFile, const std::string& outputFile, 
                 const std::string& modelType, bool train);

int main(int argc, char* argv[]) {
    try {
        // Configure logging
        configureLogger();
        Logger::getInstance().info("Data Analyzer starting...");
        
        // Parse command line arguments
        if (argc <= 1) {
            // No arguments, run the example
            Logger::getInstance().info("No arguments provided, running example demo");
            runExample();
            return 0;
        }
        
        // Process command line arguments
        std::string command = argv[1];
        
        if (command == "--help" || command == "-h") {
            showUsage(argv[0]);
        } else if (command == "preprocess" && argc >= 3) {
            // preprocess <file> [--header] [--delimiter=<char>]
            std::string filename = argv[2];
            bool hasHeader = false;
            char delimiter = ',';
            
            for (int i = 3; i < argc; ++i) {
                if (std::strcmp(argv[i], "--header") == 0) {
                    hasHeader = true;
                } else if (std::strncmp(argv[i], "--delimiter=", 12) == 0) {
                    delimiter = argv[i][12];
                }
            }
            
            loadAndPreprocessData(filename, hasHeader, delimiter);
        } else if (command == "train" && argc >= 4) {
            // train <model_type> <data_file>
            std::string modelType = argv[2];
            std::string dataFile = argv[3];
            trainAndEvaluateModel(modelType, dataFile);
        } else if (command == "run" && argc >= 4) {
            // run <input_file> <output_file> [--model=<model_type>] [--train]
            std::string inputFile = argv[2];
            std::string outputFile = argv[3];
            std::string modelType = "linear_regression";
            bool train = false;
            
            for (int i = 4; i < argc; ++i) {
                if (std::strncmp(argv[i], "--model=", 8) == 0) {
                    modelType = argv[i] + 8;
                } else if (std::strcmp(argv[i], "--train") == 0) {
                    train = true;
                }
            }
            
            runPipeline(inputFile, outputFile, modelType, train);
        } else {
            Logger::getInstance().error("Unknown command or missing arguments");
            showUsage(argv[0]);
            return 1;
        }
        
        Logger::getInstance().info("Data Analyzer completed successfully");
        return 0;
    } catch (const std::exception& e) {
        Logger::getInstance().critical("Unhandled exception: " + std::string(e.what()));
        return 1;
    }
}

void configureLogger() {
    Logger& logger = Logger::getInstance();
    
    // Add console sink with colored output
    logger.addSink(std::make_shared<ConsoleLoggerSink>(true));
    
    // Add file sink
    std::string logFile = "data_analyzer_" + Utils::getCurrentDateTime("%Y%m%d_%H%M%S") + ".log";
    logger.addSink(std::make_shared<FileLoggerSink>(logFile));
    
    // Set log level (INFO by default, but can be configured)
    logger.setLevel(LogLevel::INFO);
}

void showUsage(const std::string& programName) {
    std::cout << "Usage: " << programName << " <command> [options]" << std::endl;
    std::cout << std::endl;
    std::cout << "Commands:" << std::endl;
    std::cout << "  preprocess <file> [--header] [--delimiter=<char>]" << std::endl;
    std::cout << "    Load and preprocess data from a file" << std::endl;
    std::cout << std::endl;
    std::cout << "  train <model_type> <data_file>" << std::endl;
    std::cout << "    Train a model on the specified data file" << std::endl;
    std::cout << "    Model types: linear_regression, logistic_regression, k_means" << std::endl;
    std::cout << std::endl;
    std::cout << "  run <input_file> <output_file> [--model=<model_type>] [--train]" << std::endl;
    std::cout << "    Run a complete pipeline (load, preprocess, train/predict, save)" << std::endl;
    std::cout << "    If --train is specified, the model will be trained on the input data" << std::endl;
    std::cout << "    Otherwise, the model will be loaded from a saved file" << std::endl;
    std::cout << std::endl;
    std::cout << "  --help, -h" << std::endl;
    std::cout << "    Show this help message" << std::endl;
}

void runExample() {
    Logger& logger = Logger::getInstance();
    logger.info("Running example with synthetic data");
    
    // Create a simple dataset
    DataSet::DataMatrix data;
    DataSet::LabelVector labels;
    
    // Generate some synthetic data for a simple linear relationship: y = 2x + 1 + noise
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    
    for (int i = 0; i < 100; ++i) {
        double x = i / 10.0;
        double y = 2 * x + 1 + ((std::rand() % 100) - 50) / 100.0; // Add some noise
        data.push_back({x});
        labels.push_back(y);
    }
    
    DataSet dataset(data, labels, {"x"});
    
    // Split into training and testing sets
    auto [trainData, testData] = dataset.split(0.2);
    logger.info("Dataset created and split into training (" + 
                std::to_string(trainData.getData().size()) + 
                " samples) and testing (" + 
                std::to_string(testData.getData().size()) + 
                " samples) sets");
    
    // Train a linear regression model
    LinearRegressionModel model;
    logger.info("Training linear regression model...");
    model.train(trainData);
    
    // Make predictions
    DataSet predictions = model.predict(testData);
    
    // Evaluate the model
    ModelMetrics metrics = model.evaluate(testData, DataSet({{0}}, testData.getLabels()));
    
    logger.info("Model parameters: " + model.getParameters());
    logger.info("Mean Squared Error: " + std::to_string(metrics.meanSquaredError));
    logger.info("R² Score: " + std::to_string(metrics.r2Score));
    
    // Save the model
    std::string modelFile = "example_model.dat";
    model.saveToFile(modelFile);
    logger.info("Model saved to " + modelFile);
    
    // Save the predictions
    DataManager dataManager;
    dataManager.saveToFile(predictions, "example_predictions.csv");
    logger.info("Predictions saved to example_predictions.csv");
}

void loadAndPreprocessData(const std::string& filename, bool hasHeader, char delimiter) {
    Logger& logger = Logger::getInstance();
    logger.info("Loading data from " + filename);
    
    try {
        // Load the data
        DataManager dataManager;
        DataSet dataset = dataManager.loadFromFile(filename, DataFormat::CSV, hasHeader, delimiter);
        
        // Log dataset info
        auto [rows, cols] = dataset.dimensions();
        logger.info("Loaded dataset with " + std::to_string(rows) + " rows and " + 
                   std::to_string(cols) + " columns");
        
        // Perform some preprocessing
        DataProcessor processor;
        
        // 1. Remove outliers
        DataSet datasetCopy = dataset;
        processor.filterOutliers(datasetCopy);
        auto [filteredRows, filteredCols] = datasetCopy.dimensions();
        logger.info("After outlier removal: " + std::to_string(filteredRows) + " rows");
        
        // 2. Normalize data
        processor.normalize(datasetCopy, NormalizationType::Z_SCORE);
        logger.info("Data normalized using Z-score normalization");
        
        // 3. Calculate statistics
        auto stats = processor.calculateStatistics(datasetCopy);
        logger.info("Dataset statistics calculated");
        
        // Save preprocessed data
        std::string outputFile = filename.substr(0, filename.find_last_of('.')) + "_preprocessed.csv";
        dataManager.saveToFile(datasetCopy, outputFile);
        logger.info("Preprocessed data saved to " + outputFile);
        
    } catch (const std::exception& e) {
        logger.error("Error preprocessing data: " + std::string(e.what()));
        throw;
    }
}

void trainAndEvaluateModel(const std::string& modelType, const std::string& dataFile) {
    Logger& logger = Logger::getInstance();
    logger.info("Training " + modelType + " model on " + dataFile);
    
    try {
        // Load the data
        DataManager dataManager;
        DataSet dataset = dataManager.loadFromFile(dataFile, DataFormat::CSV, true);
        
        // Split into training and testing sets
        auto [trainData, testData] = dataset.split(0.2);
        logger.info("Dataset split into training and testing sets");
        
        // Create and train the model
        std::unique_ptr<Model> model = ModelFactory::createModel(modelType);
        if (!model) {
            logger.error("Unknown model type: " + modelType);
            return;
        }
        
        logger.info("Training model...");
        model->train(trainData);
        
        // Make predictions
        DataSet predictions = model->predict(testData);
        
        // Evaluate the model
        ModelMetrics metrics = model->evaluate(testData, DataSet({{0}}, testData.getLabels()));
        
        // Log results
        logger.info("Model parameters: " + model->getParameters());
        logger.info("Mean Squared Error: " + std::to_string(metrics.meanSquaredError));
        if (metrics.r2Score != 0.0) {
            logger.info("R² Score: " + std::to_string(metrics.r2Score));
        }
        if (metrics.accuracy != 0.0) {
            logger.info("Accuracy: " + std::to_string(metrics.accuracy));
        }
        
        // Save the model
        std::string modelFile = modelType + "_model.dat";
        model->saveToFile(modelFile);
        logger.info("Model saved to " + modelFile);
        
        // Save the predictions
        std::string predictionFile = dataFile.substr(0, dataFile.find_last_of('.')) + "_predictions.csv";
        dataManager.saveToFile(predictions, predictionFile);
        logger.info("Predictions saved to " + predictionFile);
        
    } catch (const std::exception& e) {
        logger.error("Error training model: " + std::string(e.what()));
        throw;
    }
}

void runPipeline(const std::string& inputFile, const std::string& outputFile, 
                 const std::string& modelType, bool train) {
    Logger& logger = Logger::getInstance();
    logger.info("Running pipeline with input: " + inputFile + ", output: " + outputFile);
    
    try {
        // Load the data
        DataManager dataManager;
        DataSet dataset = dataManager.loadFromFile(inputFile, DataFormat::CSV, true);
        
        // Preprocess data
        DataProcessor processor;
        processor.normalize(dataset);
        logger.info("Data loaded and normalized");
        
        std::unique_ptr<Model> model = ModelFactory::createModel(modelType);
        if (!model) {
            logger.error("Unknown model type: " + modelType);
            return;
        }
        
        if (train) {
            // Train the model
            logger.info("Training " + modelType + " model...");
            model->train(dataset);
            
            // Save the model
            std::string modelFile = modelType + "_model.dat";
            model->saveToFile(modelFile);
            logger.info("Model trained and saved to " + modelFile);
        } else {
            // Load the model
            std::string modelFile = modelType + "_model.dat";
            if (!Utils::fileExists(modelFile)) {
                logger.error("Model file not found: " + modelFile);
                return;
            }
            
            logger.info("Loading model from " + modelFile);
            model->loadFromFile(modelFile);
        }
        
        // Make predictions
        DataSet predictions = model->predict(dataset);
        logger.info("Predictions generated");
        
        // Save the results
        dataManager.saveToFile(predictions, outputFile);
        logger.info("Results saved to " + outputFile);
        
    } catch (const std::exception& e) {
        logger.error("Error in pipeline: " + std::string(e.what()));
        throw;
    }
}
