#ifndef ANNOTATION_LABELS
#define ANNOTATION_LABELS

#include <QStringList>
#include <QCoreApplication>
#include <QFile>
#include <QTextStream>
#include <QDebug>


QStringList LoadAnnotationLabels();

const QStringList & GetAnnotationLabels();

#endif // ANNOTATION_LABELS

