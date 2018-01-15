#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QQmlContext>
#include "maincontroller.h"
#include "utils.h"

int main(int argc, char *argv[])
{
#ifdef AUTO_HIDE_NAVI
    Utils::hideNavigateBar();
#endif

    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    qmlRegisterType<MainController>("MainController", 1, 0, "MainController");

    QQuickStyle::setStyle("Material");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    int iret = app.exec();
#ifdef AUTO_HIDE_NAVI
    Utils::showNavigateBar();
#endif

    return iret;
}
