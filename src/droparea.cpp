#include "droparea.h"

#include <iostream>

DropArea::DropArea(classifierUI* mainWindow,  const char* modelPath, QWidget *parent) 
    : QWidget(parent), m_mainWindow(mainWindow), NN(modelPath) {
    setAcceptDrops(true);

    std::cout << "Drop Area Constructor" << std::endl;
    qDebug() << "Drop Area Constructor";

    // Drop area background animation
    m_dropAnimation = new QPropertyAnimation(this, "backgroundColor");
    m_dropAnimation->setDuration(500);  // Duration of 0.5 seconds
    m_dropAnimation->setStartValue(QColor(255, 255, 255));  // Initial color: White
    m_dropAnimation->setEndValue(QColor(0, 255, 0));  // End color: Green
    m_dropAnimation->setDirection(QAbstractAnimation::Forward);
    connect(m_dropAnimation, &QPropertyAnimation::finished, [this]() {
        m_dropAnimation->setDirection(m_dropAnimation->direction() == QAbstractAnimation::Forward
                                      ? QAbstractAnimation::Backward
                                      : QAbstractAnimation::Forward);
        m_dropAnimation->start();
    });

}
void DropArea::setBackgroundColor(const QColor &color) {
    m_backgroundColor = color;
    update();  // Trigger a repaint
}
void DropArea::paintEvent(QPaintEvent *event) {
    QPainter painter(this);

    // Draw the background color
    painter.fillRect(rect(), m_backgroundColor);

    // Load the image
    QPixmap pixmap(":/assets/droparea.png"); 

    // Draw the pixmap onto the widget
    painter.drawPixmap(0, 0, width(), height(), pixmap);

    QWidget::paintEvent(event);
}


void DropArea::dragEnterEvent(QDragEnterEvent *event) {
    std::cout << "Entering drag enter event" << std::endl;
    // Start Animation
    m_dropAnimation->start();


    const QMimeData *mimeData = event->mimeData();
    if (mimeData->hasUrls()) {
        QMimeDatabase mimeDatabase; // Create an instance of QMimeDatabase
        bool allAreImages = true;
        
        for (const QUrl &url : mimeData->urls()) {
            QString filePath = url.toLocalFile();
            
            // Determine the MIME type of the file
            QMimeType mimeType = mimeDatabase.mimeTypeForFile(filePath);
            
            if (!mimeType.name().startsWith("image/")) {
                allAreImages = false;
                break;
            }
        }
        
        if (allAreImages) {
            event->acceptProposedAction();
        }
    }
    std::cout << "Living drag enter event" << std::endl;
   
}



void DropArea::dragMoveEvent(QDragMoveEvent *event) {
    event->acceptProposedAction();
}

void DropArea::dropEvent(QDropEvent *event) {
    // Process the dropped items...
    event->acceptProposedAction();
    // Stop the animation once all images are dropped
    m_dropAnimation->stop();
    setBackgroundColor(QColor(255, 255, 255));  // Optionally reset the background color to white

    QList<QUrl> urls = event->mimeData()->urls();
    if (urls.isEmpty()) {
        return;
    }

    for (const QUrl &url : urls) {
        QString filePath = url.toLocalFile();

        // Process the image and get the prediction
        // Neural Network should be called here.
        QString prediction = QString::fromStdString(processAndPredict(filePath));

        // Create a thumbnail of the dropped image
        QPixmap originalPixmap(filePath);
        QPixmap thumbnail = originalPixmap.scaled(100, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation);

        // Add the image and prediction to the table
        QTableWidget *table = m_mainWindow->findChild<QTableWidget*>("results");

        int newRow = 0;  // Insert at the beginning
        table->insertRow(newRow);

        // Add image thumbnail to the left column
        QTableWidgetItem *imageItem = new QTableWidgetItem();
        table->setIconSize(QSize(100, 80));
        imageItem->setIcon(QIcon(thumbnail)); 
        table->setItem(newRow, 0, imageItem);
        table->setStyleSheet("QTableWidget::item { padding: 0px; margin: 0px; }");

        // Add the applyIcon to the last column
        QIcon originalIcon;
        if(prediction.contains("DOG", Qt::CaseInsensitive)){
            originalIcon = QApplication::style()->standardIcon(QStyle::SP_DialogApplyButton);
        } else {
            originalIcon = QApplication::style()->standardIcon(QStyle::SP_DialogCancelButton);
        }
        QPixmap pixmap = originalIcon.pixmap(originalIcon.availableSizes().first());  // Get the QPixmap from QIcon
        QPixmap resizedPixmap = pixmap.scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation);  // Resize the QPixmap
        QIcon resizedIcon(resizedPixmap);  // Convert the QPixmap back to QIcon

        // Create a new QTableWidgetItem and set the resized icon
        QTableWidgetItem *iconItem = new QTableWidgetItem();
        iconItem->setIcon(resizedIcon);
        table->setItem(newRow, 2, iconItem);  // Assuming 2 is the column where you want to place the icon

        // Resizes the row accordingly
        QHeaderView *verticalHeader = table->verticalHeader();
        verticalHeader->setSectionResizeMode(QHeaderView::Fixed);
        verticalHeader->setDefaultSectionSize(100);

        // Add prediction to the right column
        QTableWidgetItem *predictionItem = new QTableWidgetItem(prediction);
        table->setItem(newRow, 1, predictionItem);
    }
}




void DropArea::dragLeaveEvent(QDragLeaveEvent *event) {
    // Handle drag leave if necessary
    Q_UNUSED(event);
}

// Implementation of the processAndPredict method
std::string DropArea::processAndPredict(const QString& imagePath)
{
    std::vector<float> processedImage = IMG_PROCESSOR.processImage(imagePath.toStdString());

    if (processedImage.empty()) {
        std::cerr << "Failed to process the image!" << std::endl;
        return "Failed to process the image!";
    }
    
    // Handle The Neural Network
    try {
        std::vector<float> predictions = NN.predict(processedImage);

        // Display prediction results (assuming binary classification for simplicity)
        std::string output;
        if(predictions[0] <= 0.5) {
            std::cout << "The image is likely a DOG with confidence: " << 1 - predictions[0] << std::endl;
            return "The image is likely a DOG with confidence: " + std::to_string(1 - predictions[0]);
        } else {
            std::cout << "The image likely belongs to some other class with confidence: " << predictions[0] << std::endl;
            return "The image likely belongs to some other class with confidence: " + std::to_string(predictions[0]);
        }
    } catch (const std::exception& e) {
        std::cerr << "An error occurred while predicting the image: " << e.what() << std::endl;
        return "Unable to classify the image due to an internal error.";
    }

}