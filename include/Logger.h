/**
 * @file Logger.h
 * @brief Header file for the logging system
 */

#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <mutex>
#include <chrono>
#include <memory>
#include <sstream>
#include <vector>

/**
 * @enum LogLevel
 * @brief Available logging levels in order of severity
 */
enum class LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    CRITICAL
};

/**
 * @class LogEntry
 * @brief Represents a single log entry with timestamp, level, and message
 */
struct LogEntry {
    std::chrono::system_clock::time_point timestamp;
    LogLevel level;
    std::string message;
    
    LogEntry(LogLevel lvl, const std::string& msg)
        : timestamp(std::chrono::system_clock::now()),
          level(lvl),
          message(msg) {}
};

/**
 * @class LoggerSink
 * @brief Abstract base class for log sinks (destinations)
 */
class LoggerSink {
public:
    virtual ~LoggerSink() = default;
    
    /**
     * @brief Write a log entry to this sink
     * @param entry Log entry to write
     */
    virtual void write(const LogEntry& entry) = 0;
    
    /**
     * @brief Flush any buffered log entries
     */
    virtual void flush() = 0;
};

/**
 * @class ConsoleLoggerSink
 * @brief Logger sink that writes to the console
 */
class ConsoleLoggerSink : public LoggerSink {
public:
    /**
     * @brief Constructor
     * @param colorOutput Whether to use colors in console output
     */
    ConsoleLoggerSink(bool colorOutput = true);
    
    void write(const LogEntry& entry) override;
    void flush() override;
    
private:
    bool m_colorOutput;
    std::mutex m_mutex;
};

/**
 * @class FileLoggerSink
 * @brief Logger sink that writes to a file
 */
class FileLoggerSink : public LoggerSink {
public:
    /**
     * @brief Constructor
     * @param filename Path to the log file
     * @param append Whether to append to existing file
     */
    FileLoggerSink(const std::string& filename, bool append = true);
    
    ~FileLoggerSink();
    
    void write(const LogEntry& entry) override;
    void flush() override;
    
private:
    std::string m_filename;
    std::ofstream m_file;
    std::mutex m_mutex;
};

/**
 * @class Logger
 * @brief Main logger class that distributes log entries to sinks
 */
class Logger {
public:
    /**
     * @brief Default constructor
     */
    Logger();
    
    /**
     * @brief Get the singleton instance
     * @return Reference to the singleton logger
     */
    static Logger& getInstance();
    
    /**
     * @brief Add a sink to the logger
     * @param sink Shared pointer to a LoggerSink
     */
    void addSink(std::shared_ptr<LoggerSink> sink);
    
    /**
     * @brief Remove all sinks
     */
    void clearSinks();
    
    /**
     * @brief Set the minimum log level that will be processed
     * @param level Minimum log level
     */
    void setLevel(LogLevel level);
    
    /**
     * @brief Get the current minimum log level
     * @return Current minimum log level
     */
    LogLevel getLevel() const;
    
    /**
     * @brief Log a message with the specified level
     * @param level Log level for this message
     * @param message Message to log
     */
    void log(LogLevel level, const std::string& message);
    
    /**
     * @brief Log a debug message
     * @param message Message to log
     */
    void debug(const std::string& message);
    
    /**
     * @brief Log an info message
     * @param message Message to log
     */
    void info(const std::string& message);
    
    /**
     * @brief Log a warning message
     * @param message Message to log
     */
    void warning(const std::string& message);
    
    /**
     * @brief Log an error message
     * @param message Message to log
     */
    void error(const std::string& message);
    
    /**
     * @brief Log a critical message
     * @param message Message to log
     */
    void critical(const std::string& message);
    
    /**
     * @brief Flush all sinks
     */
    void flush();
    
private:
    std::vector<std::shared_ptr<LoggerSink>> m_sinks;
    LogLevel m_level;
    std::mutex m_mutex;
    
    std::string formatTimestamp(const std::chrono::system_clock::time_point& time);
    std::string logLevelToString(LogLevel level);
};
