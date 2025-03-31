#!/bin/bash

# สคริปต์สำหรับรันตัวอย่างของ AI Language

if [ $# -lt 1 ]; then
    echo "โปรดระบุประเภทของตัวอย่างที่ต้องการรัน (ml, dl, rl) และชื่อไฟล์ (ไม่ต้องมีนามสกุล .ai)"
    echo "ตัวอย่าง: ./run_example.sh ml linear_regression"
    echo "หรือ: ./run_example.sh dl neural_network"
    
    echo ""
    echo "ตัวอย่างที่มีอยู่:"
    echo "ML Examples:"
    ls -1 examples/ml_examples/*.ai 2>/dev/null | sed 's/.*\//  - /' | sed 's/\.ai$//'
    echo "DL Examples:"
    ls -1 examples/dl_examples/*.ai 2>/dev/null | sed 's/.*\//  - /' | sed 's/\.ai$//'
    echo "RL Examples:"
    ls -1 examples/rl_examples/*.ai 2>/dev/null | sed 's/.*\//  - /' | sed 's/\.ai$//'
    
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
example_name=$2

case $type in
    ml|ML)
        example_folder="ml_examples"
        if [ -z "$example_name" ]; then
            echo "รายการตัวอย่าง ML:"
            ls -1 examples/ml_examples/*.ai 2>/dev/null | sed 's/.*\//  - /' | sed 's/\.ai$//'
            exit 1
        fi
        ;;
    dl|DL)
        example_folder="dl_examples"
        if [ -z "$example_name" ]; then
            echo "รายการตัวอย่าง DL:"
            ls -1 examples/dl_examples/*.ai 2>/dev/null | sed 's/.*\//  - /' | sed 's/\.ai$//'
            exit 1
        fi
        ;;
    rl|RL)
        example_folder="rl_examples"
        if [ -z "$example_name" ]; then
            echo "รายการตัวอย่าง RL:"
            ls -1 examples/rl_examples/*.ai 2>/dev/null | sed 's/.*\//  - /' | sed 's/\.ai$//'
            exit 1
        fi
        ;;
    *)
        echo "ประเภทของตัวอย่างไม่ถูกต้อง กรุณาใช้ ml, dl, หรือ rl"
        exit 1
        ;;
esac

# ตรวจสอบไฟล์ตัวอย่าง
example_file="examples/${example_folder}/${example_name}.ai"
if [ ! -f "$example_file" ]; then
    echo "ไม่พบไฟล์ตัวอย่าง: $example_file"
    
    # ตรวจสอบชื่อไฟล์ที่ใกล้เคียง
    echo "กำลังค้นหาไฟล์ที่ใกล้เคียง..."
    found_files=$(find "examples/${example_folder}" -name "*${example_name}*.ai" 2>/dev/null)
    
    if [ -n "$found_files" ]; then
        echo "พบไฟล์ที่คล้ายกัน:"
        echo "$found_files" | sed 's/.*\//  - /'
    else
        echo "ไม่พบไฟล์ที่คล้ายกัน"
        echo "ตัวอย่างที่มีอยู่ในประเภท $type:"
        ls -1 "examples/${example_folder}/"*.ai 2>/dev/null | sed 's/.*\//  - /' | sed 's/\.ai$//'
    fi
    
    exit 1
fi

# รันตัวอย่าง
echo "กำลังรันตัวอย่าง: $example_file"
./ai_lang "$example_file"
