
#ifndef SCIKIT_LEARN_CONNECTOR_H
#define SCIKIT_LEARN_CONNECTOR_H

#include <string>
#include <map>

namespace ai_language {

class ScikitLearnConnector {
public:
    ScikitLearnConnector();
    ~ScikitLearnConnector();
    
    // ตรวจสอบว่ามี Python และ scikit-learn หรือไม่
    bool checkPythonAndScikitLearn();
    
    // เทรนโมเดลด้วย scikit-learn
    bool trainModel(const std::string& modelType, 
                   const std::string& datasetPath, 
                   const std::map<std::string, double>& parameters);

private:
    // สร้างไฟล์ Python ชั่วคราวสำหรับการเทรนโมเดล
    std::string createTemporaryPythonScript(const std::string& modelType, 
                                          const std::string& datasetPath, 
                                          const std::map<std::string, double>& parameters);
};

} // namespace ai_language

#endif // SCIKIT_LEARN_CONNECTOR_H
