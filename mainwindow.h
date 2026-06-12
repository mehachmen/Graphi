#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "imagewidget.h" // вместо QLabel
#include <QPushButton>
#include <QImage>
#include <QObject>
#include <QStack> // для отмены

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void openImage();
    void applyGrayscale();
    void saveImage();
    void applyInvert();
    void changeBrightness();
    void cropImage();
    void undoLastAction();
    void decreaseBrightness();

private:
    void updateDisplay();

    ImageWidget *imageWidget; // вместо QLabel

    QPushButton *undoButton;
    QPushButton *openButton;
    QPushButton *grayscaleButton;
    QPushButton *saveButton;
    QPushButton *invertButton;
    QPushButton *brightnessButton;
    QPushButton *brightnessMinusButton;
    QPushButton *cropButton;
    QStack<QImage> history; // отмена

    QImage currentImage; // сохраняет изображение
};

#endif
