#include <gtest/gtest.h>
#include "../include/interpreter.h"
#include <sstream>

using namespace ai_language;

class InterpreterTestFixture : public ::testing::Test {
protected:
    Interpreter interpreter;
    std::stringstream output;
    std::stringstream error;
    
    void SetUp() override {
        interpreter.setOutputHandler([this](const std::string& message) {
            output << message << std::endl;
        });
        
        interpreter.setErrorHandler([this](const std::string& message) {
            error << message << std::endl;
        });
    }
    
    void TearDown() override {
        output.str("");
        output.clear();
        error.str("");
        error.clear();
    }
};

TEST_F(InterpreterTestFixture, InterpretEmpty) {
    interpreter.interpret("");
    EXPECT_FALSE(interpreter.hasError());
    EXPECT_EQ("", output.str());
    EXPECT_EQ("", error.str());
}

TEST_F(InterpreterTestFixture, InterpretBasic) {
    // ตอนนี้ยังไม่ได้รองรับการทำงานอย่างเต็มที่ จะทดสอบในอนาคต
    interpreter.interpret("start create ML");
    EXPECT_FALSE(interpreter.hasError());
    // ตรวจสอบ output จะทำในอนาคตเมื่อพร้อม
}

TEST_F(InterpreterTestFixture, InterpretInvalid) {
    // ตอนนี้ยังไม่ได้ตรวจสอบ syntax error ทั้งหมด จะทดสอบในอนาคต
    interpreter.interpret("invalid command");
    // ตรวจสอบ error จะทำในอนาคตเมื่อพร้อม
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}