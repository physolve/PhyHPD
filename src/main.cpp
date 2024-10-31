#include <iostream>
#include "mainwindow.h"
int main(int argc, char *argv[]) {
    QCoreApplication::setApplicationName("MHgrph");
    QCoreApplication::setApplicationVersion("0.0.1");
    QCoreApplication::setOrganizationName(QStringLiteral("Tomsk Polytechnic University"));
    QCoreApplication::setOrganizationDomain(QStringLiteral("tpu.ru"));
    // QLocale curLocale(QLocale("ru_RU"));
    // QLocale::setDefault(curLocale);
    qputenv("QT_FONT_DPI", QByteArray("96")); //156
    qputenv("QML_DISABLE_DISTANCEFIELD", "1");
    // qputenv("QML_XHR_ALLOW_FILE_READ", QString("1").toUtf8());
    MainWindow app(argc, argv);
    return app.exec();
}