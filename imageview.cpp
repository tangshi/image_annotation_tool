#include "imageview.h"
#include <QPaintEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>
#include <QDebug>
#include <QImageReader>

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

    setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
}

void ImageView::loadImage(QString imagePath)
{
    QImageReader reader(imagePath);
    reader.setAutoTransform(true);
    const QImage image = reader.read();

    if (image.isNull())
    {
        return;
    }

    QPixmap pixmap = QPixmap::fromImage(image);

    if (pixmap.isNull())
    {
        return;
    }
    else
    {
        setPixmap(pixmap);

        emit imageLoaded(pixmap);

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
        QPoint pos = event->pos();
        int x1 = std::min(pos.x(), mLeftTop.x());
        int x2 = std::max(pos.x(), mLeftTop.x());
        int y1 = std::min(pos.y(), mLeftTop.y());
        int y2 = std::max(pos.y(), mLeftTop.y());

        mRect.setCoords(x1, y1, x2, y2);

        repaint();
    }
}

void ImageView::mouseReleaseEvent(QMouseEvent *)
{
    if (mHasLoaded && !mRect.isEmpty()) {
        emit this->mouseReleased();
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

const QRectF ImageView::getNormalizedRect() const
{
    const QRect widgetRect = rect();

    double x = static_cast<double>(mRect.x()) / widgetRect.width();
    double y = static_cast<double>(mRect.y()) / widgetRect.height();
    double w = static_cast<double>(mRect.width()) / widgetRect.width();
    double h = static_cast<double>(mRect.height()) / widgetRect.height();

    return QRectF(x, y, w, h);
}

void ImageView::clearRect()
{
    mRect = QRect();
    repaint();
}
