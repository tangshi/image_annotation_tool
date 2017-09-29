#include "labels.h"

QStringList LoadAnnotationLabels() {
    QString labelFilePath = QCoreApplication::applicationDirPath() + "/labels.txt";
    QFile file(labelFilePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Disable to read label file";
        exit(-1);
    }

    QStringList labels;
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (not line.isEmpty()) {
            labels << line;
        }
    }

    return labels;
}

const QStringList & GetAnnotationLabels() {
    const static QStringList kLabels = LoadAnnotationLabels();
    return kLabels;
}
