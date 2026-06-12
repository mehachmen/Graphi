#ifndef CROP_H
#define CROP_H

#include <QDialog>
#include <QImage>
#include <QRect>

class CropDialog : public QDialog
{
    Q_OBJECT

public:
    CropDialog(const QImage &image, QWidget *parent = nullptr);

    QImage getCroppedImage() const;

protected:
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

    QImage image;

    QRect cropRect;

    ResizeMode resizeMode = None;

    int borderSize = 10;
};

#endif
