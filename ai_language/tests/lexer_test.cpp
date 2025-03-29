#include <gtest/gtest.h>
#include "../include/lexer.h"

using namespace ai_language;

TEST(LexerTest, TokenizeEmpty) {
    Lexer lexer("");
    auto tokens = lexer.tokenize();
    ASSERT_EQ(1, tokens.size());
    EXPECT_EQ(TokenType::END, tokens[0].type);
}

TEST(LexerTest, TokenizeStart) {
    Lexer lexer("start create ML");
    auto tokens = lexer.tokenize();
    
    ASSERT_GE(tokens.size(), 4);
    EXPECT_EQ(TokenType::START, tokens[0].type);
    EXPECT_EQ(TokenType::CREATE, tokens[1].type);
    EXPECT_EQ(TokenType::IDENTIFIER, tokens[2].type);
    EXPECT_EQ("ML", tokens[2].value);
    EXPECT_EQ(TokenType::END, tokens[3].type);
}

TEST(LexerTest, TokenizeLoad) {
    Lexer lexer("load dataset \"data.csv\" type \"csv\"");
    auto tokens = lexer.tokenize();
    
    ASSERT_GE(tokens.size(), 6);
    EXPECT_EQ(TokenType::LOAD, tokens[0].type);
    EXPECT_EQ(TokenType::IDENTIFIER, tokens[1].type);
    EXPECT_EQ("dataset", tokens[1].value);
    EXPECT_EQ(TokenType::STRING, tokens[2].type);
    EXPECT_EQ("data.csv", tokens[2].value);
    EXPECT_EQ(TokenType::IDENTIFIER, tokens[3].type);
    EXPECT_EQ("type", tokens[3].value);
    EXPECT_EQ(TokenType::STRING, tokens[4].type);
    EXPECT_EQ("csv", tokens[4].value);
}

TEST(LexerTest, TokenizeNumbers) {
    Lexer lexer("split data into train, test with ratio 0.8, 0.2");
    auto tokens = lexer.tokenize();
    
    ASSERT_GE(tokens.size(), 10);
    EXPECT_EQ(TokenType::SPLIT, tokens[0].type);
    EXPECT_EQ(TokenType::IDENTIFIER, tokens[1].type);
    EXPECT_EQ(TokenType::INTO, tokens[2].type);
    EXPECT_EQ(TokenType::IDENTIFIER, tokens[3].type); // train
    EXPECT_EQ(TokenType::IDENTIFIER, tokens[5].type); // test
    EXPECT_EQ(TokenType::WITH, tokens[6].type);
    EXPECT_EQ(TokenType::RATIO, tokens[7].type);
    EXPECT_EQ(TokenType::NUMBER, tokens[8].type);
    EXPECT_EQ("0.8", tokens[8].value);
    EXPECT_EQ(TokenType::NUMBER, tokens[10].type);
    EXPECT_EQ("0.2", tokens[10].value);
}

TEST(LexerTest, TokenizeStringWithEscapes) {
    Lexer lexer("load dataset \"file\\nwith\\tescape\\\\sequences\"");
    auto tokens = lexer.tokenize();
    
    ASSERT_GE(tokens.size(), 4);
    EXPECT_EQ(TokenType::STRING, tokens[2].type);
    EXPECT_EQ("file\nwith\tescape\\sequences", tokens[2].value);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}