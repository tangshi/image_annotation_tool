#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QLabel>
#include <QRect>

class QMouseEvent;
class QPaintEvent;

class ImageView : public QLabel
{
    Q_OBJECT

public:
    explicit ImageView(QWidget *parent = nullptr);

    void loadImage(QString imagePath);
    const QRect getRect() const;

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    QPoint mLeftTop;
    QRect mRect;
    bool mHasLoaded;
};

#endif // IMAGEVIEW_H
