#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QStyleHints>
#include <QPalette>
#include <QQmlEngine>
#include <QJSEngine>

#include "backend/backend.h"
#include "controller/controller.h"
#include "gui/pictureplane_item.h"

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);
    QGuiApplication::setPalette(QGuiApplication::palette());

    QObject::connect(app.styleHints(), &QStyleHints::colorSchemeChanged, []() {
        QGuiApplication::setPalette(QGuiApplication::palette());
    });

    Backend &back = Backend::Get();
    Controller::Get().setBackend(&back);

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
