
#ifndef DL_INTERPRETER_H
#define DL_INTERPRETER_H

#include "BaseInterpreter.h"

namespace ai_language {

class DLInterpreter : public BaseInterpreter {
public:
    DLInterpreter() = default;
    ~DLInterpreter() override = default;
    
    void interpret();
    
    // Override virtual functions from BaseInterpreter
    void setDefaultParameters() override;
    void handleStartCommand() override;
    void handleCreateCommand(const std::vector<std::string>& args) override;
    void handleLoadCommand(const std::vector<std::string>& args) override;
    void handleSetCommand(const std::vector<std::string>& args) override;
    void handleTrainCommand(const std::vector<std::string>& args) override;
    void handleShowCommand(const std::vector<std::string>& args) override;
    void handleSaveCommand(const std::vector<std::string>& args) override;
    void handleHelpCommand() override;
};

} // namespace ai_language

#endif // DL_INTERPRETER_H
