#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "imageview.h"
#include "previewimageview.h"
#include "labelvalidator.h"
#include <QFileDialog>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QRegularExpression>
#include <QMessageBox>
#include <QDebug>
#include <QLabel>
#include <QInputDialog>
#include <QPixmap>
#include <QLineEdit>
#include <QJsonObject>
#include <QApplication>
#include <QDesktopWidget>
#include <QRect>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    labelValidator(new LabelValidator)
{
    ui->setupUi(this);

    int screenHeight = qApp->desktop()->screenGeometry().height();
    ui->previewImage->setMaximumHeight(screenHeight / 3);
    ui->imageView->setMaximumHeight(screenHeight / 3);

    for (int i = 0; i < 16; ++i)
    {
        auto lineEdit = new QLineEdit(this);
        lineEdit->setObjectName(QString("Label_") + QString::number(i));
        lineEdit->setMinimumWidth(10);
        lineEdit->setAlignment(Qt::AlignCenter);
        lineEdit->setEnabled(false);
        lineEdit->setMaxLength(1);
        lineEdit->setValidator(labelValidator);
        ui->labelsLayout->addWidget(lineEdit);

        connect(lineEdit, &QLineEdit::editingFinished, this, [this]()
        {
            QLineEdit *lineEdit = static_cast<QLineEdit*>(QObject::sender());
            QString labelName = lineEdit->objectName();
            int labelIndex = labelName.midRef(labelName.lastIndexOf('_') + 1).toInt();

            QJsonObject oldBox = currentElem.bboxArr.at(labelIndex).toObject();

            QJsonObject newBox {
                {QStringLiteral("label"),  lineEdit->text()},
                {QStringLiteral("left"),   oldBox.value(QStringLiteral("left"))},
                {QStringLiteral("top"),    oldBox.value(QStringLiteral("top"))},
                {QStringLiteral("width"),  oldBox.value(QStringLiteral("width"))},
                {QStringLiteral("height"), oldBox.value(QStringLiteral("height"))}
            };

            currentElem.bboxArr.replace(labelIndex, newBox);
        });
    }

    connect(ui->loadBtn, &QPushButton::clicked, this, &MainWindow::loadImageDir);
    connect(ui->nextBtn, &QPushButton::clicked, this, &MainWindow::next);
    connect(ui->resetBtn, &QPushButton::clicked, this, &MainWindow::reset);

    connect(ui->imageView, &ImageView::imageLoaded, this, [this](QPixmap pixmap)
    {
        ui->previewImage->setPixmap(pixmap);
    });

    connect(ui->imageView, &ImageView::mouseReleased, this, [this]()
    {
        const static QStringList kLabels {
            "0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
            "A", "B", "C", "D", "E", "F", "G",
            "H", "I", "J", "K", "L", "M", "N",
            "O", "P", "Q", "R", "S", "T",
            "U", "V", "W", "X", "Y", "Z"
        };

        bool ok;
        QString label = QInputDialog::getItem(this, QStringLiteral("标注"), QStringLiteral("请选择对应标注："), kLabels, 0, false, &ok);
        if (ok && !label.isEmpty())
        {
            if (currentElem.bboxArr.size() < 16)
            {
                qDebug() << "selected label: " << label;
                int label_index = currentElem.bboxArr.size();
                QLineEdit *labelWidget = findChild<QLineEdit *>(QString("Label_") + QString::number(label_index));
                labelWidget->setEnabled(true);
                labelWidget->setText(label);
                label_index = label_index + 1;
                ui->previewImage->addRect(ui->imageView->getNormalizedRect());

                currentElem.addBBox(ui->imageView->getRect(), label);
            }
            else
            {
                QMessageBox::warning(this, QStringLiteral("提示"), QStringLiteral("标注数目已超过16个！"), QMessageBox::Ok);
            }
        }

        ui->imageView->clearRect();
    });
}

void MainWindow::reset()
{
    while (!currentElem.bboxArr.isEmpty()) {
        currentElem.bboxArr.removeLast();
    }

    for (auto labelWidget : findChildren<QLineEdit *>())
    {
        labelWidget->setText("");
        labelWidget->setEnabled(false);
    }

    ui->previewImage->clearRects();

    ui->imageView->clearRect();
}

void MainWindow::loadImageDir()
{
    QString dirname = QFileDialog::getExistingDirectory(this, QString("载入图片文件夹"), QString(), QFileDialog::ReadOnly);
    if (dirname.isEmpty()) {return;}

    if (!annotationData.loadImageDir(dirname))
    {
        int clicked;
        clicked = QMessageBox::question(this, "", QStringLiteral("该文件夹没有需要标注的图片，是否打开另一个文件夹？"),
                                        QMessageBox::Ok, QMessageBox::Cancel, QMessageBox::NoButton);
        if (clicked == QMessageBox::Ok)
        {
            loadImageDir();
        }
    }
    else
    {
        QString nextImagePath = annotationData.nextImagePath();
        QString nextImageName = nextImagePath.mid(nextImagePath.lastIndexOf('/') + 1);
        currentElem.name = nextImageName;
        reset();
        ui->imageView->loadImage(nextImagePath);
        ui->progressBar->setRange(0, annotationData.getNumberOfAllImages());
        ui->progressBar->setValue(annotationData.getNumberOfAnnotatedImages());
    }
}

void MainWindow::next()
{
    auto LoadNext = [this]
    {
        if (!currentElem.bboxArr.isEmpty()) {
            annotationData.addElement(currentElem);
            annotationData.save();
        }

        QString nextImagePath = annotationData.nextImagePath();

        if (nextImagePath.isEmpty())
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
            QString nextImageName = nextImagePath.mid(nextImagePath.lastIndexOf('/') + 1);
            currentElem.name = nextImageName;
            reset();

            ui->progressBar->setValue(ui->progressBar->value() + 1);
            ui->imageView->loadImage(nextImagePath);
        }
    };

    if (currentElem.bboxArr.isEmpty())
    {
        QMessageBox::StandardButton clicked;
        clicked = QMessageBox::question(this, "", QString("该图片尚未标注！确定跳过吗？"),
                                        QMessageBox::StandardButtons(QMessageBox::No | QMessageBox::Yes),
                                        QMessageBox::No);
        if (clicked == QMessageBox::Yes)
        {
            LoadNext();
        }
    }
    else
    {
        annotationData.save();
        LoadNext();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
