
#!/usr/bin/env python3
import matplotlib.pyplot as plt
import pandas as pd
import os
import sys

def generate_learning_curves(csv_path, output_dir):
    """
    สร้างกราฟการเรียนรู้จากไฟล์ CSV และบันทึกเป็น PNG
    
    Args:
        csv_path: พาธของไฟล์ CSV ที่มีข้อมูล
        output_dir: โฟลเดอร์ที่จะบันทึกไฟล์ PNG
    """
    try:
        # ตรวจสอบว่าไฟล์ CSV มีอยู่จริง
        if not os.path.exists(csv_path):
            print(f"Error: CSV file not found: {csv_path}")
            return False
            
        # อ่านข้อมูลจากไฟล์ CSV
        data = pd.read_csv(csv_path)
        
        # ตรวจสอบว่ามีคอลัมน์ที่จำเป็น
        required_columns = ['Epoch', 'Loss', 'Accuracy']
        for col in required_columns:
            if col not in data.columns:
                print(f"Error: Required column '{col}' not found in CSV")
                return False
        
        # สร้างกราฟ
        plt.figure(figsize=(10, 6))
        
        # กราฟแสดง Loss
        plt.subplot(2, 1, 1)
        plt.plot(data['Epoch'], data['Loss'], 'r-', label='Loss')
        plt.title('Learning Curves')
        plt.ylabel('Loss')
        plt.grid(True)
        plt.legend()
        
        # กราฟแสดง Accuracy
        plt.subplot(2, 1, 2)
        plt.plot(data['Epoch'], data['Accuracy'], 'b-', label='Accuracy')
        plt.xlabel('Epoch')
        plt.ylabel('Accuracy')
        plt.grid(True)
        plt.legend()
        
        # บันทึกกราฟเป็นไฟล์ PNG
        plt.tight_layout()
        plt.savefig(output_png_path)
        print(f"Plot saved as {output_png_path}")
        
        return True
    except Exception as e:
        print(f"Error generating plot: {e}")
        return False

if __name__ == "__main__":
    # เมื่อเรียกใช้งานโดยตรง สามารถส่งพารามิเตอร์ได้
    if len(sys.argv) != 3:
        print("Usage: python plot_generator.py <csv_file_path> <output_directory>")
        sys.exit(1)
        
    csv_path = sys.argv[1]
    output_dir = sys.argv[2]
    
    success = generate_learning_curves(csv_path, output_dir)
    sys.exit(0 if success else 1)
#!/usr/bin/env python3
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
        
        # สร้างโฟลเดอร์สำหรับเก็บไฟล์ PNG ถ้ายังไม่มี
        output_dir = os.path.dirname(output_png_path)
        if not os.path.exists(output_dir):
            os.makedirs(output_dir)
            
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
        
        # บันทึกกราฟเป็นไฟล์ PNG
        plt.savefig(output_path)
        print(f"Graph saved successfully to {output_path}")
        return 0
    
    except Exception as e:
        print(f"Error: Failed to generate plot: {str(e)}")
        return 1

if __name__ == "__main__":
    sys.exit(main())
