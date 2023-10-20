#ifndef NEURAL_NETWORK_H
#define NEURAL_NETWORK_H

#include <tensorflow/c/c_api.h> // TensorFlow C API header
#include <vector>
#include <memory> // Include for std::unique_ptr

class NeuralNetwork {
public:
    NeuralNetwork();
    NeuralNetwork(const char* model_path);

    // Deleted copy constructor and copy assignment operator
    NeuralNetwork(const NeuralNetwork& other) = delete;
    NeuralNetwork& operator=(const NeuralNetwork& other) = delete;

    // Move constructor and move assignment operator
    NeuralNetwork(NeuralNetwork&& other) noexcept;
    NeuralNetwork& operator=(NeuralNetwork&& other) noexcept;
    ~NeuralNetwork(); // Destructor for cleanup
    std::vector<float> predict(const std::vector<float>& inputs);
private:
    TF_Graph* graph;
    TF_Status* status;
    TF_Session* session;
    TF_Tensor* loss;
    TF_SessionOptions* options;
    // Add other members as needed
};

#endif // NEURAL_NETWORK_H

