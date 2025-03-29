#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <algorithm>

// นี่คือคอมเมนต์บรรทัดเดียว

/*
 * นี่คือคอมเมนต์หลายบรรทัด
 * คุณสามารถเขียนหลายบรรทัดได้
 */

// นิยามค่าคงที่
#define PI 3.14159265359
constexpr double e = 2.7182818284;

// ประกาศฟังก์ชัน
void hello_world();
int add(int a, int b);
double calculate_area(double radius);

// ประกาศคลาส
class Person {
private:
    std::string name;
    int age;

public:
    // คอนสตรัคเตอร์
    Person(const std::string& name, int age) : name(name), age(age) {}
    
    // getter methods
    std::string getName() const { return name; }
    int getAge() const { return age; }
    
    // setter methods
    void setName(const std::string& new_name) { name = new_name; }
    void setAge(int new_age) { age = new_age; }
    
    // เมธอดอื่นๆ
    void sayHello() const {
        std::cout << "สวัสดี! ฉันชื่อ " << name << " อายุ " << age << " ปี" << std::endl;
    }
};

// สืบทอดคลาส
class Student : public Person {
private:
    std::string school;
    double gpa;
    
public:
    // คอนสตรัคเตอร์ ที่เรียกคอนสตรัคเตอร์ของคลาสแม่
    Student(const std::string& name, int age, const std::string& school, double gpa)
        : Person(name, age), school(school), gpa(gpa) {}
    
    // เมธอดที่โอเวอร์ไรด์
    void sayHello() const {
        std::cout << "สวัสดี! ฉันชื่อ " << getName() << " อายุ " << getAge() 
                  << " ปี เรียนที่ " << school << " เกรดเฉลี่ย " << gpa << std::endl;
    }
    
    // เพิ่มเมธอดใหม่
    void study() const {
        std::cout << getName() << " กำลังเรียนหนังสือ..." << std::endl;
    }
};

// เทมเพลต
template <typename T>
T maximum(T a, T b) {
    return (a > b) ? a : b;
}

// enum
enum Color { RED, GREEN, BLUE };

// enum class (scoped enumeration)
enum class Day { MONDAY, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY, SATURDAY, SUNDAY };

int main() {
    std::cout << "=========== ทดสอบไวยากรณ์ภาษา C++ ===========" << std::endl;
    
    // ===================== ตัวแปรพื้นฐาน ======================
    std::cout << "\n----- ตัวแปรพื้นฐาน -----" << std::endl;
    
    int number = 42;
    double decimal = 3.14;
    char character = 'A';
    bool flag = true;
    
    std::cout << "int: " << number << std::endl;
    std::cout << "double: " << decimal << std::endl;
    std::cout << "char: " << character << std::endl;
    std::cout << "bool: " << std::boolalpha << flag << std::endl;
    
    // ===================== ข้อความ ======================
    std::cout << "\n----- ข้อความ (String) -----" << std::endl;
    
    std::string greeting = "สวัสดี";
    std::string name = "ชาวโลก";
    std::string full_greeting = greeting + " " + name;
    
    std::cout << full_greeting << std::endl;
    std::cout << "ความยาวข้อความ: " << full_greeting.length() << std::endl;
    std::cout << "อักขระที่ 0: " << full_greeting[0] << std::endl;
    
    // ===================== การควบคุมการทำงาน ======================
    std::cout << "\n----- การควบคุมการทำงาน (Control Flow) -----" << std::endl;
    
    // if-else
    int x = 10;
    if (x > 5) {
        std::cout << "x มากกว่า 5" << std::endl;
    } else if (x == 5) {
        std::cout << "x เท่ากับ 5" << std::endl;
    } else {
        std::cout << "x น้อยกว่า 5" << std::endl;
    }
    
    // switch
    int day = 3;
    switch (day) {
        case 1:
            std::cout << "วันจันทร์" << std::endl;
            break;
        case 2:
            std::cout << "วันอังคาร" << std::endl;
            break;
        case 3:
            std::cout << "วันพุธ" << std::endl;
            break;
        default:
            std::cout << "วันอื่นๆ" << std::endl;
    }
    
    // ลูป for
    std::cout << "ลูป for: ";
    for (int i = 0; i < 5; i++) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    
    // ลูป while
    std::cout << "ลูป while: ";
    int j = 0;
    while (j < 5) {
        std::cout << j << " ";
        j++;
    }
    std::cout << std::endl;
    
    // ลูป do-while
    std::cout << "ลูป do-while: ";
    int k = 0;
    do {
        std::cout << k << " ";
        k++;
    } while (k < 5);
    std::cout << std::endl;
    
    // ===================== อาร์เรย์และเวกเตอร์ ======================
    std::cout << "\n----- อาร์เรย์และเวกเตอร์ -----" << std::endl;
    
    // อาร์เรย์แบบดั้งเดิม
    int numbers[5] = {1, 2, 3, 4, 5};
    std::cout << "อาร์เรย์: ";
    for (int i = 0; i < 5; i++) {
        std::cout << numbers[i] << " ";
    }
    std::cout << std::endl;
    
    // เวกเตอร์
    std::vector<int> vec = {10, 20, 30, 40, 50};
    std::cout << "เวกเตอร์: ";
    for (const auto& v : vec) {
        std::cout << v << " ";
    }
    std::cout << std::endl;
    
    vec.push_back(60);  // เพิ่มข้อมูล
    std::cout << "เวกเตอร์หลังจากเพิ่มข้อมูล: ";
    for (const auto& v : vec) {
        std::cout << v << " ";
    }
    std::cout << std::endl;
    
    // ===================== แมพ (Map) ======================
    std::cout << "\n----- แมพ (Map) -----" << std::endl;
    
    std::map<std::string, int> ages;
    ages["สมชาย"] = 25;
    ages["สมหญิง"] = 23;
    ages["สมศรี"] = 30;
    
    std::cout << "อายุของสมชาย: " << ages["สมชาย"] << std::endl;
    std::cout << "รายการทั้งหมดในแมพ:" << std::endl;
    for (const auto& pair : ages) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }
    
    // ===================== ฟังก์ชัน ======================
    std::cout << "\n----- ฟังก์ชัน -----" << std::endl;
    
    hello_world();
    std::cout << "1 + 2 = " << add(1, 2) << std::endl;
    std::cout << "พื้นที่วงกลมรัศมี 5: " << calculate_area(5) << std::endl;
    
    // ฟังก์ชันแบบ lambda
    auto multiply = [](int a, int b) -> int { return a * b; };
    std::cout << "5 * 3 = " << multiply(5, 3) << std::endl;
    
    // ===================== คลาสและออบเจ็กต์ ======================
    std::cout << "\n----- คลาสและออบเจ็กต์ -----" << std::endl;
    
    Person person("สมชาย", 30);
    person.sayHello();
    
    person.setName("สมหญิง");
    person.setAge(25);
    std::cout << "ชื่อหลังการเปลี่ยน: " << person.getName() << std::endl;
    std::cout << "อายุหลังการเปลี่ยน: " << person.getAge() << std::endl;
    
    Student student("นักเรียน", 18, "โรงเรียนตัวอย่าง", 3.75);
    student.sayHello();
    student.study();
    
    // ===================== สมาร์ทพอยน์เตอร์ ======================
    std::cout << "\n----- สมาร์ทพอยน์เตอร์ -----" << std::endl;
    
    std::unique_ptr<Person> person_ptr = std::make_unique<Person>("คนใหม่", 40);
    person_ptr->sayHello();
    
    std::shared_ptr<Person> shared_person = std::make_shared<Person>("คนที่แชร์ได้", 35);
    {
        auto another_ref = shared_person;  // reference count เพิ่มเป็น 2
        std::cout << "Reference count: " << shared_person.use_count() << std::endl;
    }  // ตอนนี้ reference count ลดเหลือ 1
    std::cout << "Reference count after scope: " << shared_person.use_count() << std::endl;
    
    // ===================== เทมเพลต ======================
    std::cout << "\n----- เทมเพลต -----" << std::endl;
    
    std::cout << "max(5, 10): " << maximum(5, 10) << std::endl;
    std::cout << "max(3.14, 2.71): " << maximum(3.14, 2.71) << std::endl;
    std::cout << "max(\"abc\", \"xyz\"): " << maximum(std::string("abc"), std::string("xyz")) << std::endl;
    
    // ===================== การจัดการ Exception ======================
    std::cout << "\n----- การจัดการ Exception -----" << std::endl;
    
    try {
        int numerator = 10;
        int denominator = 0;
        if (denominator == 0) {
            throw std::runtime_error("การหารด้วยศูนย์");
        }
        std::cout << "ผลลัพธ์: " << numerator / denominator << std::endl;
    } catch (const std::exception& e) {
        std::cout << "เกิดข้อผิดพลาด: " << e.what() << std::endl;
    }
    
    std::cout << "\n========== จบการทดสอบไวยากรณ์ภาษา C++ ==========" << std::endl;
    return 0;
}

// นิยามฟังก์ชัน
void hello_world() {
    std::cout << "สวัสดีชาวโลก!" << std::endl;
}

int add(int a, int b) {
    return a + b;
}

double calculate_area(double radius) {
    return PI * radius * radius;
}