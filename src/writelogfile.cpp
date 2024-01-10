#include "writelogfile.h"

WriteLogFile::WriteLogFile(QObject* parent) : QObject(parent){
    QFile file(QString("log-%1.txt").arg(QDate::currentDate().toString()));
    if (!file.open(QIODevice::ReadWrite))
        return;
    QString line;
    QString header = "Time\t\tElapsed\tPressure\tVacuum";
    QTextStream in(&file);
    line = in.readLine();
    if (line != header) {
        QTextStream head(&file);
        head << header << "\n";
    }
}
WriteLogFile::~WriteLogFile(){

}

void WriteLogFile::writeLine(QString line){
    QFile file(QString("log-%1.txt").arg(QDate::currentDate().toString()));
    if (!file.open(QIODevice::Append | QIODevice::Text))
        return;
    QTextStream out(&file);
    out << QTime::currentTime().toString() << "\t";
    out << line << "\n";
}