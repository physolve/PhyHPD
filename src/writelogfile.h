#pragma once

#include <QObject>
#include <QFileDialog>
#include <QDir>
class WriteLogFile : public QObject
{
	Q_OBJECT

public:
	WriteLogFile(QObject *parent = 0);
	~WriteLogFile();
public slots:
    void writeLine(QString line);
};