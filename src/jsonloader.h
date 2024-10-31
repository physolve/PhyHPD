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
    QStringList getSamplesList() const;
    expParameters getSampleExpParameters(const QString &sampleName) const;
    expTiming getExpTiming(const QString &sampleName) const;
    bool checkNewName(const QString &sampleName);
    bool addNewSample(const QString &sampleName, const expParameters &params, const expTiming &timing);
private:
    QStringList samplesList;
    QJsonObject currentJSONobj;
};