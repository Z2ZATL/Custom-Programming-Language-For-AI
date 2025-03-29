/**
 * @file ErrorHandler.h
 * @brief Header file for error handling utilities
 */

#pragma once

#include <string>
#include <stdexcept>
#include <functional>
#include "Logger.h"

/**
 * @class ErrorHandler
 * @brief Class for centralized error handling and management
 * 
 * This class provides methods to handle, log, and recover from errors
 * throughout the application.
 */
class ErrorHandler {
public:
    /**
     * @brief Constructor with logger
     * @param logger Pointer to a Logger instance
     */
    ErrorHandler(Logger* logger = nullptr);
    
    /**
     * @brief Set the logger for this error handler
     * @param logger Pointer to a Logger instance
     */
    void setLogger(Logger* logger);
    
    /**
     * @brief Register a custom exception handler for a specific exception type
     * @tparam ExceptionType Type of exception to handle
     * @param handler Function that will handle the exception
     */
    template<typename ExceptionType>
    void registerExceptionHandler(std::function<void(const ExceptionType&)> handler);
    
    /**
     * @brief Handle an exception using registered handlers or default handling
     * @param e Exception to handle
     * @return True if handled successfully, false if no handler was found
     */
    bool handleException(const std::exception& e);
    
    /**
     * @brief Execute a function safely with exception handling
     * @tparam Func Type of the function to execute
     * @tparam Args Types of the arguments to pass to the function
     * @param func Function to execute
     * @param args Arguments to pass to the function
     * @return Result of the function or default-constructed result if exception occurred
     */
    template<typename Func, typename... Args>
    auto executeSafely(Func&& func, Args&&... args) -> 
        decltype(func(std::forward<Args>(args)...));
    
    /**
     * @brief Set a recovery strategy for a specific error
     * @param errorCode Error code to associate with the recovery strategy
     * @param recovery Function that implements the recovery strategy
     */
    void setRecoveryStrategy(int errorCode, std::function<void()> recovery);
    
    /**
     * @brief Try to recover from a specific error
     * @param errorCode Error code to recover from
     * @return True if recovery was attempted, false if no strategy exists
     */
    bool attemptRecovery(int errorCode);
    
    /**
     * @brief Log an error message
     * @param message Error message to log
     * @param errorCode Optional error code
     */
    void logError(const std::string& message, int errorCode = 0);
    
    /**
     * @brief Log a warning message
     * @param message Warning message to log
     */
    void logWarning(const std::string& message);

private:
    Logger* m_logger;
    std::unordered_map<std::type_index, std::function<void(const std::exception&)>> m_exceptionHandlers;
    std::unordered_map<int, std::function<void()>> m_recoveryStrategies;
};

// Template implementation

template<typename ExceptionType>
void ErrorHandler::registerExceptionHandler(std::function<void(const ExceptionType&)> handler) {
    m_exceptionHandlers[std::type_index(typeid(ExceptionType))] = 
        [handler](const std::exception& e) {
            handler(dynamic_cast<const ExceptionType&>(e));
        };
}

template<typename Func, typename... Args>
auto ErrorHandler::executeSafely(Func&& func, Args&&... args) -> 
    decltype(func(std::forward<Args>(args)...)) {
    
    using ResultType = decltype(func(std::forward<Args>(args)...));
    
    try {
        return func(std::forward<Args>(args)...);
    } catch (const std::exception& e) {
        handleException(e);
        if constexpr (!std::is_same_v<ResultType, void>) {
            return ResultType{};
        }
    } catch (...) {
        logError("Unknown exception caught");
        if constexpr (!std::is_same_v<ResultType, void>) {
            return ResultType{};
        }
    }
}
