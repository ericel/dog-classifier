#ifndef CLASSIFIERUI_H
#define CLASSIFIERUI_H

#include <QMainWindow>
#include <QDragEnterEvent>
#include <QMimeDatabase>
#include <QMimeType>
#include <QTableWidget>
#include <QHeaderView>
#include <QPainter>
#include <QPalette>

QT_BEGIN_NAMESPACE
namespace Ui { class classifierUI; }
QT_END_NAMESPACE

class classifierUI : public QMainWindow
{
    Q_OBJECT

public:
    explicit classifierUI(const char* modelPath, QWidget *parent = nullptr);
    virtual ~classifierUI();

private:
    Ui::classifierUI *ui;
};
#endif // CLASSIFIERUI_H
