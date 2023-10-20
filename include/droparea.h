#ifndef DROPAREA_H
#define DROPAREA_H
#include <QApplication>
#include <QWidget>
#include <QMimeData>
#include <QPropertyAnimation>
#include <QColor>

#include "classifierui.h"
#include "image_processor.h"
#include "neural_network.h"

class classifierUI;  // Forward declaration

class DropArea : public QWidget {
    Q_OBJECT

public:
    explicit DropArea(classifierUI* mainWindow, const char* modelPath,  QWidget *parent = nullptr);
    
    QColor backgroundColor() const { return m_backgroundColor; }
    void setBackgroundColor(const QColor &color);

    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor)



protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
private:
    classifierUI* m_mainWindow;
    QPropertyAnimation *m_dropAnimation;
    QColor m_backgroundColor = QColor(248, 248, 248); ;

    ImageProcessor IMG_PROCESSOR;
    NeuralNetwork NN;
    std::string processAndPredict(const QString& imagePath);
};
#endif