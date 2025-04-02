
/**
 * @file BaseConnector.h
 * @brief คลาสพื้นฐานสำหรับเชื่อมต่อกับไลบรารีภายนอกสำหรับการเรียนรู้ของเครื่อง
 */

#ifndef AI_LANGUAGE_BASECONNECTOR_H
#define AI_LANGUAGE_BASECONNECTOR_H

namespace ai_language {

/**
 * @class BaseConnector
 * @brief คลาสพื้นฐานสำหรับเชื่อมต่อกับไลบรารีภายนอก
 */
class BaseConnector {
public:
    /**
     * @brief ดีสตรัคเตอร์เสมือนสำหรับการสืบทอด
     */
    virtual ~BaseConnector() {}
    
    /**
     * @brief ฟังก์ชันเสมือนสำหรับการเชื่อมต่อกับไลบรารี
     */
    virtual void connect() = 0;
};

} // namespace ai_language

#endif // AI_LANGUAGE_BASECONNECTOR_H
