/**
 * @file Utils.h
 * @brief Utility functions and classes for the project
 */

#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <functional>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <memory>
#include <type_traits>
#include <filesystem>

namespace Utils {

/**
 * @brief Split a string into tokens based on a delimiter
 * @param str String to split
 * @param delimiter Character to split on
 * @return Vector of tokens
 */
std::vector<std::string> splitString(const std::string& str, char delimiter);

/**
 * @brief Join a vector of strings into a single string
 * @param tokens Vector of strings to join
 * @param delimiter String to insert between tokens
 * @return Joined string
 */
std::string joinStrings(const std::vector<std::string>& tokens, const std::string& delimiter);

/**
 * @brief Convert a string to lowercase
 * @param str String to convert
 * @return Lowercase string
 */
std::string toLower(const std::string& str);

/**
 * @brief Convert a string to uppercase
 * @param str String to convert
 * @return Uppercase string
 */
std::string toUpper(const std::string& str);

/**
 * @brief Trim whitespace from the beginning and end of a string
 * @param str String to trim
 * @return Trimmed string
 */
std::string trim(const std::string& str);

/**
 * @brief Check if a string starts with a prefix
 * @param str String to check
 * @param prefix Prefix to look for
 * @return True if string starts with prefix
 */
bool startsWith(const std::string& str, const std::string& prefix);

/**
 * @brief Check if a string ends with a suffix
 * @param str String to check
 * @param suffix Suffix to look for
 * @return True if string ends with suffix
 */
bool endsWith(const std::string& str, const std::string& suffix);

/**
 * @brief Convert a string to a numeric type
 * @tparam T Numeric type to convert to
 * @param str String to convert
 * @param defaultValue Value to return if conversion fails
 * @return Converted value or default value
 */
template<typename T>
T stringToNumber(const std::string& str, T defaultValue = T()) {
    T value = defaultValue;
    try {
        if constexpr (std::is_integral_v<T> && !std::is_same_v<T, bool>) {
            value = static_cast<T>(std::stoll(str));
        } else if constexpr (std::is_floating_point_v<T>) {
            value = static_cast<T>(std::stod(str));
        }
    } catch (...) {
        return defaultValue;
    }
    return value;
}

/**
 * @brief Convert a numeric value to string with optional precision
 * @tparam T Numeric type to convert
 * @param value Value to convert
 * @param precision Decimal precision for floating point
 * @return Formatted string
 */
template<typename T>
std::string numberToString(T value, int precision = 6) {
    std::ostringstream ss;
    if constexpr (std::is_floating_point_v<T>) {
        ss << std::fixed << std::setprecision(precision) << value;
    } else {
        ss << value;
    }
    return ss.str();
}

/**
 * @brief Generate a random integer in a range
 * @param min Minimum value (inclusive)
 * @param max Maximum value (inclusive)
 * @return Random integer
 */
int randomInt(int min, int max);

/**
 * @brief Generate a random double in a range
 * @param min Minimum value (inclusive)
 * @param max Maximum value (inclusive)
 * @return Random double
 */
double randomDouble(double min, double max);

/**
 * @brief Shuffle a vector randomly
 * @tparam T Type of vector elements
 * @param vec Vector to shuffle
 */
template<typename T>
void shuffleVector(std::vector<T>& vec) {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(vec.begin(), vec.end(), g);
}

/**
 * @brief Check if a file exists
 * @param filename Path to file
 * @return True if file exists
 */
bool fileExists(const std::string& filename);

/**
 * @brief Get the current date and time as a formatted string
 * @param format Format string (default: ISO 8601)
 * @return Formatted date/time string
 */
std::string getCurrentDateTime(const std::string& format = "%Y-%m-%dT%H:%M:%S");

/**
 * @brief Create a directory and any parent directories
 * @param path Directory path to create
 * @return True if successful
 */
bool createDirectories(const std::string& path);

/**
 * @class Timer
 * @brief Simple timing utility for performance measurement
 */
class Timer {
public:
    /**
     * @brief Start the timer
     */
    void start();
    
    /**
     * @brief Stop the timer
     */
    void stop();
    
    /**
     * @brief Reset the timer
     */
    void reset();
    
    /**
     * @brief Get elapsed time in seconds
     * @return Elapsed seconds
     */
    double getElapsedSeconds() const;
    
    /**
     * @brief Get elapsed time in milliseconds
     * @return Elapsed milliseconds
     */
    double getElapsedMilliseconds() const;
    
private:
    std::chrono::high_resolution_clock::time_point m_startTime;
    std::chrono::high_resolution_clock::time_point m_endTime;
    bool m_running = false;
};

/**
 * @class CSVReader
 * @brief Utility class for reading CSV files
 */
class CSVReader {
public:
    /**
     * @brief Constructor
     * @param filename CSV file to read
     * @param hasHeader Whether the file has a header row
     * @param delimiter CSV delimiter character
     */
    CSVReader(const std::string& filename, bool hasHeader = true, char delimiter = ',');
    
    /**
     * @brief Read the entire CSV file
     * @return 2D vector of strings containing CSV data
     */
    std::vector<std::vector<std::string>> readAll();
    
    /**
     * @brief Get the header row
     * @return Vector of column names (empty if no header)
     */
    std::vector<std::string> getHeader() const;
    
private:
    std::string m_filename;
    bool m_hasHeader;
    char m_delimiter;
    std::vector<std::string> m_header;
};

/**
 * @class CSVWriter
 * @brief Utility class for writing CSV files
 */
class CSVWriter {
public:
    /**
     * @brief Constructor
     * @param filename CSV file to write
     * @param append Whether to append to existing file
     * @param delimiter CSV delimiter character
     */
    CSVWriter(const std::string& filename, bool append = false, char delimiter = ',');
    
    /**
     * @brief Destructor
     */
    ~CSVWriter();
    
    /**
     * @brief Write a header row
     * @param header Vector of column names
     */
    void writeHeader(const std::vector<std::string>& header);
    
    /**
     * @brief Write a data row
     * @param row Vector of values
     */
    void writeRow(const std::vector<std::string>& row);
    
    /**
     * @brief Write multiple rows at once
     * @param rows 2D vector of data
     */
    void writeRows(const std::vector<std::vector<std::string>>& rows);
    
private:
    std::string m_filename;
    char m_delimiter;
    std::ofstream m_file;
};

} // namespace Utils
