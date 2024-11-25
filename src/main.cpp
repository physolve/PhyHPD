#include <iostream>
#include "mainwindow.h"
int main(int argc, char *argv[]) {
    QCoreApplication::setApplicationName("PhyHPD");
    QCoreApplication::setApplicationVersion("0.0.1");
    QCoreApplication::setOrganizationName(QStringLiteral("Tomsk Polytechnic University"));
    QCoreApplication::setOrganizationDomain(QStringLiteral("tpu.ru"));
    // QLocale curLocale(QLocale("ru_RU"));
    // QLocale::setDefault(curLocale);
    qputenv("QT_FONT_DPI", QByteArray("156")); //96 try to switch by resolution?
    qputenv("QML_DISABLE_DISTANCEFIELD", "1");
    // qputenv("QT_QUICK_CONTROLS_CONF", ":/qml/qtquickcontrols2.conf");
    QQuickWindow::setTextRenderType(QQuickWindow::CurveTextRendering);
    // qputenv("QML_XHR_ALLOW_FILE_READ", QString("1").toUtf8());
    MainWindow app(argc, argv);
    return app.exec();
}