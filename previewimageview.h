#ifndef PREVIEWIMAGEVIEW_H
#define PREVIEWIMAGEVIEW_H

#include <QLabel>
#include <QRectF>
#include <QVector>

class QPaintEvent;

class PreviewImageView : public QLabel
{
    Q_OBJECT

public:
    explicit PreviewImageView(QWidget *parent = nullptr);

    void addRect(const QRectF &rect);

    void clearRects();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QVector<QRectF> mRectList;
};

#endif // PREVIEWIMAGEVIEW_H
