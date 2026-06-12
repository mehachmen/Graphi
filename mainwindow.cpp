#include "mainwindow.h"

#include <QFileDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QResizeEvent>
#include <QColor>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    QWidget *central = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(central);
    setWindowTitle("Graphi 0.1");

    imageWidget = new ImageWidget();
    mainLayout->addWidget(imageWidget);

    // Кнопки
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    undoButton = new QPushButton("↶");

    openButton = new QPushButton("Открыть");

    grayscaleButton = new QPushButton("Ч/Б");

    invertButton = new QPushButton("Инверсия");

    brightnessButton = new QPushButton("Яркость +50");

    brightnessMinusButton = new QPushButton("Яркость -50");

    cropButton = new QPushButton("Обрезать");

    saveButton = new QPushButton("Сохранить");

    buttonLayout->addWidget(undoButton);

    buttonLayout->addWidget(openButton);

    buttonLayout->addWidget(grayscaleButton);

    buttonLayout->addWidget(invertButton);

    buttonLayout->addWidget(brightnessButton);

    buttonLayout->addWidget(brightnessMinusButton);

    buttonLayout->addWidget(cropButton);

    buttonLayout->addWidget(saveButton);

    mainLayout->addLayout(buttonLayout);

    setCentralWidget(central);

    // Connect
    connect(openButton,
            &QPushButton::clicked,
            this,
            &MainWindow::openImage);

    connect(grayscaleButton,
            &QPushButton::clicked,
            this,
            &MainWindow::applyGrayscale);

    connect(invertButton,
            &QPushButton::clicked,
            this,
            &MainWindow::applyInvert);

    connect(brightnessButton,
            &QPushButton::clicked,
            this,
            &MainWindow::changeBrightness);

    connect(brightnessMinusButton,
            &QPushButton::clicked,
            this,
            &MainWindow::decreaseBrightness);

    connect(cropButton,
            &QPushButton::clicked,
            this,
            &MainWindow::cropImage);

    connect(saveButton,
            &QPushButton::clicked,
            this,
            &MainWindow::saveImage);

    connect(undoButton,
            &QPushButton::clicked,
            this,
            &MainWindow::undoLastAction);
}

void MainWindow::openImage()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "Открыть изображение",
        "",
        "Images (*.png *.jpg *.bmp)"
        );

    if (fileName.isEmpty())
        return;

    QImage image;

    if (!image.load(fileName))
    {
        QMessageBox::warning(
            this,
            "Ошибка",
            "Не удалось загрузить изображение"
            );

        return;
    }

    currentImage = image;

    history.clear();

    updateDisplay();
}

void MainWindow::updateDisplay()
{
    if (currentImage.isNull())
        return;

    imageWidget->setImage(currentImage);
}

void MainWindow::applyGrayscale()
{
    if (currentImage.isNull())
        return;

    history.push(currentImage);

    for (int y = 0; y < currentImage.height(); ++y)
    {
        for (int x = 0; x < currentImage.width(); ++x)
        {
            QColor color = currentImage.pixelColor(x, y);

            int gray = static_cast<int>(
                0.299 * color.red() +
                0.587 * color.green() +
                0.114 * color.blue()
                );

            color.setRed(gray);
            color.setGreen(gray);
            color.setBlue(gray);

            currentImage.setPixelColor(x, y, color);
        }
    }

    updateDisplay();
}

void MainWindow::applyInvert()
{
    if (currentImage.isNull())
        return;

    history.push(currentImage);

    for (int y = 0; y < currentImage.height(); ++y)
    {
        for (int x = 0; x < currentImage.width(); ++x)
        {
            QColor color = currentImage.pixelColor(x, y);

            color.setRed(255 - color.red());
            color.setGreen(255 - color.green());
            color.setBlue(255 - color.blue());

            currentImage.setPixelColor(x, y, color);
        }
    }

    updateDisplay();
}

void MainWindow::changeBrightness()
{
    if (currentImage.isNull())
        return;

    history.push(currentImage);

    int brightness = 50;

    for (int y = 0; y < currentImage.height(); ++y)
    {
        for (int x = 0; x < currentImage.width(); ++x)
        {
            QColor color = currentImage.pixelColor(x, y);

            int r = qBound(0, color.red() + brightness, 255);

            int g = qBound(0, color.green() + brightness, 255);

            int b = qBound(0, color.blue() + brightness, 255);

            color.setRed(r);
            color.setGreen(g);
            color.setBlue(b);

            currentImage.setPixelColor(x, y, color);
        }
    }

    updateDisplay();
}

void MainWindow::decreaseBrightness()
{
    if (currentImage.isNull())
        return;

    history.push(currentImage);

    int brightness = -50;

    for (int y = 0; y < currentImage.height(); ++y)
    {
        for (int x = 0; x < currentImage.width(); ++x)
        {
            QColor color = currentImage.pixelColor(x, y);

            int r = qBound(0, color.red() + brightness, 255);
            int g = qBound(0, color.green() + brightness, 255);
            int b = qBound(0, color.blue() + brightness, 255);

            color.setRed(r);
            color.setGreen(g);
            color.setBlue(b);

            currentImage.setPixelColor(x, y, color);
        }
    }

    updateDisplay();
}

void MainWindow::cropImage()
{
    static bool cropping = false;

    if (currentImage.isNull())
        return;

    if (!cropping)
    {
        imageWidget->startCropMode();

        cropButton->setText("OK");

        cropping = true;
    }
    else
    {
        history.push(currentImage);

        imageWidget->applyCrop();

        currentImage = imageWidget->getImage();

        cropButton->setText("Обрезать");

        cropping = false;

        updateDisplay();
    }
}

void MainWindow::undoLastAction()
{
    if (history.isEmpty())
        return;

    currentImage = history.pop();

    updateDisplay();
}

void MainWindow::saveImage()
{
    if (currentImage.isNull())
    {
        QMessageBox::warning(
            this,
            "Ошибка",
            "Нет изображения для сохранения"
            );

        return;
    }

    QString fileName = QFileDialog::getSaveFileName(
        this,
        "Сохранить изображение",
        "",
        "PNG (*.png);;JPEG (*.jpg *.jpeg);;BMP (*.bmp)"
        );

    if (fileName.isEmpty())
        return;

    if (!currentImage.save(fileName))
    {
        QMessageBox::warning(
            this,
            "Ошибка",
            "Не удалось сохранить изображение"
            );
    }
    else
    {
        QMessageBox::information(
            this,
            "УРА!",
            "Изображение сохранено"
            );
    }
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);

    updateDisplay();
}
