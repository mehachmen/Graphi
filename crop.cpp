#include "crop.h"

#include <QPainter>
#include <QMouseEvent>
#include <QPainterPath>

CropDialog::CropDialog(const QImage &img, QWidget *parent) : QDialog(parent), image(img)
{
    setFixedSize(image.size());

    cropRect = QRect(200, 150, 300, 300);

    setWindowTitle("Обрезка");
    setModal(true);
}

void CropDialog::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    // Рисуем изображение
    painter.drawImage(rect(), image);

    // Затемнение вне рамки
    QPainterPath outer;
    outer.addRect(rect());

    QPainterPath inner;
    inner.addRect(cropRect);

    QPainterPath darkArea = outer.subtracted(inner);

    painter.fillPath(darkArea, QColor(0, 0, 0, 120));

    // Белая рамка
    painter.setPen(QPen(Qt::white, 3));
    painter.drawRect(cropRect);
}

void CropDialog::mousePressEvent(QMouseEvent *event)
{
    QPoint pos = event->pos();

    // Левая сторона
    if (abs(pos.x() - cropRect.left()) < borderSize)
    {
        resizeMode = Left;
    }

    // Правая сторона
    else if (abs(pos.x() - cropRect.right()) < borderSize)
    {
        resizeMode = Right;
    }

    // Верхняя сторона
    else if (abs(pos.y() - cropRect.top()) < borderSize)
    {
        resizeMode = Top;
    }

    // Нижняя сторона
    else if (abs(pos.y() - cropRect.bottom()) < borderSize)
    {
        resizeMode = Bottom;
    }
}

void CropDialog::mouseMoveEvent(QMouseEvent *event)
{
    QPoint pos = event->pos();

    switch (resizeMode)
    {
    case Left:

        if (pos.x() < cropRect.right() - 50)
            cropRect.setLeft(pos.x());

        break;

    case Right:

        if (pos.x() > cropRect.left() + 50)
            cropRect.setRight(pos.x());

        break;

    case Top:

        if (pos.y() < cropRect.bottom() - 50)
            cropRect.setTop(pos.y());

        break;

    case Bottom:

        if (pos.y() > cropRect.top() + 50)
            cropRect.setBottom(pos.y());

        break;

    default:
        break;
    }

    update();
}

void CropDialog::mouseReleaseEvent(QMouseEvent *)
{
    resizeMode = None;
}

QImage CropDialog::getCroppedImage() const
{
    return image.copy(cropRect);
}

