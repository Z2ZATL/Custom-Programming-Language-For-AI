#!/bin/bash

# สคริปต์จัดระเบียบไฟล์ตัวอย่างของ AI Language

echo "เริ่มต้นจัดระเบียบไฟล์ตัวอย่าง..."

# สร้างโครงสร้างโฟลเดอร์ที่เหมาะสม
mkdir -p examples/{ml_examples,dl_examples,rl_examples}
mkdir -p ai_language/examples/{ml_examples,dl_examples,rl_examples}
mkdir -p ai_language/datasets

# ตรวจสอบและย้ายไฟล์ในระดับบนสุด
if [ -f "examples/example.ai" ]; then
  echo "กำลังจัดระเบียบไฟล์ examples/example.ai"
  # ตรวจสอบเนื้อหาไฟล์เพื่อกำหนดประเภท
  content=$(cat examples/example.ai)
  
  if [[ $content == *"create ML"* ]]; then
    echo "  พบคำสั่ง ML ในไฟล์ example.ai ย้ายไปยัง ml_examples/basic_example.ai"
    cp examples/example.ai examples/ml_examples/basic_example.ai
    rm examples/example.ai
  elif [[ $content == *"create DL"* ]]; then
    echo "  พบคำสั่ง DL ในไฟล์ example.ai ย้ายไปยัง dl_examples/basic_example.ai"
    cp examples/example.ai examples/dl_examples/basic_example.ai
    rm examples/example.ai
  elif [[ $content == *"create RL"* ]]; then
    echo "  พบคำสั่ง RL ในไฟล์ example.ai ย้ายไปยัง rl_examples/basic_example.ai"
    cp examples/example.ai examples/rl_examples/basic_example.ai
    rm examples/example.ai
  else
    echo "  ไม่สามารถระบุประเภทของไฟล์ example.ai ย้ายไปยัง ml_examples/general_example.ai"
    cp examples/example.ai examples/ml_examples/general_example.ai
    rm examples/example.ai
  fi
fi

# จัดระเบียบไฟล์ในโฟลเดอร์ ai_language
if [ -d "ai_language/examples" ]; then
  # ตรวจสอบไฟล์ ml_example.ai
  if [ -f "ai_language/examples/ml_example.ai" ]; then
    echo "ย้าย ai_language/examples/ml_example.ai ไปยัง ai_language/examples/ml_examples/"
    cp ai_language/examples/ml_example.ai ai_language/examples/ml_examples/basic_ml_example.ai
    rm ai_language/examples/ml_example.ai
  fi
  
  # ตรวจสอบไฟล์ dl_example.ai
  if [ -f "ai_language/examples/dl_example.ai" ]; then
    echo "ย้าย ai_language/examples/dl_example.ai ไปยัง ai_language/examples/dl_examples/"
    cp ai_language/examples/dl_example.ai ai_language/examples/dl_examples/basic_dl_example.ai
    rm ai_language/examples/dl_example.ai
  fi
  
  # ตรวจสอบไฟล์ rl_example.ai
  if [ -f "ai_language/examples/rl_example.ai" ]; then
    echo "ย้าย ai_language/examples/rl_example.ai ไปยัง ai_language/examples/rl_examples/"
    cp ai_language/examples/rl_example.ai ai_language/examples/rl_examples/basic_rl_example.ai
    rm ai_language/examples/rl_example.ai
  fi
  
  # ตรวจสอบไฟล์ example.ai
  if [ -f "ai_language/examples/example.ai" ]; then
    echo "ย้าย ai_language/examples/example.ai ไปยังโฟลเดอร์ที่เหมาะสม"
    content=$(cat ai_language/examples/example.ai)
    
    if [[ $content == *"create ML"* ]]; then
      echo "  พบคำสั่ง ML ในไฟล์ example.ai ย้ายไปยัง ml_examples/basic_example.ai"
      cp ai_language/examples/example.ai ai_language/examples/ml_examples/basic_example.ai
      rm ai_language/examples/example.ai
    elif [[ $content == *"create DL"* ]]; then
      echo "  พบคำสั่ง DL ในไฟล์ example.ai ย้ายไปยัง dl_examples/basic_example.ai"
      cp ai_language/examples/example.ai ai_language/examples/dl_examples/basic_example.ai
      rm ai_language/examples/example.ai
    elif [[ $content == *"create RL"* ]]; then
      echo "  พบคำสั่ง RL ในไฟล์ example.ai ย้ายไปยัง rl_examples/basic_example.ai"
      cp ai_language/examples/example.ai ai_language/examples/rl_examples/basic_example.ai
      rm ai_language/examples/example.ai
    else
      echo "  ไม่สามารถระบุประเภทของไฟล์ example.ai ย้ายไปยัง ml_examples/general_example.ai"
      cp ai_language/examples/example.ai ai_language/examples/ml_examples/general_example.ai
      rm ai_language/examples/example.ai
    fi
  fi
fi

# ตรวจสอบโฟลเดอร์ examples ในระดับบนสุด
if [ -d "examples" ]; then
  # สร้างโฟลเดอร์ในโครงสร้าง ai_language ถ้ายังไม่มี
  if [ ! -d "ai_language/examples" ]; then
    mkdir -p ai_language/examples/{ml_examples,dl_examples,rl_examples}
  fi
  
  # คัดลอกไฟล์ตัวอย่างทั้งหมดไปยังโฟลเดอร์ที่เหมาะสมใน ai_language
  echo "กำลังคัดลอกไฟล์จาก examples/ ไปยัง ai_language/examples/"
  
  if [ -d "examples/ml_examples" ]; then
    cp -r examples/ml_examples/* ai_language/examples/ml_examples/ 2>/dev/null || true
  fi
  
  if [ -d "examples/dl_examples" ]; then
    cp -r examples/dl_examples/* ai_language/examples/dl_examples/ 2>/dev/null || true
  fi
  
  if [ -d "examples/rl_examples" ]; then
    cp -r examples/rl_examples/* ai_language/examples/rl_examples/ 2>/dev/null || true
  fi
fi

# แสดงโครงสร้างไฟล์หลังจากการจัดเรียง
echo ""
echo "โครงสร้างไฟล์หลังจากการจัดเรียง:"
echo "ai_language/examples/"
ls -la ai_language/examples/ 2>/dev/null || echo "  (ไม่พบไฟล์)"
echo "ai_language/examples/ml_examples/"
ls -la ai_language/examples/ml_examples/ 2>/dev/null || echo "  (ไม่พบไฟล์)"
echo "ai_language/examples/dl_examples/"
ls -la ai_language/examples/dl_examples/ 2>/dev/null || echo "  (ไม่พบไฟล์)"
echo "ai_language/examples/rl_examples/"
ls -la ai_language/examples/rl_examples/ 2>/dev/null || echo "  (ไม่พบไฟล์)"

echo ""
echo "การจัดระเบียบไฟล์เสร็จสิ้น"
