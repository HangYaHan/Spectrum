import numpy as np


import matplotlib.pyplot as plt

# 读取文件
def load_absorption_data(file_path):
    absorption_data = []
    with open(file_path, 'r') as file:
        for line in file:
            if line.startswith("ROI"):
                values = list(map(float, line.split(":")[1].strip().split()))
                absorption_data.append(values)
    return absorption_data

# 对吸收数据进行归一化处理
def normalize_absorption_data(absorption_data):
    normalized_data = []
    for data in absorption_data:
        max_value = max(data)
        min_value = min(data)
        normalized_data.append([(value - min_value) / (max_value - min_value) for value in data])
    return normalized_data

# 计算吸收数据的相关系数矩阵并绘制图形
def calculate_correlation_matrix(absorption_data):
    # 转置数据，使每列代表一个 ROI
    data_matrix = np.array(absorption_data)
    # 计算相关系数矩阵
    correlation_matrix = np.corrcoef(data_matrix)
    
    # 绘制相关系数矩阵的热图
    plt.figure(figsize=(10, 8))
    plt.imshow(correlation_matrix, cmap="coolwarm", interpolation="nearest")
    plt.colorbar(label="Correlation Coefficient")
    
    # 添加标题和标签
    plt.title("Correlation Matrix Heatmap", fontsize=16)
    plt.xlabel("ROI Index", fontsize=14)
    plt.ylabel("ROI Index", fontsize=14)
    
    # 设置刻度
    num_rois = len(absorption_data)  # 确保横竖刻度为 ROI 的数量
    plt.xticks(range(num_rois), [f"ROI {i+1}" for i in range(num_rois)], rotation=45, fontsize=10)
    plt.yticks(range(num_rois), [f"ROI {i+1}" for i in range(num_rois)], fontsize=10)
    
    # 显示网格线
    plt.grid(False)
    
    # 保存热图
    plt.savefig("correlation_matrix_heatmap.png", dpi=300)
    
    # 显示热图
    plt.show()
    
    return correlation_matrix

# 绘制曲线
def plot_absorption_curves(absorption_data):
    # 横坐标：400 到 800，步长为 5
    wavelengths = list(range(400, 805, 5))
    
    # 设置图形大小
    plt.figure(figsize=(12, 8))
    
    # 绘制每个 ROI 的曲线
    for i, data in enumerate(absorption_data):
        plt.plot(wavelengths, data, label=f"ROI {i+1}")
    
    # 添加标题和标签
    plt.title("Trans Curves for 14 ROIs", fontsize=16)
    plt.xlabel("Wavelength (nm)", fontsize=14)
    plt.ylabel("Trans Value", fontsize=14)
    
    # 设置横坐标刻度
    plt.xticks(range(400, 805, 50), fontsize=12)
    plt.yticks(fontsize=12)
    
    # 添加图例
    plt.legend(loc="upper right", fontsize=12)
    
    # 显示网格
    plt.grid(True, linestyle="--", alpha=0.6)
    
    # 保存图像
    plt.savefig("absorption_curves.png", dpi=300)
    
    # 显示图像
    plt.show()

# 主函数
if __name__ == "__main__":
    # 文件路径
    file_path = "D:\\Code\\Spectrum\\Output\\absorption_curves.txt"
    
    # 加载数据
    absorption_data = load_absorption_data(file_path)

    # 归一化数据
    normalize_absorption_data = normalize_absorption_data(absorption_data)

    # 计算相关系数矩阵
    correlation_matrix = calculate_correlation_matrix(absorption_data)
    print("Correlation Matrix:")
    print(correlation_matrix)
    
    # 绘制曲线
    plot_absorption_curves(normalize_absorption_data)
