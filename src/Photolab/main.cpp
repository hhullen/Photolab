#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QStyleHints>
#include <QPalette>
#include <QDirIterator>

#include <QDebug>
#include <QFile>

int main(int argc, char *argv[]) {

    // QDirIterator it(":", QDirIterator::Subdirectories);
    // while (it.hasNext()) {
    //     qDebug() << it.next();
    // }

    QGuiApplication app(argc, argv);
    QGuiApplication::setPalette(QGuiApplication::palette());

    QObject::connect(app.styleHints(), &QStyleHints::colorSchemeChanged, []() {
        QGuiApplication::setPalette(QGuiApplication::palette());

    });


    QQmlApplicationEngine engine;

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("Photolab", "Main");

    return app.exec();
}
