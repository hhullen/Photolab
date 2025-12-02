#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QStyleHints>
#include <QPalette>

int main(int argc, char *argv[])
{

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
