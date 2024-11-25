#pragma once

#include <QFile>

class WriteLogFile : public QObject
{
	Q_OBJECT

public:
	WriteLogFile(QObject *parent = 0);
	~WriteLogFile();
	
public slots:
    void writeLine(const QString &line);
private:
	QString fileName;
	QFile file;
};