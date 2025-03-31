#!/bin/bash

# สคริปต์สำหรับบันทึก output ของโปรแกรม AI Language ในโหมดโต้ตอบ

# ตรวจสอบและสร้างโฟลเดอร์ถ้าไม่มี
if [ ! -d "Program test" ]; then
    mkdir -p "Program test"
fi

# สร้างไฟล์ output
OUTPUT_FILE="Program test/interaction_output_$(date +%Y%m%d_%H%M%S).txt"

echo "บันทึก output ไปที่: $OUTPUT_FILE"
echo "===== เริ่มต้นการบันทึก $(date) =====" > "$OUTPUT_FILE"

# รันโปรแกรมและบันทึก output
cd ai_language && ./ai_lang -i | tee -a "$OUTPUT_FILE"

echo "===== สิ้นสุดการบันทึก $(date) =====" >> "$OUTPUT_FILE"
echo "บันทึกเสร็จสิ้น"

# แสดงคำแนะนำเกี่ยวกับนามสกุลไฟล์ที่ควรใช้กับแต่ละประเภทโมเดล
echo ""
echo "คำแนะนำ: กรุณาใช้นามสกุลไฟล์ต่อไปนี้เมื่อบันทึกโมเดล"
echo "  - Machine Learning (ML): .mlmodel"
echo "  - Deep Learning (DL): .dlmodel"
echo "  - Reinforcement Learning (RL): .rlmodel"
echo "  - Natural Language Processing (NLP): .nlpmodel"
echo "  - Computer Vision (CV): .cvmodel"
echo "ดูรายละเอียดเพิ่มเติมได้ที่ README.md ในโฟลเดอร์ Program test"
