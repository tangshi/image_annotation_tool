#ifndef ANNOTATIONDATA_H
#define ANNOTATIONDATA_H

#include <QObject>
#include <QVector>
#include <QHash>
#include <QStack>
#include <QDir>
#include <QJsonArray>
#include <QRect>


struct AnnotationDataElement
{
    QString name;
    QJsonArray bboxArr;

    void addBBox(const QRect &rect, QString label);
};


class AnnotationData
{
public:

    bool loadImageDir(const QString dirpath);

    void save();

    /// return empty string if no more image exists
    const QString nextImagePath();

    void addElement(const AnnotationDataElement &ele);

    int getNumberOfAllImages() const;
    int getNumberOfAnnotatedImages() const;

    QDir imageDir;
    QStack<QString> imageNames;
    QJsonArray elements;
};

#endif // ANNOTATIONDATA_H
