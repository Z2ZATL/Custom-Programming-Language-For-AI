#pragma once

#include "parser.h"
#include <functional>
#include <string>

namespace ai_language {

// Interpreter
class Interpreter {
public:
    void interpret(const std::string& source);
    
    // สำหรับ API เพื่อเชื่อมต่อกับระบบอื่น
    void setOutputHandler(std::function<void(const std::string&)> handler);
    void setErrorHandler(std::function<void(const std::string&)> handler);
    
    // สถานะของโปรแกรม
    bool hasError() const;
    void reset();

private:
    std::function<void(const std::string&)> outputHandler;
    std::function<void(const std::string&)> errorHandler;
    bool error = false;
    
    void output(const std::string& message);
    void reportError(const std::string& message);
};

} // namespace ai_language