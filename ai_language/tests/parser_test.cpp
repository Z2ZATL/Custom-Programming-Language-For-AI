#include <gtest/gtest.h>
#include "../include/parser.h"
#include "../include/lexer.h"

using namespace ai_language;

TEST(ParserTest, ParseEmpty) {
    Lexer lexer("");
    auto tokens = lexer.tokenize();
    Parser parser(tokens);
    
    auto statements = parser.parse();
    EXPECT_EQ(0, statements.size());
}

TEST(ParserTest, ParseStart) {
    Lexer lexer("start create ML");
    auto tokens = lexer.tokenize();
    Parser parser(tokens);
    
    auto statements = parser.parse();
    EXPECT_EQ(1, statements.size());
    // จะทดสอบการทำงานในอนาคต
}

TEST(ParserTest, ParseLoad) {
    Lexer lexer("load dataset \"data.csv\" type \"csv\"");
    auto tokens = lexer.tokenize();
    Parser parser(tokens);
    
    auto statements = parser.parse();
    EXPECT_EQ(1, statements.size());
    // จะทดสอบการทำงานในอนาคต
}

TEST(ParserTest, ParseMultipleStatements) {
    Lexer lexer("start create ML\nload dataset \"data.csv\"\nsplit data into train, test with ratio 0.8, 0.2");
    auto tokens = lexer.tokenize();
    Parser parser(tokens);
    
    auto statements = parser.parse();
    EXPECT_EQ(3, statements.size());
    // จะทดสอบการทำงานในอนาคต
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}