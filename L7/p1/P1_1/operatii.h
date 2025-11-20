#pragma once

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

unsigned char* negateImage(unsigned char* img, int w, int h);
unsigned char* binaryImage(unsigned char* img, int w, int h);

cv::Mat drawHistogram(const cv::Mat gray);

class ImageGrid {
public:
    std::vector<std::pair<std::string, cv::Mat>> images;

    void add(const std::string& name, const cv::Mat& img) {
        images.push_back({ name, img });
    }

    void show(const std::string& windowName) {
        if (images.empty()) return;

        int count = images.size();
        int rows = std::ceil(std::sqrt(count));
        int cols = rows;

        int img_h = 250; // dimensiunea fiecărei imagini în grid
        int img_w = 250;
       

        cv::Mat grid = cv::Mat::zeros(rows * img_h, cols * img_w, CV_8UC3);

        int idx = 0;
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                if (idx >= count) break;

                cv::Mat resized;
                cv::Mat input = images[idx].second;

                if (input.channels() == 1)
                    cv::cvtColor(input, resized, cv::COLOR_GRAY2BGR);
                else
                    resized = input.clone();

                cv::resize(resized, resized, cv::Size(img_w, img_h));

                resized.copyTo(grid(cv::Rect(c * img_w, r * img_h, img_w, img_h)));

                cv::putText(grid, images[idx].first,
                    cv::Point(c * img_w + 10, r * img_h + 20),
                    cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0), 1);
                idx++;
            }
        }

        cv::imshow(windowName, grid);
    }
};


unsigned char* sobelImage(unsigned char* img, int w, int h);
unsigned char* motionBlur(unsigned char* img, int w, int h);

