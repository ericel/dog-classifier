#include "classifierui.h"
#include "ui_classifierui.h"
#include "droparea.h"
#include <iostream>
#include <QVBoxLayout>


classifierUI::classifierUI(const char* modelPath, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::classifierUI)
{
    ui->setupUi(this);
    // Create DropArea instance with the correct arguments
    DropArea* dropArea = new DropArea(this, modelPath, this);
    
    // Set up the QVBoxLayout
    QVBoxLayout* mainLayout = new QVBoxLayout(ui->centralwidget);

    // Add DropArea to the layout
    mainLayout->addWidget(dropArea);

    // Add QTableWidget (results) to the layout
    mainLayout->addWidget(ui->results);

    // Set equal stretch factors
    mainLayout->setStretchFactor(dropArea, 1);
    mainLayout->setStretchFactor(ui->results, 1);

    // Set the width of the first column (Image) to 200 and (Predictions) to 500 and (Status) 50 pixels
    //ui->results->setColumnWidth(0, 200);  
    ui->results->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->results->setColumnWidth(1, 500);
    ui->results->setColumnWidth(2, 55);

    // Change Active color
    QPalette palette = ui->results->palette();
    palette.setColor(QPalette::Highlight, QColor(248, 248, 248)); // Set to red color
    ui->results->setPalette(palette);
    
}

// Destructor
classifierUI::~classifierUI()
{   
    // I feel more save deleting the QT ui with delete than 
    delete ui;
}




