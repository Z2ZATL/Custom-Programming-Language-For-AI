
/**
 * @file interpreter.h
 * @brief Header file for the AI Language interpreter
 */

#pragma once

#include <string>
#include <functional>
#include <map>
#include <vector>

namespace ai_language {

/**
 * @class Interpreter
 * @brief Class for interpreting AI Language source code
 */
class Interpreter {
public:
    using OutputHandler = std::function<void(const std::string&)>;
    using ErrorHandler = std::function<void(const std::string&)>;
    
    /**
     * @brief Structure to store environment variables during execution
     */
    struct Environment {
        std::string projectType;
        bool dataLoaded = false;
        bool dataCleaned = false;
        bool dataSplit = false;
        bool modelTrained = false;
        bool modelEvaluated = false;
        std::string datasetPath;
        std::string datasetType;
        double trainRatio = 0.8;
        int epochs = 10;
        std::map<std::string, double> metrics;
    };
    
    /**
     * @brief Default constructor
     */
    Interpreter();
    
    /**
     * @brief Set output handler function
     * @param handler Function to handle output messages
     */
    void setOutputHandler(OutputHandler handler);
    
    /**
     * @brief Set error handler function
     * @param handler Function to handle error messages
     */
    void setErrorHandler(ErrorHandler handler);
    
    /**
     * @brief Interpret AI Language source code
     * @param source Source code to interpret
     */
    void interpret(const std::string& source);
    
    /**
     * @brief Check if interpreter has encountered an error
     * @return True if an error has occurred
     */
    bool hasError() const;
    
    /**
     * @brief Get the last error message
     * @return Last error message
     */
    std::string getLastError() const;
    
private:
    bool m_hasError;
    std::string m_lastError;
    OutputHandler m_outputHandler;
    ErrorHandler m_errorHandler;
    
    /**
     * @brief Trim whitespace from start and end of string
     * @param str String to trim
     * @return Trimmed string
     */
    std::string trim(const std::string& str);
    
    /**
     * @brief Parse a command line into command and parameters
     * @param line Command line to parse
     * @return Pair of command string and parameter map
     */
    std::pair<std::string, std::map<std::string, std::string>> parseCommand(const std::string& line);
    
    /**
     * @brief Execute a parsed command
     * @param command Command to execute
     * @param params Parameters for the command
     * @param env Execution environment
     */
    void executeCommand(const std::string& command, 
                      const std::map<std::string, std::string>& params,
                      Environment& env);
};

} // namespace ai_language
