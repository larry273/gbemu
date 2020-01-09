#include "imageprovider.h"

//https://github.com/DeiVadder/Sandepile-Challenge-QML/blob/master/spimageprovider.cpp

ImageProvider::ImageProvider(QObject *parent) : QObject(parent), QQuickImageProvider(QQuickImageProvider::Image)
{
    image = QImage(160,144,QImage::Format_RGB32);
    image.fill(QColor("black"));
}

QImage ImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    Q_UNUSED(id);

    if(size){
        *size = image.size();
    }

    if(requestedSize.width() > 0 && requestedSize.height() > 0) {
        image = image.scaled(requestedSize.width(), requestedSize.height(), Qt::KeepAspectRatio);
    }
    return image;
}

void ImageProvider::updateImage(const QImage &image)
{
    if(!image.isNull() && this->image != image) {
        this->image = image;
        emit imageChanged();
    }
}
