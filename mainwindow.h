#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStack>
#include <QDir>

namespace Ui {
class MainWindow;
}

class ImageView;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void loadImageDir();
    void save();

private:
    Ui::MainWindow *ui;
    ImageView *mImageView;
    QDir mImageDir;
    QStack<QString> mImageNames;
    int mNumberOfImages;
};

#endif // MAINWINDOW_H
