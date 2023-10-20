#include "neural_network.h"
#include <iostream>
#include <vector>

NeuralNetwork::NeuralNetwork()
    : graph(nullptr), status(nullptr), session(nullptr), loss(nullptr), options(nullptr)
{
    std::cout << "Neural Network Default Constructor" << std::endl;
}

NeuralNetwork::NeuralNetwork(const char *model_path)
    : graph(nullptr), status(nullptr), session(nullptr), loss(nullptr), options(nullptr)
{
    std::cout << "Neural Network Constructor" << std::endl;

    // Use smart pointers for resource management
    std::unique_ptr<TF_Graph, decltype(&TF_DeleteGraph)> graphPtr(TF_NewGraph(), TF_DeleteGraph);
    std::unique_ptr<TF_Status, decltype(&TF_DeleteStatus)> statusPtr(TF_NewStatus(), TF_DeleteStatus);
    std::unique_ptr<TF_SessionOptions, decltype(&TF_DeleteSessionOptions)> optionsPtr(TF_NewSessionOptions(), TF_DeleteSessionOptions);

    TF_Buffer *run_opts = nullptr;
    const char *tags[] = {"serve"};
    int ntags = 1;

    // Create a lambda function for the custom deleter
    auto customDeleter = [this](TF_Session *s)
    {
        if (s)
        {
            TF_CloseSession(s, this->status);
            TF_DeleteSession(s, this->status);
        }
    };

    // Use the custom deleter with unique_ptr
    std::unique_ptr<TF_Session, decltype(customDeleter)> sessionPtr(
        TF_LoadSessionFromSavedModel(optionsPtr.get(), run_opts, model_path, tags, ntags, graphPtr.get(), nullptr, statusPtr.get()),
        customDeleter);

    if (TF_GetCode(statusPtr.get()) != TF_OK)
    {
        std::cerr << "Error loading model: " << TF_Message(statusPtr.get()) << std::endl;
        exit(1);
    }

    // Transfer ownership to member variables
    graph = graphPtr.release();
    status = statusPtr.release();
    session = sessionPtr.release();
    options = optionsPtr.release();
}

// Custom copy constructor

// Custom move constructor
NeuralNetwork::NeuralNetwork(NeuralNetwork &&other) noexcept
    : graph(other.graph), status(other.status), session(other.session), loss(other.loss), options(other.options)
{
    std::cout << "Neural Network Move Constructor" << std::endl;

    // Reset the source object's pointers to nullptr
    other.graph = nullptr;
    other.status = nullptr;
    other.session = nullptr;
    other.loss = nullptr;
    other.options = nullptr;
}
NeuralNetwork &NeuralNetwork::operator=(NeuralNetwork &&other) noexcept
{
    // Check for self-assignment
    if (this == &other)
    {
        return *this;
    }

    // Release current resources
    TF_DeleteGraph(graph);
    TF_DeleteStatus(status);
    if (session)
    {
        TF_CloseSession(session, status);
        TF_DeleteSession(session, status);
    }
    TF_DeleteTensor(loss);
    TF_DeleteSessionOptions(options);

    // Transfer ownership of TensorFlow resources from the source object
    graph = other.graph;
    status = other.status;
    session = other.session;
    loss = other.loss;
    options = other.options;

    // Reset the source object's pointers to nullptr
    other.graph = nullptr;
    other.status = nullptr;
    other.session = nullptr;
    other.loss = nullptr;
    other.options = nullptr;

    return *this;
}

NeuralNetwork::~NeuralNetwork()
{
    std::cout << "Neural Network Destructor Called" << std::endl;
    // Use smart pointers for resource management
    std::unique_ptr<TF_Tensor, decltype(&TF_DeleteTensor)> lossPtr(loss, TF_DeleteTensor);

    TF_DeleteGraph(graph);
    TF_DeleteStatus(status);
    if (session)
    {
        TF_CloseSession(session, status);
        TF_DeleteSession(session, status);
    }
}

// The predict function to infer results from the model
std::vector<float> NeuralNetwork::predict(const std::vector<float> &inputs)
{

    // Validate the size of the inputs vector
    if (inputs.size() != 150 * 150 * 3) {
        std::cerr << "Error: The size of the inputs vector is not correct." << std::endl;
        std::cerr << "Expected size: " << 150 * 150 * 3 << ", but got: " << inputs.size() << std::endl;
        return std::vector<float>(); // Return an empty vector to indicate an error
    }

    // Quick check our graph is functioning
    if (TF_GetCode(status) != TF_OK) {
        std::string errorMessage = "ERROR: Unable to import graph " + std::string(TF_Message(status));
        std::cout << errorMessage << std::endl;
        throw std::runtime_error(errorMessage);
    } else {
        std::cout << "Successfully imported graph " << TF_Message(status) << std::endl;
    }

    // Step 1: Create Tensors for inputs
    int64_t dims[] = {1, 150, 150, 3};

    // Use unique_ptr to manage the Tensor resources
    std::unique_ptr<TF_Tensor, decltype(&TF_DeleteTensor)> input_tensor(
        TF_AllocateTensor(TF_FLOAT, dims, 4, inputs.size() * sizeof(float)),
        TF_DeleteTensor);

    memcpy(TF_TensorData(input_tensor.get()), inputs.data(), TF_TensorByteSize(input_tensor.get()));

    // Step 2: Setup running the session
    std::vector<TF_Output> input_ops = {{TF_GraphOperationByName(graph, "serving_default_conv2d_6_input"), 0}};
    /* TF_Operation* op = TF_GraphOperationByName(graph, "serving_default_conv2d_6_input");//serving_default_conv2d_12_input
    if (!ops) {
        throw std::runtime_error("Failed to get operation by name.");
    } */
    if (input_ops.empty()) {
        throw std::runtime_error("Failed to get operation by name.");
    }


    std::vector<std::unique_ptr<TF_Tensor, decltype(&TF_DeleteTensor)>> input_values; 
    input_values.push_back(std::move(input_tensor));
   
    std::vector<TF_Tensor *> input_values_raw;
    for (auto &unique_tensor : input_values)
    {
        input_values_raw.push_back(unique_tensor.get());
    }

   

    std::vector<TF_Output> output_ops = {{TF_GraphOperationByName(graph, "StatefulPartitionedCall"), 0}};
    if (output_ops.empty()) {
        throw std::runtime_error("Failed to get operation by name.");
    }
    std::vector<TF_Tensor *> raw_output_values(output_ops.size(), nullptr);

    // Step 3: Run the session
    //std::cout << input_ops.size() << "size of input" << std::endl;
    //std::cout << output_ops.size() << "size of output" << raw_output_values.size() << std::endl;

    TF_SessionRun(session,
                  nullptr, // run options
                  input_ops.data(), input_values_raw.data(), input_ops.size(),
                  output_ops.data(), raw_output_values.data(), raw_output_values.size(),
                  nullptr, 0, // target operations, number of targets
                  nullptr,    // run metadata
                  status      // output status
    );

    std::cout << "Predicting Called debug 3.1" << std::endl;

    if (TF_GetCode(status) != TF_OK)
    {
        std::cerr << "Error running session: " << TF_Message(status) << std::endl;
        exit(1);
    }

    // Step 4: Transfer the ownership of output tensors to smart pointers
    std::vector<std::unique_ptr<TF_Tensor, decltype(&TF_DeleteTensor)>> output_values;
    for (TF_Tensor *tensor : raw_output_values)
    {
        output_values.emplace_back(tensor, TF_DeleteTensor);
    }

    // Extract the results from the output tensor
    float *output_data = static_cast<float *>(TF_TensorData(output_values[0].get()));
    std::vector<float> results(output_data, output_data + TF_TensorByteSize(output_values[0].get()) / sizeof(float));

    return results;
}
 