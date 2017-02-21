#include "imageview.h"
#include <QPaintEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>
#include <QDebug>

using namespace Qt;

ImageView::ImageView(QWidget *parent) : QLabel(parent)
{
    mRect = QRect();
    mLeftTop = QPoint();
    mHasLoaded = false;

    setFrameShape(QFrame::Box);
    setFrameShadow(QFrame::Plain);

    setMouseTracking(false);
    setScaledContents(true);

    setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
}

void ImageView::loadImage(QString imagePath)
{
    QPixmap pixmap(imagePath);
    if (pixmap.isNull())
    {
        return;
    }
    else
    {
        setPixmap(pixmap);

        mRect = QRect();
        mLeftTop = QPoint();
        mHasLoaded = true;

        repaint();
    }
}

void ImageView::mousePressEvent(QMouseEvent *event)
{
    if (mHasLoaded)
    {
        mLeftTop = event->pos();
    }
}

void ImageView::mouseMoveEvent(QMouseEvent *event)
{
    if (mHasLoaded)
    {
        mRect.setCoords(mLeftTop.x(), mLeftTop.y(), event->pos().x(), event->pos().y());
        qDebug() << "rect: " << mRect << getRect();
        repaint();
    }
}

void ImageView::paintEvent(QPaintEvent *event)
{
    //Invoke parent's paintEvent to draw 'background'!!!
    QLabel::paintEvent(event);

    if (! mRect.isNull() && mHasLoaded)
    {
        QPainter painter(this);
        painter.setPen(QPen(Qt::red, 1));
        painter.drawRect(mRect);
    }
}

const QRect ImageView::getRect() const
{
    const QRect pixmapRect = pixmap()->rect();

    const QRect widgetRect = rect();
    double xRatio = static_cast<double>(mRect.x()) / widgetRect.width();
    double yRatio = static_cast<double>(mRect.y()) / widgetRect.height();
    double wRatio = static_cast<double>(mRect.width()) / widgetRect.width();
    double hRatio = static_cast<double>(mRect.height()) / widgetRect.height();

    int x = static_cast<int>(xRatio * pixmapRect.width());
    int y = static_cast<int>(yRatio * pixmapRect.height());
    int w = static_cast<int>(wRatio * pixmapRect.width());
    int h = static_cast<int>(hRatio * pixmapRect.height());

    return QRect(x, y, w, h);
}
