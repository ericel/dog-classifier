#include <iostream>
#include <vector>
#include <filesystem>
#include <algorithm>
#include <random>
#include <iterator>

// For C++17 and above
namespace fs = std::filesystem;

// Helper function to split and move files for a given animal type
void splitAndMoveFiles(const fs::path& source_folder, const fs::path& train_folder, const fs::path& test_folder, std::string obj) {
    // Create destination directories if they don't exist
    fs::create_directories(train_folder);
    fs::create_directories(test_folder);

    // List all files in the source directory
    std::vector<fs::path> all_files;
    for (const auto& entry : fs::directory_iterator(source_folder)) {
        if (entry.is_regular_file()) { // Check if it's a file and not a directory
            auto extension = entry.path().extension().string();
            if (extension == ".jpg" || extension == ".jpeg" || extension == ".png") {
                // Push only image files
                all_files.push_back(entry.path());
            }
        }
    }

    std::cout << "All files in count for " << obj << ": " << all_files.size() << " files" << std::endl;
    // Shuffle the files
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(all_files.begin(), all_files.end(), g);

    // Split files into training and testing sets
    size_t num_train = static_cast<size_t>(all_files.size() * 0.8);  // 80% for training
    std::vector<fs::path> train_files(all_files.begin(), all_files.begin() + num_train);
    std::vector<fs::path> test_files(all_files.begin() + num_train, all_files.end());

    // Move the files
    for (const auto& f : train_files) {
        fs::path dest = train_folder / f.filename();
        fs::rename(f, dest);
    }


    for (const auto& f : test_files) {
        fs::path dest = test_folder / f.filename();
        fs::rename(f, dest);
    }

    std::cout << "Moved " << train_files.size() << " images to " << train_folder << std::endl;
    std::cout << "Moved " << test_files.size() << " images to " << test_folder << std::endl;
}

void setTrainingData() {
    // Set source and destination directories for dogs
    fs::path source_dogs = "/Applications/dev/cplusplus/dog-classifier/data/all_dogs";
    fs::path train_dogs = "/Applications/dev/cplusplus/dog-classifier/data/train/dogs";
    fs::path test_dogs = "/Applications/dev/cplusplus/dog-classifier/data/test/dogs";

    // Call helper function for dogs
    splitAndMoveFiles(source_dogs, train_dogs, test_dogs, "dogs");

    // Set source and destination directories for cats
    fs::path source_cats = "/Applications/dev/cplusplus/dog-classifier/data/all_cats";
    fs::path train_cats = "/Applications/dev/cplusplus/dog-classifier/data/train/cats";
    fs::path test_cats = "/Applications/dev/cplusplus/dog-classifier/data/test/cats";

    // Call helper function for cats
    splitAndMoveFiles(source_cats, train_cats, test_cats, "cats");
}
