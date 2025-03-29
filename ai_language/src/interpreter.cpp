#include "../include/interpreter.h"
#include "../include/lexer.h"
#include <iostream>

namespace ai_language {

void Interpreter::interpret(const std::string& source) {
    // Reset error state
    error = false;
    
    try {
        // 1. Tokenize the source code
        Lexer lexer(source);
        std::vector<Token> tokens = lexer.tokenize();
        
        // For debug
        /*
        output("Tokens:");
        for (const auto& token : tokens) {
            if (token.type == TokenType::END) break;
            output("Type: " + std::to_string(static_cast<int>(token.type)) + ", Value: " + token.value);
        }
        */
        
        // 2. Parse the tokens (ยังไม่ได้ใช้จริง)
        Parser parser(tokens);
        auto statements = parser.parse();
        
        // 3. Execute the statements
        for (const auto& stmt : statements) {
            stmt->execute();
        }
    } catch (const std::exception& e) {
        reportError("Interpretation error: " + std::string(e.what()));
    }
}

void Interpreter::setOutputHandler(std::function<void(const std::string&)> handler) {
    outputHandler = std::move(handler);
}

void Interpreter::setErrorHandler(std::function<void(const std::string&)> handler) {
    errorHandler = std::move(handler);
}

bool Interpreter::hasError() const {
    return error;
}

void Interpreter::reset() {
    error = false;
}

void Interpreter::output(const std::string& message) {
    if (outputHandler) {
        outputHandler(message);
    } else {
        std::cout << message << std::endl;
    }
}

void Interpreter::reportError(const std::string& message) {
    error = true;
    if (errorHandler) {
        errorHandler(message);
    } else {
        std::cerr << "Error: " << message << std::endl;
    }
}

} // namespace ai_language