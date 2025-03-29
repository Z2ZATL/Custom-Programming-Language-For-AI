#pragma once

#include "lexer.h"
#include <iostream>
#include <memory>
#include <unordered_map>

namespace ai_language {

// Forward declarations
class Statement;
class Expression;

// Parser
class Parser {
public:
    Parser(const std::vector<Token>& tokens);
    
    std::vector<std::unique_ptr<Statement>> parse();

private:
    std::vector<Token> tokens;
    size_t current;
    
    bool isAtEnd() const;
    Token advance();
    Token peek() const;
    Token previous() const;
    bool match(TokenType type);
    
    std::unique_ptr<Statement> parseStatement();
    std::unique_ptr<Statement> parseStartStatement();
    std::unique_ptr<Statement> parseLoadStatement();
    std::unique_ptr<Statement> parseCleanStatement();
    std::unique_ptr<Statement> parseSplitStatement();
    std::unique_ptr<Statement> parseCreateStatement();
    std::unique_ptr<Statement> parseTrainStatement();
    std::unique_ptr<Statement> parseEvaluateStatement();
    std::unique_ptr<Statement> parseShowStatement();
    std::unique_ptr<Statement> parseSaveStatement();
    std::unique_ptr<Statement> parseVisualizeStatement();
    std::unique_ptr<Statement> parsePlotStatement();
    std::unique_ptr<Statement> parseAddLayerStatement();
    std::unique_ptr<Statement> parseCompileStatement();
    std::unique_ptr<Statement> parsePredictStatement();
    std::unique_ptr<Statement> parseConfigureStatement();
    std::unique_ptr<Statement> parseCompareStatement();
    
    std::unordered_map<std::string, std::string> parseParameters();
};

// Statement base class
class Statement {
public:
    virtual ~Statement() = default;
    virtual void execute() const = 0;
};

// Specific statement classes (to be implemented)
class StartStatement : public Statement {
public:
    StartStatement(const std::string& learningType);
    void execute() const override;

private:
    std::string learningType;
};

class LoadStatement : public Statement {
public:
    LoadStatement(const std::string& filename, const std::string& type = "");
    void execute() const override;

private:
    std::string filename;
    std::string type;
};

// (อื่นๆ จะเพิ่มเติมในอนาคต)

} // namespace ai_language