
/**
 * @file token_types.h
 * @brief ประเภทของ Token ทั้งหมดสำหรับ Lexer
 */

#ifndef AI_LANGUAGE_TOKEN_TYPES_H
#define AI_LANGUAGE_TOKEN_TYPES_H

namespace ai_language {

enum class TokenType {
    // Token พื้นฐาน
    IDENTIFIER,   // ตัวระบุชื่อต่างๆ
    STRING,       // ข้อความในเครื่องหมายคำพูด
    NUMBER,       // ตัวเลข
    
    // Token สำหรับการควบคุม
    NEWLINE,      // การขึ้นบรรทัดใหม่
    END,          // จุดสิ้นสุดของไฟล์
    
    // คำสั่งหลัก
    START,        // คำสั่ง start
    END_KEYWORD,  // คำสั่ง end
    CREATE,       // คำสั่ง create
    LOAD,         // คำสั่ง load
    SET,          // คำสั่ง set
    TRAIN,        // คำสั่ง train
    EVALUATE,     // คำสั่ง evaluate
    SHOW,         // คำสั่ง show
    SAVE,         // คำสั่ง save
    ADD,          // คำสั่ง add
    PREDICT,      // คำสั่ง predict
    
    // คำสำคัญสำหรับ Dataset
    DATASET,      // dataset
    CLEAN,        // clean
    SPLIT,        // split
    TYPE,         // type
    
    // คำสำคัญสำหรับโปรเจค
    ML_TYPE,      // ML
    DL_TYPE,      // DL
    RL_TYPE,      // RL
    
    // คำสำคัญสำหรับโมเดล
    MODEL,        // model
    LAYER,        // layer
    
    // คำสำคัญสำหรับพารามิเตอร์
    LEARNING_RATE,     // learning_rate
    BATCH_SIZE,        // batch_size
    EPOCHS,            // epochs
    DISCOUNT_FACTOR,   // discount_factor
    TIMEZONE,          // timezone
    
    // คำสำคัญสำหรับสภาพแวดล้อม
    ENVIRONMENT,       // environment
    CONFIG,            // config
    
    // คำสำคัญสำหรับการแสดงผล
    ACCURACY,          // accuracy
    LOSS,              // loss
    PERFORMANCE,       // performance
    GRAPH,             // graph
    VISUALIZE,         // visualize
    PLOT,              // plot
    
    // คำสำคัญสำหรับการแบ่งข้อมูล
    WITH,              // with
    INTO,              // into
    RATIO,             // ratio
    
    // คำสำคัญอื่นๆ
    PATH,              // path
    METRIC,            // metric
    DATA,              // data
};

} // namespace ai_language

#endif // AI_LANGUAGE_TOKEN_TYPES_H
