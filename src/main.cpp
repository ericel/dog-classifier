#include <iostream>
#include <QApplication>
#include "classifierui.h"
#include "image_processor.h"
#include "neural_network.h"
#include <QDir>
// #include "split_move_files.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Define NN Model Path
    QDir dir(QCoreApplication::applicationDirPath());
    QString modelPathStr = dir.filePath("../thirdparty/my_model/");
    qDebug() << "Model path:" << modelPathStr;
    std::string modelPathStd = modelPathStr.toStdString();
    const char *modelPath = modelPathStd.c_str();
    // Create the main window using the constructor that accepts the model path
    classifierUI mainWindow(modelPath);
    // Set the style for the mainWindow
    // mainWindow.setStyleSheet("background-color: cyan;");  // Change "cyan" to any color you prefer

    mainWindow.show();

    return app.exec();
}