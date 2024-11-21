#pragma once

#include <QJsonObject>
#include <QList>

#include "expstructures.h"

class JsonLoader : public QObject
{
    Q_OBJECT
public:
    JsonLoader(QObject *parent = 0);
    ~JsonLoader();
    bool readProfile(QString& rawData);
    bool jsonParser(const QString &rawData);
    QStringList getExpList() const;
    expParameters getExpParameters(const QString &expName) const;
    expTiming getExpTiming(const QString &expName) const;
    expInfo getExpInfo(const QString &expName) const;

    QString getSampleDataFile(const QString &expName) const;
    bool checkLastSampleData(const QString &expName) const;

    bool checkNewName(const QString &expName);
    bool addNewSample(const expInfo &info, const expParameters &params, const expTiming &timing);
private:
    QStringList expList;
    QJsonObject currentJSONobj;
};