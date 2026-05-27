#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "hexagon.h"
#include "board.h"
#include "types.h"


int main(int argc, char *argv[])
{
#if defined(Q_OS_WIN) && QT_VERSION_CHECK(5, 6, 0) <= QT_VERSION && QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    qmlRegisterType<Game>("App", 1, 0, "Game");
    Board* b = new Board();
    qmlRegisterSingletonInstance<Board>("App", 1, 0, "Board", b);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/qt/qml/hexgame/src/qml/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
