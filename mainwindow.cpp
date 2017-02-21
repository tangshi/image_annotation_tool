#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "imageview.h"
#include <QFileDialog>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QRegularExpression>
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mNumberOfImages(0)
{
    ui->setupUi(this);

    mImageView = ui->imageView;

    connect(ui->loadBtn, &QPushButton::clicked, this, &MainWindow::loadImageDir);
    connect(ui->saveBtn, &QPushButton::clicked, this, &MainWindow::save);
}

void MainWindow::loadImageDir()
{
    QString dirname = QFileDialog::getExistingDirectory(this, QString("载入图片文件夹"), QString(), QFileDialog::ReadOnly);
    if (dirname.isEmpty()) {return;}

    QDir dir(dirname);
    QStringList images = dir.entryList({"*.jpg", "*.png"}, QDir::Files, QDir::Name | QDir::Reversed | QDir::IgnoreCase);
    if (images.isEmpty()) {return;}

    QString filepath = dir.filePath(dir.dirName() + QStringLiteral(".txt"));
    QFile file(filepath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QRegularExpression re("^(\\S+)\\s+\\d+\\s+\\d+\\s+\\d+\\s+\\d+");

        QTextStream in(&file);
        in.setCodec("UTF-8");
        while (!in.atEnd()) {
            QString line = in.readLine();
            QRegularExpressionMatch match = re.match(line);
            if (match.hasMatch())
            {
                QString imagename = match.captured(1);
                images.removeOne(imagename);
            }
        }
        file.close();
    }

    if (images.isEmpty())
    {
        int clicked;
        clicked = QMessageBox::question(this, "", QString("该文件夹没有需要标注的图片，是否打开另一个文件夹？"),
                                        QMessageBox::Ok, QMessageBox::Cancel, QMessageBox::NoButton);
        if (clicked == QMessageBox::Ok)
        {
            loadImageDir();
        }
    }
    else
    {
        this->mImageDir = dir;
        for (const QString &image : images)
        {
            mImageNames.push(image);
        }
        mNumberOfImages = mImageNames.size();
        ui->progressBar->setRange(0, mNumberOfImages);
        ui->progressBar->setValue(1);
        mImageView->loadImage(dir.filePath(mImageNames.top()));
    }
}


void MainWindow::save()
{
    auto LoadNext = [this]
    {
        if (mImageNames.isEmpty())
        {
            int clicked;
            clicked = QMessageBox::question(this, "", QString("该文件夹的图片标注已全部完成，是否打开另一个文件夹？"),
                                            QMessageBox::Ok, QMessageBox::Cancel, QMessageBox::NoButton);
            if (clicked == QMessageBox::Ok)
            {
                loadImageDir();
            }
        }
        else
        {
            ui->progressBar->setValue(ui->progressBar->value() + 1);
            mImageView->loadImage(mImageDir.filePath(mImageNames.top()));
        }
    };

    const QRect rect = mImageView->getRect();

    if (rect.isNull())
    {
        QMessageBox::StandardButton clicked;
        clicked = QMessageBox::question(this, "", QString("该图片尚未标注！确定跳过吗？"),
                                        QMessageBox::StandardButtons(QMessageBox::No | QMessageBox::Yes),
                                        QMessageBox::No);
        if (clicked == QMessageBox::Yes)
        {
            mImageNames.pop();
            LoadNext();
        }
    }
    else
    {
        QString filename = mImageDir.filePath(mImageDir.dirName() + QStringLiteral(".txt"));
        QFile file(filename);
        if (file.open(QFile::Append | QFile::Text))
        {
            QTextStream in(&file);
            in.setCodec("UTF-8");
            in << mImageNames.pop() << ' '
               << QString::number(rect.x()) << ' '
               << QString::number(rect.y()) << ' '
               << QString::number(rect.width()) << ' '
               << QString::number(rect.height()) << '\n';

            in.flush();
            file.close();
        }

        LoadNext();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
