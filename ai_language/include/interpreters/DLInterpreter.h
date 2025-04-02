
#ifndef DL_INTERPRETER_H
#define DL_INTERPRETER_H

#include "BaseInterpreter.h"

namespace ai_language {

class DLInterpreter : public BaseInterpreter {
public:
    DLInterpreter() = default;
    ~DLInterpreter() override = default;
    
    void interpret();
    
    // เพิ่มฟังก์ชันที่จำเป็นสำหรับการทำงานกับ Deep Learning
};

} // namespace ai_language

#endif // DL_INTERPRETER_H
