#include <iostream>
#include <opencv2/opencv.hpp>
#include "image_processor.h"

ImageProcessor::ImageProcessor() {
    // constructor implementation
    std::cout << "ImageProcessor Default Constructor Called!" << std::endl;
}

std::vector<float> ImageProcessor::processImage(const std::string& imagePath) {
    // Read the image
    cv::Mat image = cv::imread(imagePath, cv::IMREAD_COLOR);

    // Check if the image is empty
    if (image.empty()) {
        std::cerr << "Could not read the image: " << imagePath << std::endl;
        return std::vector<float>(); // Return an empty vector
    }

    // Convert BGR to RGB
    cv::cvtColor(image, image, cv::COLOR_BGR2RGB);

    // Resize the image to a fixed size (e.g., 150x150)
    cv::resize(image, image, cv::Size(150, 150));

    // Normalize the image
    image.convertTo(image, CV_32F, 1.0 / 255.0);

    // Convert the image to a 4D tensor shape [batch_size, height, width, channels]
    // Since batch_size is 1 in this case, we'll just convert the 3D image tensor to a vector
    std::vector<float> tensorImage;
    tensorImage.assign((float*)image.data, (float*)image.data + image.total() * image.channels());

    return tensorImage;
}
