#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <fstream>
#include <numeric>

namespace fs = std::filesystem;

// 计算 ROI 的平均灰度值
double calculateMeanGray(const cv::Mat& image, const cv::Rect& roi) {
    cv::Mat roiImage = image(roi);
    return cv::mean(roiImage)[0]; // 灰度图的平均值
}

int main() {
    std::string filterPath = "D:\\Code\\Spectrum\\Input\\Pictures\\filter\\";
    std::string backPath = "D:\\Code\\Spectrum\\Input\\Pictures\\back\\";
    std::vector<cv::Rect> rois; // 存储 14 个 ROI
    std::vector<std::string> wavelengths;

    // 手动选择 ROI
    cv::Mat referenceImage = cv::imread(filterPath + "745.bmp", cv::IMREAD_GRAYSCALE);
    if (referenceImage.empty()) {
        std::cerr << "Failed to load reference image!" << std::endl;
        return -1;
    }

    // 放大图像中间的区域，只展示那一块
    const std::string windowName = "Select ROI";
    cv::namedWindow(windowName, cv::WINDOW_NORMAL);

    // 定义放大倍数和放大区域的大小
    double zoomFactor = 15.0; // 放大倍数
    int zoomWidth = static_cast<int>(referenceImage.cols / zoomFactor);
    int zoomHeight = static_cast<int>(referenceImage.rows / zoomFactor);
    int startX = (referenceImage.cols - zoomWidth) / 2;
    int startY = (referenceImage.rows - zoomHeight) / 2;

    cv::Rect zoomRegion(startX, startY, zoomWidth, zoomHeight);
    cv::Mat zoomedImage = referenceImage(zoomRegion);

    cv::resizeWindow(windowName, 800, 600); // 调整窗口大小
    for (int i = 0; i < 14; ++i) {
        std::cout << "Select ROI " << i + 1 << std::endl;
        cv::Rect roi = cv::selectROI(windowName, zoomedImage);
        if (roi.width == 0 || roi.height == 0) {
            std::cerr << "Invalid ROI selected!" << std::endl;
            return -1;
        }
        // 调整 ROI 的坐标到原始图像的坐标系
        roi.x += startX;
        roi.y += startY;
        rois.push_back(roi);
    }
    cv::destroyAllWindows();

    // 遍历波长文件
    for (const auto& entry : fs::directory_iterator(filterPath)) {
        std::string filename = entry.path().filename().string();
        if (filename.find(".bmp") != std::string::npos) {
            wavelengths.push_back(filename);
        }
    }
    std::sort(wavelengths.begin(), wavelengths.end()); // 按文件名排序

    // 计算每张图片的平均灰度值
    std::vector<std::vector<double>> filterGrayValues(wavelengths.size(), std::vector<double>(14));
    std::vector<std::vector<double>> backGrayValues(wavelengths.size(), std::vector<double>(14));

    for (size_t i = 0; i < wavelengths.size(); ++i) {
        cv::Mat filterImage = cv::imread(filterPath + wavelengths[i], cv::IMREAD_GRAYSCALE);
        cv::Mat backImage = cv::imread(backPath + wavelengths[i], cv::IMREAD_GRAYSCALE);

        if (filterImage.empty() || backImage.empty()) {
            std::cerr << "Failed to load image: " << wavelengths[i] << std::endl;
            return -1;
        }

        for (size_t j = 0; j < rois.size(); ++j) {
            filterGrayValues[i][j] = calculateMeanGray(filterImage, rois[j]);
            backGrayValues[i][j] = calculateMeanGray(backImage, rois[j]);
        }
    }

    // 计算吸收曲线
    std::vector<std::vector<double>> absorptionCurves(wavelengths.size(), std::vector<double>(14));
    for (size_t i = 0; i < wavelengths.size(); ++i) {
        for (size_t j = 0; j < rois.size(); ++j) {
            absorptionCurves[i][j] = filterGrayValues[i][j] / backGrayValues[i][j];
        }
    }

    // 计算14条吸收曲线的相关系数矩阵
    std::vector<std::vector<double>> correlationMatrix(14, std::vector<double>(14, 0.0));
    for (size_t i = 0; i < rois.size(); ++i) {
        for (size_t j = 0; j < rois.size(); ++j) {
            double meanI = std::accumulate(absorptionCurves.begin(), absorptionCurves.end(), 0.0,
                                           [i](double sum, const std::vector<double>& curve) {
                                               return sum + curve[i];
                                           }) / wavelengths.size();
            double meanJ = std::accumulate(absorptionCurves.begin(), absorptionCurves.end(), 0.0,
                                           [j](double sum, const std::vector<double>& curve) {
                                               return sum + curve[j];
                                           }) / wavelengths.size();

            double numerator = 0.0;
            double denominatorI = 0.0;
            double denominatorJ = 0.0;

            for (size_t k = 0; k < wavelengths.size(); ++k) {
                numerator += (absorptionCurves[k][i] - meanI) * (absorptionCurves[k][j] - meanJ);
                denominatorI += (absorptionCurves[k][i] - meanI) * (absorptionCurves[k][i] - meanI);
                denominatorJ += (absorptionCurves[k][j] - meanJ) * (absorptionCurves[k][j] - meanJ);
            }

            correlationMatrix[i][j] = (denominatorI != 0.0 && denominatorJ != 0.0)
                                          ? (numerator / std::sqrt(denominatorI * denominatorJ))
                                          : 0.0;
        }
    }

    // 保存相关系数矩阵到文件
    std::ofstream correlationFile("D:\\Code\\Spectrum\\Output\\correlation_matrix.txt");
    if (!correlationFile) {
        std::cerr << "Failed to open correlation matrix output file!" << std::endl;
        return -1;
    }
    for (size_t i = 0; i < rois.size(); ++i) {
        for (size_t j = 0; j < rois.size(); ++j) {
            correlationFile << correlationMatrix[i][j] << " ";
        }
        correlationFile << std::endl;
    }
    correlationFile.close();

    

    // // 输出吸收曲线
    // for (size_t j = 0; j < rois.size(); ++j) {
    //     std::cout << "Absorption Curve for ROI " << j + 1 << ": ";
    //     for (size_t i = 0; i < wavelengths.size(); ++i) {
    //         std::cout << absorptionCurves[i][j] << " ";
    //     }
    //     std::cout << std::endl;
    // }

    // 保存吸收曲线到文件
    std::ofstream outFile("D:\\Code\\Spectrum\\Output\\absorption_curves.txt");
    if (!outFile) {
        std::cerr << "Failed to open output file!" << std::endl;
        return -1;
    }
    for (size_t j = 0; j < rois.size(); ++j) {
        outFile << "ROI " << j + 1 << ": ";
        for (size_t i = 0; i < wavelengths.size(); ++i) {
            outFile << absorptionCurves[i][j] << " ";
        }
        outFile << std::endl;
    }
    outFile.close();

    return 0;
}