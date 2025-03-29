/**
 * @file TestUtils.cpp
 * @brief Unit tests for utility functions and classes
 */

#include "Utils.h"
#include "gtest/gtest.h"
#include <cmath>
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include <thread>
#include <chrono>

// Test string splitting
TEST(UtilsTest, SplitString) {
    std::string input = "apple,banana,cherry,date";
    auto tokens = Utils::splitString(input, ',');
    
    ASSERT_EQ(tokens.size(), 4);
    EXPECT_EQ(tokens[0], "apple");
    EXPECT_EQ(tokens[1], "banana");
    EXPECT_EQ(tokens[2], "cherry");
    EXPECT_EQ(tokens[3], "date");
    
    // Test with empty input
    tokens = Utils::splitString("", ',');
    EXPECT_TRUE(tokens.empty());
    
    // Test with no delimiter
    tokens = Utils::splitString("apple", ',');
    ASSERT_EQ(tokens.size(), 1);
    EXPECT_EQ(tokens[0], "apple");
}

// Test string joining
TEST(UtilsTest, JoinStrings) {
    std::vector<std::string> tokens = {"apple", "banana", "cherry", "date"};
    std::string joined = Utils::joinStrings(tokens, ", ");
    
    EXPECT_EQ(joined, "apple, banana, cherry, date");
    
    // Test with empty vector
    joined = Utils::joinStrings({}, ", ");
    EXPECT_TRUE(joined.empty());
    
    // Test with single element
    joined = Utils::joinStrings({"apple"}, ", ");
    EXPECT_EQ(joined, "apple");
}

// Test string case conversion
TEST(UtilsTest, StringCaseConversion) {
    EXPECT_EQ(Utils::toLower("AbCdEf"), "abcdef");
    EXPECT_EQ(Utils::toUpper("AbCdEf"), "ABCDEF");
}

// Test string trimming
TEST(UtilsTest, StringTrim) {
    EXPECT_EQ(Utils::trim("  hello  "), "hello");
    EXPECT_EQ(Utils::trim("hello"), "hello");
    EXPECT_EQ(Utils::trim("   "), "");
    EXPECT_EQ(Utils::trim(""), "");
}

// Test string prefix/suffix checking
TEST(UtilsTest, StringPrefixSuffix) {
    EXPECT_TRUE(Utils::startsWith("hello world", "hello"));
    EXPECT_FALSE(Utils::startsWith("hello world", "world"));
    
    EXPECT_TRUE(Utils::endsWith("hello world", "world"));
    EXPECT_FALSE(Utils::endsWith("hello world", "hello"));
    
    // Edge cases
    EXPECT_TRUE(Utils::startsWith("hello", "hello"));
    EXPECT_TRUE(Utils::endsWith("hello", "hello"));
    EXPECT_FALSE(Utils::startsWith("hello", "hello world"));
    EXPECT_FALSE(Utils::endsWith("hello", "hello world"));
}

// Test number conversion
TEST(UtilsTest, NumberConversion) {
    // String to number
    EXPECT_EQ(Utils::stringToNumber<int>("123"), 123);
    EXPECT_DOUBLE_EQ(Utils::stringToNumber<double>("123.456"), 123.456);
    
    // Invalid conversions should return default value
    EXPECT_EQ(Utils::stringToNumber<int>("abc", 42), 42);
    
    // Number to string
    EXPECT_EQ(Utils::numberToString(123), "123");
    EXPECT_EQ(Utils::numberToString(123.456, 2), "123.46");
}

// Test random number generation
TEST(UtilsTest, RandomNumbers) {
    // Test range
    for (int i = 0; i < 100; ++i) {
        int randomInt = Utils::randomInt(1, 10);
        EXPECT_GE(randomInt, 1);
        EXPECT_LE(randomInt, 10);
        
        double randomDouble = Utils::randomDouble(1.0, 10.0);
        EXPECT_GE(randomDouble, 1.0);
        EXPECT_LE(randomDouble, 10.0);
    }
}

// Test file operations
TEST(UtilsTest, FileOperations) {
    // Test file existence
    EXPECT_FALSE(Utils::fileExists("nonexistent_file.txt"));
    
    // Create a file and test again
    std::ofstream testFile("test_utils_file.txt");
    testFile << "Test content" << std::endl;
    testFile.close();
    
    EXPECT_TRUE(Utils::fileExists("test_utils_file.txt"));
    
    // Clean up
    std::filesystem::remove("test_utils_file.txt");
}

// Test date/time formatting
TEST(UtilsTest, DateTime) {
    std::string dateTime = Utils::getCurrentDateTime("%Y-%m-%d");
    
    // Format should be YYYY-MM-DD
    EXPECT_EQ(dateTime.length(), 10);
    EXPECT_EQ(dateTime[4], '-');
    EXPECT_EQ(dateTime[7], '-');
}

// Test directory creation
TEST(UtilsTest, DirectoryCreation) {
    std::string testDir = "test_utils_dir";
    
    // Create directory
    EXPECT_TRUE(Utils::createDirectories(testDir));
    
    // Check it exists
    EXPECT_TRUE(std::filesystem::exists(testDir));
    
    // Clean up
    std::filesystem::remove(testDir);
}

// Test timer
TEST(UtilsTest, Timer) {
    Utils::Timer timer;
    
    timer.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    timer.stop();
    
    double elapsed = timer.getElapsedMilliseconds();
    
    // Should be at least 100ms, but allow some tolerance
    EXPECT_GE(elapsed, 90.0);
}

// Test CSV reader/writer
TEST(UtilsTest, CSVOperations) {
    // Create a test CSV file
    {
        Utils::CSVWriter writer("test_utils.csv");
        writer.writeHeader({"col1", "col2", "col3"});
        writer.writeRow({"1", "2", "3"});
        writer.writeRow({"4", "5", "6"});
    }
    
    // Read the CSV file
    Utils::CSVReader reader("test_utils.csv");
    auto data = reader.readAll();
    
    // Check data
    ASSERT_EQ(data.size(), 2);
    ASSERT_EQ(data[0].size(), 3);
    EXPECT_EQ(data[0][0], "1");
    EXPECT_EQ(data[1][2], "6");
    
    // Check header
    auto header = reader.getHeader();
    ASSERT_EQ(header.size(), 3);
    EXPECT_EQ(header[0], "col1");
    EXPECT_EQ(header[1], "col2");
    EXPECT_EQ(header[2], "col3");
    
    // Clean up
    std::filesystem::remove("test_utils.csv");
}

// Test vector shuffle
TEST(UtilsTest, VectorShuffle) {
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<int> original = numbers;
    
    Utils::shuffleVector(numbers);
    
    // Size should be the same
    EXPECT_EQ(numbers.size(), original.size());
    
    // Contents should be the same, but order might be different
    std::sort(numbers.begin(), numbers.end());
    EXPECT_EQ(numbers, original);
    
    // Original order is extremely unlikely
    bool isDifferent = false;
    for (size_t i = 0; i < numbers.size(); ++i) {
        if (numbers[i] != i + 1) {
            isDifferent = true;
            break;
        }
    }
    
    // This could technically fail with very small probability
    EXPECT_TRUE(isDifferent);
}
