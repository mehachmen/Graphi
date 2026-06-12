#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QWidget>
#include <QImage>
#include <QRect>

class ImageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ImageWidget(QWidget *parent = nullptr);

    void setImage(const QImage &img);

    QImage getImage() const;

    void startCropMode();

    void applyCrop();

protected:
    void resizeEvent(QResizeEvent *event) override;
    void updateImageRect();
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:

    enum ResizeMode
    {
        None,
        Left,
        Right,
        Top,
        Bottom
    };

    QImage currentImage;
    QRect imageRect;
    QRect cropRect;
    bool cropMode = false;
    ResizeMode resizeMode = None;
    int borderSize = 10;
};

#endif
