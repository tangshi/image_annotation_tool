#include "previewimageview.h"
#include <QPainter>
#include <QRect>
#include <QApplication>


PreviewImageView::PreviewImageView(QWidget *parent) : QLabel(parent)
{
    setScaledContents(true);
    setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
}

void PreviewImageView::addRect(const QRectF &rect)
{
    mRectList.push_back(rect);
    repaint();
}

void PreviewImageView::clearRects()
{
    mRectList.clear();
    repaint();
}

void PreviewImageView::paintEvent(QPaintEvent *event)
{
    //Invoke parent's paintEvent to draw 'background'!!!
    QLabel::paintEvent(event);

    QPainter painter(this);

    int width = this->width();
    int height = this->height();
    int x, y, w, h;

    for (int i = 0; i < mRectList.size(); ++i)
    {
        const QRectF &rectf = mRectList.at(i);

        x = static_cast<int>(rectf.x() * width);
        w = static_cast<int>(rectf.width() * width);
        y = static_cast<int>(rectf.y() * height);
        h = static_cast<int>(rectf.height() * height);

        painter.setPen(QPen(Qt::blue, 1));
        painter.drawRect(x, y, w, h);

        painter.setPen(QPen(Qt::red, 1));
        painter.drawText(x, y + h, QString::number(i + 1));
    }

}
