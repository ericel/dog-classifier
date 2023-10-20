// ImageProcessor.h
#ifndef IMAGE_PROCESSOR_H
#define IMAGE_PROCESSOR_H

#include <opencv2/opencv.hpp>
#include <string>

class ImageProcessor {
public:
    ImageProcessor();  // Declare constructor
    std::vector<float> processImage(const std::string& imagePath);
};

#endif // IMAGE_PROCESSOR_H