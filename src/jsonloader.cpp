#include "jsonloader.h"

#include <QDir>
#include <QFile>
#include <QJsonDocument>

JsonLoader::JsonLoader(QObject *parent) :
    QObject(parent)
{
    bool checkPass = false;
    QString m_rawData;
    checkPass = readProfile(m_rawData);
    checkPass = jsonParser(m_rawData);
}

JsonLoader::~JsonLoader(){

}

bool JsonLoader::readProfile(QString &rawData){
    QDir dir("data");
    if(!dir.exists()) return false;
    QFile file;
    file.setFileName(dir.filePath("pseudoSampleReport.json"));
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) return false;
    
    rawData = file.readAll();
    file.close();
    return true;
}

bool JsonLoader::jsonParser(const QString &rawData){
    QJsonDocument document = { QJsonDocument::fromJson(rawData.toUtf8()) };
    currentJSONobj = document.object();
    samplesList = currentJSONobj.keys();
    // QMap<int, QString> m;
    // for(auto s : jsonObject.keys()) // it is just sorting thing
    //     m[jsonObject[s].toObject()["profileId"].toInt()] = s;
    // m_profileNames = m.values();
    // m_profileJson = jsonObject.toVariantMap();
    return true;
}

QStringList JsonLoader::getSamplesList() const{
    return samplesList;
}

expParameters JsonLoader::getSampleExpParameters(const QString &sampleName) const{
    const auto &expObj = currentJSONobj[sampleName].toObject();
    const auto &sampleObj = expObj["sample"].toObject();
    const double &sideVolume = sampleObj["sideVolume_m3"].toDouble();
    const double &thickness = sampleObj["thickness_mm"].toDouble();
    const double &diameter = sampleObj["diameter_m"].toDouble();
    return expParameters(sideVolume, thickness, diameter);
}

expTiming JsonLoader::getExpTiming(const QString &sampleName) const{
    const auto &expObj = currentJSONobj[sampleName].toObject();
    const auto &leakStart = expObj["leakStart_s"].toInt();
    const auto &leakEnd = expObj["leakEnd_s"].toInt();
    const auto &steadyStateStart = expObj["steadyStateStart_s"].toInt();
    return expTiming(leakStart, leakEnd, steadyStateStart);
}

bool JsonLoader::checkNewName(const QString &sampleName){
    return !currentJSONobj.contains(sampleName); // return true if not have this key
}

bool JsonLoader::addNewSample(const QString &sampleName, const expParameters &params, const expTiming &timing){
    bool newSample = !currentJSONobj.contains(sampleName);
    QJsonObject expObj;
    QJsonObject sampleObj;
    sampleObj["sideVolume_m3"] = params.m_volume;
    sampleObj["thickness_mm"] = params.m_thickness;
    sampleObj["diameter_m"] = params.m_diameter;
    expObj["sample"] = sampleObj;
    expObj["leakStart_s"] = (int)timing.m_leakStart;
    expObj["leakEnd_s"] = (int)timing.m_leakEnd;
    expObj["steadyStateStart_s"] = (int)timing.m_steadyStateStart;
    expObj["sampleName_s"] = sampleName;
    currentJSONobj[sampleName] = expObj;

    QDir dir("data");
    if(!dir.exists()) return false;
    QFile saveFile;
    saveFile.setFileName(dir.filePath("pseudoSampleReport.json"));
    if(!saveFile.open(QIODevice::WriteOnly | QIODevice::Text)) return false;
    saveFile.write(QJsonDocument(currentJSONobj).toJson());
    saveFile.close();
    if(newSample){
        bool checkPass = false;
        QString m_rawData;
        checkPass = readProfile(m_rawData);
        checkPass = jsonParser(m_rawData);
    }

    return newSample;
}