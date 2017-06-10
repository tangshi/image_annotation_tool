#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStack>
#include <QDir>
#include "annotationdata.h"

namespace Ui {
class MainWindow;
}

class ImageView;
class QLabel;
class LabelValidator;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void loadImageDir();
    void next();
    void reset();

private:
    Ui::MainWindow *ui;
    AnnotationDataElement currentElem;
    AnnotationData annotationData;
    LabelValidator *labelValidator;
};

#endif // MAINWINDOW_H
