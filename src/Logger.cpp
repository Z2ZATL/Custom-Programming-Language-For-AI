/**
 * @file Logger.cpp
 * @brief Implementation of the logging system
 */

#include "Logger.h"
#include <iomanip>
#include <sstream>
#include <ctime>

// ConsoleLoggerSink Implementation

ConsoleLoggerSink::ConsoleLoggerSink(bool colorOutput) : m_colorOutput(colorOutput) {}

void ConsoleLoggerSink::write(const LogEntry& entry) {
    std::string levelStr;
    std::string colorStart;
    std::string colorEnd;
    
    if (m_colorOutput) {
        colorEnd = "\033[0m";
        
        switch (entry.level) {
            case LogLevel::DEBUG:
                levelStr = "DEBUG";
                colorStart = "\033[36m"; // Cyan
                break;
            case LogLevel::INFO:
                levelStr = "INFO";
                colorStart = "\033[32m"; // Green
                break;
            case LogLevel::WARNING:
                levelStr = "WARNING";
                colorStart = "\033[33m"; // Yellow
                break;
            case LogLevel::ERROR:
                levelStr = "ERROR";
                colorStart = "\033[31m"; // Red
                break;
            case LogLevel::CRITICAL:
                levelStr = "CRITICAL";
                colorStart = "\033[31;1m"; // Bright Red
                break;
        }
    } else {
        colorStart = "";
        colorEnd = "";
        
        switch (entry.level) {
            case LogLevel::DEBUG:   levelStr = "DEBUG";    break;
            case LogLevel::INFO:    levelStr = "INFO";     break;
            case LogLevel::WARNING: levelStr = "WARNING";  break;
            case LogLevel::ERROR:   levelStr = "ERROR";    break;
            case LogLevel::CRITICAL:levelStr = "CRITICAL"; break;
        }
    }
    
    // Format timestamp
    auto timeT = std::chrono::system_clock::to_time_t(entry.timestamp);
    std::tm tmBuf;
    
#ifdef _WIN32
    localtime_s(&tmBuf, &timeT);
#else
    localtime_r(&timeT, &tmBuf);
#endif
    
    char timeBuf[100];
    std::strftime(timeBuf, sizeof(timeBuf), "%Y-%m-%d %H:%M:%S", &tmBuf);
    
    std::ostringstream formatter;
    formatter << "[" << timeBuf << "] " 
              << colorStart << std::setw(8) << std::left << levelStr << colorEnd 
              << " - " << entry.message;
    
    std::lock_guard<std::mutex> lock(m_mutex);
    std::cout << formatter.str() << std::endl;
}

void ConsoleLoggerSink::flush() {
    std::cout.flush();
}

// FileLoggerSink Implementation

FileLoggerSink::FileLoggerSink(const std::string& filename, bool append) 
    : m_filename(filename) {
    
    m_file.open(filename, append ? std::ios::app : std::ios::trunc);
    if (!m_file.is_open()) {
        throw std::runtime_error("Failed to open log file: " + filename);
    }
}

FileLoggerSink::~FileLoggerSink() {
    if (m_file.is_open()) {
        m_file.close();
    }
}

void FileLoggerSink::write(const LogEntry& entry) {
    std::string levelStr;
    switch (entry.level) {
        case LogLevel::DEBUG:   levelStr = "DEBUG";    break;
        case LogLevel::INFO:    levelStr = "INFO";     break;
        case LogLevel::WARNING: levelStr = "WARNING";  break;
        case LogLevel::ERROR:   levelStr = "ERROR";    break;
        case LogLevel::CRITICAL:levelStr = "CRITICAL"; break;
    }
    
    // Format timestamp
    auto timeT = std::chrono::system_clock::to_time_t(entry.timestamp);
    std::tm tmBuf;
    
#ifdef _WIN32
    localtime_s(&tmBuf, &timeT);
#else
    localtime_r(&timeT, &tmBuf);
#endif
    
    char timeBuf[100];
    std::strftime(timeBuf, sizeof(timeBuf), "%Y-%m-%d %H:%M:%S", &tmBuf);
    
    std::ostringstream formatter;
    formatter << "[" << timeBuf << "] " 
              << std::setw(8) << std::left << levelStr 
              << " - " << entry.message;
    
    std::lock_guard<std::mutex> lock(m_mutex);
    m_file << formatter.str() << std::endl;
}

void FileLoggerSink::flush() {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_file.flush();
}

// Logger Implementation

Logger::Logger() : m_level(LogLevel::INFO) {}

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

void Logger::addSink(std::shared_ptr<LoggerSink> sink) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_sinks.push_back(sink);
}

void Logger::clearSinks() {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_sinks.clear();
}

void Logger::setLevel(LogLevel level) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_level = level;
}

LogLevel Logger::getLevel() const {
    return m_level;
}

void Logger::log(LogLevel level, const std::string& message) {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    if (level < m_level) {
        return;
    }
    
    LogEntry entry(level, message);
    for (auto& sink : m_sinks) {
        sink->write(entry);
    }
}

void Logger::debug(const std::string& message) {
    log(LogLevel::DEBUG, message);
}

void Logger::info(const std::string& message) {
    log(LogLevel::INFO, message);
}

void Logger::warning(const std::string& message) {
    log(LogLevel::WARNING, message);
}

void Logger::error(const std::string& message) {
    log(LogLevel::ERROR, message);
}

void Logger::critical(const std::string& message) {
    log(LogLevel::CRITICAL, message);
}

void Logger::flush() {
    std::lock_guard<std::mutex> lock(m_mutex);
    for (auto& sink : m_sinks) {
        sink->flush();
    }
}

std::string Logger::formatTimestamp(const std::chrono::system_clock::time_point& time) {
    auto timeT = std::chrono::system_clock::to_time_t(time);
    std::tm tmBuf;
    
#ifdef _WIN32
    localtime_s(&tmBuf, &timeT);
#else
    localtime_r(&timeT, &tmBuf);
#endif
    
    char timeBuf[100];
    std::strftime(timeBuf, sizeof(timeBuf), "%Y-%m-%d %H:%M:%S", &tmBuf);
    return std::string(timeBuf);
}

std::string Logger::logLevelToString(LogLevel level) {
    switch (level) {
        case LogLevel::DEBUG:   return "DEBUG";
        case LogLevel::INFO:    return "INFO";
        case LogLevel::WARNING: return "WARNING";
        case LogLevel::ERROR:   return "ERROR";
        case LogLevel::CRITICAL:return "CRITICAL";
        default:                return "UNKNOWN";
    }
}
