#include "imagewidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QPainterPath>
#include <QPen>
#include <QtMath>

ImageWidget::ImageWidget(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(400, 400);
}

void ImageWidget::setImage(const QImage &img)
{
    currentImage = img;
    updateImageRect();
    update();
}

QImage ImageWidget::getImage() const
{
    return currentImage;
}

void ImageWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    updateImageRect();
}

void ImageWidget::updateImageRect()
{
    if (currentImage.isNull())
        return;

    QSize widgetSize = size();
    QSize imgSize = currentImage.size();

    float scale = qMin(
        (float)widgetSize.width() / imgSize.width(),
        (float)widgetSize.height() / imgSize.height()
        );

    QSize scaledSize(
        imgSize.width() * scale,
        imgSize.height() * scale
        );

    int x = (widgetSize.width() - scaledSize.width()) / 2;
    int y = (widgetSize.height() - scaledSize.height()) / 2;

    imageRect = QRect(QPoint(x, y), scaledSize);
}

void ImageWidget::startCropMode()
{
    cropMode = true;

    cropRect = QRect(
        imageRect.x() + imageRect.width() / 4,
        imageRect.y() + imageRect.height() / 4,
        imageRect.width() / 2,
        imageRect.height() / 2
        );

    update();
}

void ImageWidget::applyCrop()
{
    if (!cropMode || currentImage.isNull())
        return;

    float scaleX = (float)currentImage.width() / imageRect.width();
    float scaleY = (float)currentImage.height() / imageRect.height();

    QRect imageCrop(
        (cropRect.x() - imageRect.x()) * scaleX,
        (cropRect.y() - imageRect.y()) * scaleY,
        cropRect.width() * scaleX,
        cropRect.height() * scaleY
        );

    currentImage = currentImage.copy(imageCrop);

    cropMode = false;

    updateImageRect();
    update();
}

void ImageWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    if (currentImage.isNull())
        return;

    painter.drawImage(imageRect, currentImage);

    if (!cropMode)
        return;

    QPainterPath outer;
    outer.addRect(imageRect);

    QPainterPath inner;
    inner.addRect(cropRect);

    painter.fillPath(outer.subtracted(inner), QColor(0, 0, 0, 120));

    painter.setPen(QPen(Qt::white, 3));
    painter.drawRect(cropRect);
}

void ImageWidget::mousePressEvent(QMouseEvent *event)
{
    if (!cropMode)
        return;

    QPoint pos = event->pos();

    if (!imageRect.contains(pos))
        return;

    if (qAbs(pos.x() - cropRect.left()) < borderSize)
        resizeMode = Left;

    else if (qAbs(pos.x() - cropRect.right()) < borderSize)
        resizeMode = Right;

    else if (qAbs(pos.y() - cropRect.top()) < borderSize)
        resizeMode = Top;

    else if (qAbs(pos.y() - cropRect.bottom()) < borderSize)
        resizeMode = Bottom;

    else
        resizeMode = None;
}

void ImageWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (!cropMode)
        return;

    QPoint pos = event->pos();

    switch (resizeMode)
    {
    case Left:
        if (pos.x() < cropRect.right() - 20)
            cropRect.setLeft(pos.x());
        break;

    case Right:
        if (pos.x() > cropRect.left() + 20)
            cropRect.setRight(pos.x());
        break;

    case Top:
        if (pos.y() < cropRect.bottom() - 20)
            cropRect.setTop(pos.y());
        break;

    case Bottom:
        if (pos.y() > cropRect.top() + 20)
            cropRect.setBottom(pos.y());
        break;

    default:
        break;
    }

    update();
}

void ImageWidget::mouseReleaseEvent(QMouseEvent *)
{
    resizeMode = None;
}
