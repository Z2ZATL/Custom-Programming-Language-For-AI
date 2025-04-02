
#ifndef INTERPRETER_FACTORY_H
#define INTERPRETER_FACTORY_H

#include "BaseInterpreter.h"
#include <memory>
#include <string>

namespace ai_language {

class InterpreterFactory {
public:
    // สร้าง interpreter ตามประเภทที่ระบุ
    static std::unique_ptr<BaseInterpreter> createInterpreter(const std::string& type);
    
    // สร้าง interpreter แบบอัตโนมัติจากการตรวจจับคำสั่งในไฟล์
    static std::unique_ptr<BaseInterpreter> createInterpreterFromFile(const std::string& filename);
};

} // namespace ai_language

#endif // INTERPRETER_FACTORY_H
