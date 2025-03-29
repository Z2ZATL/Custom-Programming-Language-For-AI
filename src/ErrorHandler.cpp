/**
 * @file ErrorHandler.cpp
 * @brief Implementation of ErrorHandler class
 */

#include "ErrorHandler.h"
#include <typeindex>
#include <iostream>

ErrorHandler::ErrorHandler(Logger* logger) : m_logger(logger) {}

void ErrorHandler::setLogger(Logger* logger) {
    m_logger = logger;
}

bool ErrorHandler::handleException(const std::exception& e) {
    // Try to find a handler for this specific exception type
    auto typeIndex = std::type_index(typeid(e));
    auto it = m_exceptionHandlers.find(typeIndex);
    
    if (it != m_exceptionHandlers.end()) {
        // We have a handler for this type
        it->second(e);
        return true;
    }
    
    // No specific handler, use default handling
    logError(std::string("Unhandled exception: ") + e.what());
    return false;
}

void ErrorHandler::setRecoveryStrategy(int errorCode, std::function<void()> recovery) {
    m_recoveryStrategies[errorCode] = recovery;
}

bool ErrorHandler::attemptRecovery(int errorCode) {
    auto it = m_recoveryStrategies.find(errorCode);
    if (it != m_recoveryStrategies.end()) {
        logWarning("Attempting recovery for error code " + std::to_string(errorCode));
        it->second();
        return true;
    }
    return false;
}

void ErrorHandler::logError(const std::string& message, int errorCode) {
    if (m_logger) {
        std::string fullMessage = message;
        if (errorCode != 0) {
            fullMessage += " (Error code: " + std::to_string(errorCode) + ")";
        }
        m_logger->error(fullMessage);
    } else {
        std::cerr << "ERROR: " << message;
        if (errorCode != 0) {
            std::cerr << " (Error code: " << errorCode << ")";
        }
        std::cerr << std::endl;
    }
}

void ErrorHandler::logWarning(const std::string& message) {
    if (m_logger) {
        m_logger->warning(message);
    } else {
        std::cerr << "WARNING: " << message << std::endl;
    }
}
