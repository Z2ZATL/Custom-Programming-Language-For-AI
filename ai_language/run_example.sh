#!/bin/bash

# สคริปต์สำหรับรันตัวอย่างของ AI Language

if [ $# -eq 0 ]; then
    echo "โปรดระบุประเภทของตัวอย่างที่ต้องการรัน (ml, dl, rl)"
    echo "ตัวอย่าง: ./run_example.sh ml"
    exit 1
fi

# ตรวจสอบว่า ai_lang มีอยู่หรือไม่
if [ ! -f "./ai_lang" ]; then
    echo "ไม่พบไฟล์ ai_lang กำลังคอมไพล์..."
    
    # ตรวจสอบโฟลเดอร์ build
    if [ ! -d "./build" ]; then
        mkdir build
    fi
    
    cd build
    cmake ..
    make
    cp ai_lang ..
    cd ..
    
    if [ ! -f "./ai_lang" ]; then
        echo "การคอมไพล์ล้มเหลว กรุณาตรวจสอบข้อผิดพลาด"
        exit 1
    fi
    
    echo "คอมไพล์เสร็จสิ้น"
fi

# ตรวจสอบประเภทของตัวอย่าง
type=$1
case $type in
    ml|ML)
        echo "กำลังรันตัวอย่าง Machine Learning..."
        ./ai_lang examples/ml_example.ai
        ;;
    dl|DL)
        echo "กำลังรันตัวอย่าง Deep Learning..."
        ./ai_lang examples/dl_example.ai
        ;;
    rl|RL)
        echo "กำลังรันตัวอย่าง Reinforcement Learning..."
        ./ai_lang examples/rl_example.ai
        ;;
    *)
        echo "ไม่รู้จักประเภท: $type"
        echo "โปรดระบุ ml, dl, หรือ rl"
        exit 1
        ;;
esac

exit 0