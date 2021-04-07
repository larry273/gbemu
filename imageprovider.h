#include <QObject>
#include <QImage>
/*
//class to handle image updating for qml interface
class ImageProvider : public QObject, public QQuickImageProvider
{
    Q_OBJECT

public:
    ImageProvider(QObject *parent = nullptr);
    //override default function from qquickimageprovider
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;

public slots:
    void updateImage(const QImage &image);

signals:
    //signal to notify frame change
    void imageChanged();

private:
    QImage image;

};

#endif // IMAGEPROVIDER_H
*/
