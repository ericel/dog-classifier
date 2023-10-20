cmake_minimum_required(VERSION 3.10)
project(DogClassifier)

# Set the default build type to Debug
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif()

# Find OpenCV package
find_package(OpenCV REQUIRED)

# Include OpenCV directories and your include directory
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(include)

# Include TensorFlow headers
include_directories(/usr/local/Cellar/libtensorflow/2.12.0/include)  # <-- Replace this with actual path

# Automatically collect all source files from the "src" directory
file(GLOB SOURCES "src/*.cpp" "utils/*.cpp" "resources.qrc")

# Find Qt6 packages
find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)

# Include the current directory (where the moc files will be generated)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# List of header files that contain Q_OBJECT and need to be processed by MOC
set(MOC_HEADERS
./include/classifierui.h  # Replace 'path/to' with the actual path to your header file
./include/droparea.h 
)

# Create the executable
add_executable(DogClassifier ${SOURCES} ${MOC_HEADERS})

# Link against OpenCV libraries, TensorFlow libraries, and Qt6 libraries
target_link_libraries(DogClassifier PRIVATE 
    ${OpenCV_LIBS}
    /usr/local/Cellar/libtensorflow/2.12.0/lib/libtensorflow.so  # <-- Replace this with actual path
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

# Set C++ standard to C++17
set_property(TARGET DogClassifier PROPERTY CXX_STANDARD 17)

# Compile options
# TODO: Run -Werror in CI.
target_compile_options(DogClassifier PRIVATE -Wall -Wextra)

# For Qt: let CMake know we will be using the automoc feature
set_target_properties(DogClassifier PROPERTIES AUTOMOC ON AUTOUIC ON AUTORCC ON)
