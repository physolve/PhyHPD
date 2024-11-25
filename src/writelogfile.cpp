#include "writelogfile.h"
#include <QDate>

WriteLogFile::WriteLogFile(QObject* parent) : QObject(parent), file(QString("log-%1.txt").arg(QDate::currentDate().toString())){
    if (!file.open(QIODevice::ReadWrite))
        return;
    fileName = file.fileName();
    QString line;
    QString header = "Time\tElapsed\tPressure\tVacuum";
    QTextStream in(&file);
    line = in.readLine();
    if (line != header) {
        QTextStream head(&file);
        head << header << "\n";
    }

}
WriteLogFile::~WriteLogFile(){
    file.close();
}

void WriteLogFile::writeLine(const QString &line){
    QTextStream out(&file);
    out << QTime::currentTime().toString() << "\t";
    out << line << "\n";
}