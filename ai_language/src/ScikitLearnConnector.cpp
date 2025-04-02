#include "../include/ScikitLearnConnector.h"
#include <iostream>
#include <fstream>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

namespace ai_language {

ScikitLearnConnector::ScikitLearnConnector() {
    // ตรวจสอบว่ามี Python และ scikit-learn หรือไม่
    checkPythonAndScikitLearn();
}

ScikitLearnConnector::~ScikitLearnConnector() {
}

bool ScikitLearnConnector::checkPythonAndScikitLearn() {
    std::string command = "python3 -c \"import sklearn; print('scikit-learn version:', sklearn.__version__)\"";

    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        std::cerr << "ไม่สามารถเรียกใช้ Python ได้" << std::endl;
        return false;
    }

    char buffer[128];
    std::string result = "";
    while (!feof(pipe)) {
        if (fgets(buffer, 128, pipe) != NULL)
            result += buffer;
    }

    pclose(pipe);

    if (result.find("scikit-learn version") != std::string::npos) {
        std::cout << "ตรวจพบ scikit-learn: " << result;
        return true;
    } else {
        std::cerr << "ไม่พบ scikit-learn โปรดติดตั้งด้วยคำสั่ง 'pip install scikit-learn'" << std::endl;
        return false;
    }
}

bool ScikitLearnConnector::trainModel(const std::string& modelType, 
                                     const std::string& datasetPath, 
                                     const std::map<std::string, double>& parameters) {
    // สร้างไฟล์ Python ชั่วคราวสำหรับการเทรนโมเดล
    std::string pythonScript = createTemporaryPythonScript(modelType, datasetPath, parameters);

    // เรียกใช้สคริปต์ Python
    std::string command = "python3 " + pythonScript;

    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        std::cerr << "ไม่สามารถเรียกใช้สคริปต์ Python ได้" << std::endl;
        return false;
    }

    char buffer[128];
    std::string result = "";
    while (!feof(pipe)) {
        if (fgets(buffer, 128, pipe) != NULL)
            result += buffer;
    }

    pclose(pipe);

    std::cout << result << std::endl;

    // ลบไฟล์ชั่วคราว
    std::remove(pythonScript.c_str());

    return true;
}

std::string ScikitLearnConnector::createTemporaryPythonScript(
    const std::string& modelType, 
    const std::string& datasetPath, 
    const std::map<std::string, double>& parameters) const {

    std::string filename = "temp_scikit_script.py";
    std::ofstream file(filename);

    file << "import numpy as np\n";
    file << "import pandas as pd\n";
    file << "import matplotlib.pyplot as plt\n";
    file << "from sklearn.model_selection import train_test_split\n";
    file << "from sklearn.metrics import accuracy_score, precision_score, recall_score, f1_score\n";
    file << "import pickle\n\n";

    file << "# โหลดข้อมูล\n";
    file << "data = pd.read_csv('" << datasetPath << "')\n";
    file << "print('โหลดข้อมูลสำเร็จ:', data.shape)\n\n";

    file << "# แยก features และ target\n";
    file << "X = data.iloc[:, :-1]\n";
    file << "y = data.iloc[:, -1]\n\n";

    file << "# แบ่งข้อมูลสำหรับการเทรนและทดสอบ\n";
    file << "X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)\n\n";

    file << "# สร้างและเทรนโมเดล\n";
    if (modelType == "RandomForest") {
        file << "from sklearn.ensemble import RandomForestClassifier\n";
        file << "model = RandomForestClassifier(\n";
        file << "    n_estimators=" << parameters.at("trees") << ",\n";
        file << "    max_depth=" << parameters.at("max_depth") << ",\n";
        file << "    random_state=42\n";
        file << ")\n";
    } else if (modelType == "LinearRegression") {
        file << "from sklearn.linear_model import LinearRegression\n";
        file << "model = LinearRegression()\n";
    } else if (modelType == "LogisticRegression") {
        file << "from sklearn.linear_model import LogisticRegression\n";
        file << "model = LogisticRegression(random_state=42)\n";
    } else if (modelType == "SVM") {
        file << "from sklearn.svm import SVC\n";
        file << "model = SVC(random_state=42)\n";
    } else {
        file << "# โมเดลไม่รองรับ\n";
        file << "print('โมเดล " << modelType << " ไม่รองรับใน scikit-learn connector')\n";
        file << "exit(1)\n";
    }

    file << "model.fit(X_train, y_train)\n\n";

    file << "# ทำนายและประเมินโมเดล\n";
    file << "y_pred = model.predict(X_test)\n";
    file << "accuracy = accuracy_score(y_test, y_pred)\n";
    file << "try:\n";
    file << "    precision = precision_score(y_test, y_pred, average='macro')\n";
    file << "    recall = recall_score(y_test, y_pred, average='macro')\n";
    file << "    f1 = f1_score(y_test, y_pred, average='macro')\n";
    file << "    print(f'ความแม่นยำ (Accuracy): {accuracy:.4f}')\n";
    file << "    print(f'ความแม่นยำ (Precision): {precision:.4f}')\n";
    file << "    print(f'ความไว (Recall): {recall:.4f}')\n";
    file << "    print(f'F1 Score: {f1:.4f}')\n";
    file << "except Exception as e:\n";
    file << "    print(f'ไม่สามารถคำนวณ metrics ได้: {e}')\n";
    file << "    print(f'ความแม่นยำ (Accuracy): {accuracy:.4f}')\n\n";

    file << "# บันทึกโมเดล\n";
    file << "with open('scikit_model.pkl', 'wb') as f:\n";
    file << "    pickle.dump(model, f)\n";
    file << "print('บันทึกโมเดลสำเร็จที่ scikit_model.pkl')\n\n";

    file << "# แสดงกราฟ (สำหรับโมเดล classification)\n";
    file << "try:\n";
    file << "    from sklearn.decomposition import PCA\n";
    file << "    pca = PCA(n_components=2)\n";
    file << "    X_pca = pca.fit_transform(X)\n";
    file << "    plt.figure(figsize=(10, 6))\n";
    file << "    for i in np.unique(y):\n";
    file << "        plt.scatter(X_pca[y == i, 0], X_pca[y == i, 1], label=f'Class {i}')\n";
    file << "    plt.legend()\n";
    file << "    plt.title('PCA 2D Visualization')\n";
    file << "    plt.savefig('scikit_visualization.png')\n";
    file << "    print('บันทึกกราฟสำเร็จที่ scikit_visualization.png')\n";
    file << "except Exception as e:\n";
    file << "    print(f'ไม่สามารถสร้างกราฟได้: {e}')\n";

    file.close();

    return filename;
}

} // namespace ai_language