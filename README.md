# Dog-Classfier

This is a C++ desktop Dog Image Identification application. 

It uses Neural Network Trained in Python Tensorflow to Determine whether a given image is a Dog and the confidence level!

![Dog Classifier](assets/dog-classifier.png)

## Build on top of QT UI
QT one of the most popular C++ framework / tools for developing/designing Applications.
You are welcome to develop/try this project on your local machine.


## Make
This project uses [Make](https://www.gnu.org/software/make/). The Makefile has four targets:
* `build` compiles the source code and generates an executable
* `format` applies [ClangFormat](https://clang.llvm.org/docs/ClangFormat.html) to style the source code
* `debug` compiles the source code and generates an executable, including debugging symbols
* `clean` deletes the `build/` directory, including all of the build artifacts

## Instructions

1. Clone the project repository: `git clone `

2. Build the project: `make build`

3. Run the resulting executable: `./DogClassifier`
![Starting Dog Classifier](images/dog-classifier-startup.png)

