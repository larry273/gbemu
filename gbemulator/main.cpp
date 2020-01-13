#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <QDebug>

#include "interface.h"
#include "imageprovider.h"






int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    qmlRegisterType<Interface>("Interface", 1, 0, "Interface");


    ImageProvider *imageProvider(new ImageProvider());

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("imageProvider", imageProvider);
    engine.addImageProvider("live", imageProvider);


    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    //get interface qobject in qml
    Interface *interface = engine.rootObjects().first()->findChild<Interface*>("gameboy");
    QObject::connect(interface, &Interface::imageChanged, imageProvider, &ImageProvider::updateImage);


    return app.exec();
}
