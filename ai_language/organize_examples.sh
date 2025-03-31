
#!/bin/bash

# สคริปต์จัดระเบียบไฟล์ตัวอย่างของ AI Language

echo "เริ่มต้นจัดระเบียบไฟล์ตัวอย่าง..."

# สร้างโครงสร้างโฟลเดอร์ที่เหมาะสม
mkdir -p examples/{ml_examples,dl_examples,rl_examples}

# ย้ายไฟล์ตัวอย่างจากระดับบนสุดไปยังโฟลเดอร์ที่เหมาะสม
if [ -f "example.ai" ]; then
  echo "ย้าย example.ai ไปยัง examples/"
  mv example.ai examples/
fi

# จัดระเบียบไฟล์ตัวอย่างในโฟลเดอร์ examples
cd ai_language

# ตรวจสอบว่ามีไฟล์เก่าและใหม่ที่ซ้ำกัน
echo "ตรวจสอบไฟล์ซ้ำ..."

# รายชื่อไฟล์ที่เป็นแบบเก่า (ที่จะถูกลบ)
declare -a old_files=(
  "examples/dl_example.ai"  # เก่า - เราจะใช้ไฟล์ในโฟลเดอร์ dl_examples แทน
)

# แสดงรายการไฟล์ที่จะลบ
echo "ไฟล์ที่จะถูกลบ (เวอร์ชันเก่า):"
for file in "${old_files[@]}"; do
  if [ -f "$file" ]; then
    echo "- $file"
  fi
done

# ถามผู้ใช้ว่าต้องการลบไฟล์หรือไม่
echo ""
echo "ไฟล์เหล่านี้เป็นเวอร์ชันเก่าและอาจซ้ำซ้อนกับไฟล์ในโฟลเดอร์ย่อย"
echo "คุณต้องการลบไฟล์เหล่านี้หรือไม่? (y/n)"
read -p "> " answer

if [ "$answer" = "y" ]; then
  # ลบไฟล์เก่า
  for file in "${old_files[@]}"; do
    if [ -f "$file" ]; then
      rm "$file"
      echo "ลบไฟล์: $file"
    fi
  done
  echo "ลบไฟล์เก่าเรียบร้อยแล้ว"
else
  echo "ไม่มีการลบไฟล์"
fi

echo "จัดระเบียบไฟล์ตัวอย่างเสร็จสิ้น"
