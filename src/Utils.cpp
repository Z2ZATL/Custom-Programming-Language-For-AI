/**
 * @file Utils.cpp
 * @brief Implementation of utility functions and classes
 */

#include "Utils.h"
#include <algorithm>
#include <random>
#include <sstream>
#include <fstream>
#include <cctype>
#include <ctime>
#include <filesystem>

namespace Utils {

std::vector<std::string> splitString(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    
    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    
    return tokens;
}

std::string joinStrings(const std::vector<std::string>& tokens, const std::string& delimiter) {
    if (tokens.empty()) {
        return "";
    }
    
    std::stringstream ss;
    ss << tokens[0];
    
    for (size_t i = 1; i < tokens.size(); ++i) {
        ss << delimiter << tokens[i];
    }
    
    return ss.str();
}

std::string toLower(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return result;
}

std::string toUpper(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return std::toupper(c); });
    return result;
}

std::string trim(const std::string& str) {
    auto wsfront = std::find_if_not(str.begin(), str.end(),
                                    [](int c) { return std::isspace(c); });
    auto wsback = std::find_if_not(str.rbegin(), str.rend(),
                                   [](int c) { return std::isspace(c); }).base();
    
    return (wsback <= wsfront ? std::string() : std::string(wsfront, wsback));
}

bool startsWith(const std::string& str, const std::string& prefix) {
    if (prefix.size() > str.size()) {
        return false;
    }
    return std::equal(prefix.begin(), prefix.end(), str.begin());
}

bool endsWith(const std::string& str, const std::string& suffix) {
    if (suffix.size() > str.size()) {
        return false;
    }
    return std::equal(suffix.rbegin(), suffix.rend(), str.rbegin());
}

int randomInt(int min, int max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(min, max);
    return dist(gen);
}

double randomDouble(double min, double max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(min, max);
    return dist(gen);
}

bool fileExists(const std::string& filename) {
    return std::filesystem::exists(filename);
}

std::string getCurrentDateTime(const std::string& format) {
    auto now = std::chrono::system_clock::now();
    auto timeT = std::chrono::system_clock::to_time_t(now);
    std::tm tmBuf;
    
#ifdef _WIN32
    localtime_s(&tmBuf, &timeT);
#else
    localtime_r(&timeT, &tmBuf);
#endif
    
    char buffer[100];
    std::strftime(buffer, sizeof(buffer), format.c_str(), &tmBuf);
    return std::string(buffer);
}

bool createDirectories(const std::string& path) {
    try {
        return std::filesystem::create_directories(path);
    } catch (const std::filesystem::filesystem_error&) {
        return false;
    }
}

// Timer implementation

void Timer::start() {
    m_startTime = std::chrono::high_resolution_clock::now();
    m_running = true;
}

void Timer::stop() {
    m_endTime = std::chrono::high_resolution_clock::now();
    m_running = false;
}

void Timer::reset() {
    m_running = false;
}

double Timer::getElapsedSeconds() const {
    auto endTime = m_running ? 
                  std::chrono::high_resolution_clock::now() : m_endTime;
    
    return std::chrono::duration<double>(endTime - m_startTime).count();
}

double Timer::getElapsedMilliseconds() const {
    return getElapsedSeconds() * 1000.0;
}

// CSVReader implementation

CSVReader::CSVReader(const std::string& filename, bool hasHeader, char delimiter)
    : m_filename(filename), m_hasHeader(hasHeader), m_delimiter(delimiter) {}

std::vector<std::vector<std::string>> CSVReader::readAll() {
    std::vector<std::vector<std::string>> data;
    std::ifstream file(m_filename);
    
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + m_filename);
    }
    
    std::string line;
    
    // Read header if needed
    if (m_hasHeader && std::getline(file, line)) {
        m_header = splitString(line, m_delimiter);
    }
    
    // Read data
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        std::vector<std::string> row = splitString(line, m_delimiter);
        data.push_back(row);
    }
    
    return data;
}

std::vector<std::string> CSVReader::getHeader() const {
    return m_header;
}

// CSVWriter implementation

CSVWriter::CSVWriter(const std::string& filename, bool append, char delimiter)
    : m_filename(filename), m_delimiter(delimiter) {
    
    m_file.open(filename, append ? std::ios::app : std::ios::trunc);
    if (!m_file.is_open()) {
        throw std::runtime_error("Failed to open file for writing: " + filename);
    }
}

CSVWriter::~CSVWriter() {
    if (m_file.is_open()) {
        m_file.close();
    }
}

void CSVWriter::writeHeader(const std::vector<std::string>& header) {
    writeRow(header);
}

void CSVWriter::writeRow(const std::vector<std::string>& row) {
    if (!m_file.is_open()) {
        throw std::runtime_error("File is not open for writing");
    }
    
    m_file << joinStrings(row, std::string(1, m_delimiter)) << '\n';
}

void CSVWriter::writeRows(const std::vector<std::vector<std::string>>& rows) {
    for (const auto& row : rows) {
        writeRow(row);
    }
}

} // namespace Utils
