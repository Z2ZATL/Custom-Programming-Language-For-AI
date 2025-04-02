#!/usr/bin/env python3
import sys
import os
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib
matplotlib.use('Agg')  # Use non-interactive backend

def main():
    if len(sys.argv) < 3:
        print("Error: Not enough arguments")
        print("Usage: python plot_generator.py <csv_data_path> <output_png_path> [<title>]")
        return 1

    csv_path = sys.argv[1]
    output_png_path = sys.argv[2]
    title = sys.argv[3] if len(sys.argv) > 3 else "Learning Curves"

    try:
        # ตรวจสอบว่าไฟล์ CSV มีอยู่จริง
        if not os.path.exists(csv_path):
            print(f"Error: CSV file does not exist: {csv_path}")
            return 1

        # อ่านข้อมูล CSV
        df = pd.read_csv(csv_path)

        # สร้างกราฟ
        plt.figure(figsize=(10, 6))

        # ตรวจสอบคอลัมน์ที่มีในข้อมูล
        if 'epoch' in df.columns and 'accuracy' in df.columns:
            plt.plot(df['epoch'], df['accuracy'], 'b-', label='Accuracy')
        if 'epoch' in df.columns and 'loss' in df.columns:
            plt.plot(df['epoch'], df['loss'], 'r-', label='Loss')

        plt.title(title)
        plt.xlabel('Epochs')
        plt.ylabel('Value')
        plt.legend()
        plt.grid(True)

        # บันทึกกราฟเป็นไฟล์ PNG - บันทึกไปยังตำแหน่งที่ถูกต้อง
        output_file = f"{output_png_path}/learning_curves.png"
        plt.savefig(output_file)
        print(f"Graph saved successfully to {output_png_path}")"Graph saved successfully to {output_png_path}")
        return 0

    except Exception as e:
        print(f"Error: Failed to generate plot: {str(e)}")
        return 1

if __name__ == "__main__":
    sys.exit(main())