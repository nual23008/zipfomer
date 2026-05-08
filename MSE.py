import numpy as np

def load_data_from_txt(file_path):
    """Đọc file txt và chuyển đổi thành mảng numpy chứa số thực"""
    try:
        with open(file_path, 'r') as f:
            content = f.read().strip()
            
        # Loại bỏ các dấu ngoặc vuông nếu có
        content = content.replace('[', '').replace(']', '')
        
        # Tách chuỗi thành các phần tử (dựa trên dấu phẩy)
        str_values = content.split(',')
        
        # Chuyển đổi sang float (bỏ qua các khoảng trắng thừa)
        float_array = np.array([float(x.strip()) for x in str_values if x.strip()])
        return float_array
    except Exception as e:
        print(f"Lỗi khi đọc file {file_path}: {e}")
        return None

def calculate_mse(file1, file2):
    """Tính toán MSE giữa 2 file"""
    data1 = load_data_from_txt(file1)
    data2 = load_data_from_txt(file2)
    
    if data1 is None or data2 is None:
        return
        
    # Kiểm tra kích thước 2 mảng có khớp nhau không
    if len(data1) != len(data2):
        print(f"⚠️ Cảnh báo: Kích thước hai mảng không khớp! (File 1: {len(data1)}, File 2: {len(data2)})")
        # Ép về cùng kích thước nhỏ nhất để không bị lỗi code (tùy chọn)
        min_len = min(len(data1), len(data2))
        data1 = data1[:min_len]
        data2 = data2[:min_len]
        
    # Công thức MSE: Trung bình cộng của (Sai số bình phương)
    mse = np.mean((data1 - data2) ** 2)
    
    # Tính thêm Max Absolute Error (Sai số tuyệt đối lớn nhất) để dễ debug
    max_err = np.max(np.abs(data1 - data2))
    
    print(f"✅ Đã so sánh {len(data1)} phần tử.")
    print(f"➤ Sai số MSE (Mean Squared Error):  {mse:.8f}")
    print(f"➤ Sai số lớn nhất (Max Error):      {max_err:.8f}")
    
    return mse

# --- HƯỚNG DẪN SỬ DỤNG ---
# Thay tên file bằng đường dẫn thực tế của bạn
file_c_output = 'C:\\Users\\admin\\OneDrive\\Desktop\\Zipforner\\CODE C\\output.txt'               # File do code C in ra
file_golden_output = 'C:\\Users\\admin\\OneDrive\\Desktop\\Zipforner\\CODE C\\output_list_format.txt' # File chuẩn từ Python/Numpy

# Chạy tính toán
calculate_mse(file_c_output, file_golden_output)