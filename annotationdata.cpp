#include "annotationdata.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QDebug>

void AnnotationDataElement::addBBox(const QRect &rect, QString label)
{
    QJsonObject b;

    b.insert(QStringLiteral("left"), rect.x());
    b.insert(QStringLiteral("top"), rect.y());
    b.insert(QStringLiteral("width"), rect.width());
    b.insert(QStringLiteral("height"), rect.height());
    b.insert(QStringLiteral("label"), label);

    bboxArr.append(b);
}

void AnnotationData::addElement(const AnnotationDataElement &ele)
{
    QJsonObject jEle;
    jEle.insert(QStringLiteral("name"), ele.name);
    jEle.insert(QStringLiteral("bbox"), ele.bboxArr);

    elements.append(jEle);
}

void AnnotationData::save()
{
    QString filename = imageDir.filePath(imageDir.dirName() + QStringLiteral(".json"));
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Cannot open " << filename;
        return;
    }

    QJsonDocument doc = QJsonDocument(elements);
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
}


int AnnotationData::getNumberOfAnnotatedImages() const
{
    return elements.size();
}

int AnnotationData::getNumberOfAllImages() const
{
    return elements.size() + imageNames.size() + 1;
}

const QString AnnotationData::nextImagePath()
{
    if (imageNames.isEmpty()) {
        return QString();
    }
    else {
        return imageDir.filePath(imageNames.pop());
    }
}

bool AnnotationData::loadImageDir(const QString dirname)
{
    QDir dir(dirname);
    QStringList images = dir.entryList({"*.jpg", "*.png"}, QDir::Files, QDir::Name | QDir::Reversed | QDir::IgnoreCase);

    if (images.isEmpty())
    {
        return false;
    }

    QString filepath = dir.filePath(dir.dirName() + QStringLiteral(".json"));
    QFile file(filepath);
    QJsonArray annotatedElements;
    if (file.open(QIODevice::ReadOnly))
    {
        auto jsonbytes = file.readAll();
        file.close();
        QJsonParseError error;
        auto doc = QJsonDocument::fromJson(jsonbytes, &error);
        if (error.error == QJsonParseError::NoError && doc.isArray())
        {
            annotatedElements = doc.array();
            for (int i=0; i<annotatedElements.size(); ++i)
            {
                auto ele = annotatedElements.at(i).toObject();
                if (ele.contains(QStringLiteral("name")))
                {
                    images.removeOne(ele.value(QStringLiteral("name")).toString());
                }
            }
        }
    }

    if (images.isEmpty())
    {
        return false;
    }
    else
    {
        this->imageDir = dir;
        imageNames.clear();
        for (const QString &image : images)
        {
            imageNames.push(image);
        }

        elements = annotatedElements;

        return true;
    }

}
