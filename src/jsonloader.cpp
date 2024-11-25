#pragma once

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
    expList = currentJSONobj.keys();
    // QMap<int, QString> m;
    // for(auto s : jsonObject.keys()) // it is just sorting thing
    //     m[jsonObject[s].toObject()["profileId"].toInt()] = s;
    // m_profileNames = m.values();
    // m_profileJson = jsonObject.toVariantMap();
    return true;
}

QStringList JsonLoader::getExpList() const{
    return expList;
}

expParameters JsonLoader::getExpParameters(const QString &expName) const{
    const auto &expObj = currentJSONobj[expName].toObject();
    const auto &sampleObj = expObj["sample"].toObject();
    const QString &nameOfSample = sampleObj["sampleName_s"].toString();
    const double &sideVolume = sampleObj["sideVolume_m3"].toDouble();
    const double &thickness = sampleObj["thickness_mm"].toDouble();
    const double &diameter = sampleObj["diameter_m"].toDouble();
    return expParameters(nameOfSample, sideVolume, thickness, diameter);
}

expTiming JsonLoader::getExpTiming(const QString &expName) const{
    const auto &expObj = currentJSONobj[expName].toObject();
    const auto &leakStart = expObj["leakStart_s"].toInt();
    const auto &leakEnd = expObj["leakEnd_s"].toInt();
    const auto &steadyStateStart = expObj["steadyStateStart_s"].toInt();
    return expTiming(leakStart, leakEnd, steadyStateStart);
}

expInfo JsonLoader::getExpInfo(const QString &expName) const{
    const auto &expObj = currentJSONobj[expName].toObject();
    const auto &lastExpDataFile = expObj["lastExpDataFile_str"].toString();
    const auto &currentExpDataFile = expObj["currentExpDataFile_str"].toString();
    const auto &expCount = expObj["expCount_n"].toInt();
    const auto &expTemperature = expObj["temperature_C"].toDouble();
    // unsigned int m_expStart = 0
    // unsigned int m_expEnd = 0
    // bool isExpWorking = false;
    return expInfo(expName, 0, 0, expTemperature, false, lastExpDataFile, currentExpDataFile, expCount);
}

bool JsonLoader::checkLastSampleData(const QString &expName) const{
   const auto &expObj = currentJSONobj[expName].toObject();
   if (!expObj.contains("lastExpDataFile_str")) return false;
   else if (expObj["lastExpDataFile_str"].toString().isEmpty()) return false;
   else return true;
}

QString JsonLoader::getSampleDataFile(const QString &expName) const{
    const auto &expObj = currentJSONobj[expName].toObject();
    return expObj["lastExpDataFile_str"].toString();
}

bool JsonLoader::checkNewName(const QString &expName){
    return !currentJSONobj.contains(expName); // return true if not have this key
}

bool JsonLoader::addNewSample(const expInfo &info, const expParameters &params, const expTiming &timing){
    bool newSample = !currentJSONobj.contains(info.m_expName);
    QJsonObject expObj;
    QJsonObject sampleObj;
    sampleObj["sampleName_s"] = params.m_nameOfSample;
    sampleObj["sideVolume_m3"] = params.m_volume;
    sampleObj["thickness_mm"] = params.m_thickness;
    sampleObj["diameter_m"] = params.m_diameter;

    expObj["sample"] = sampleObj;

    expObj["leakStart_s"] = (int)timing.m_leakStart - (int)round(info.m_expStart);
    expObj["leakEnd_s"] = (int)timing.m_leakEnd - (int)round(info.m_expStart);
    expObj["steadyStateStart_s"] = (int)timing.m_steadyStateStart - (int)round(info.m_expStart);

    expObj["expCount_n"] = (int)info.m_expCount+1;
    expObj["lastExpDataFile_str"] = info.m_currentExpDataFile; // grab from base sample
    expObj["currentExpDataFile_str"] = "";
    expObj["temperature_C"] = info.m_temperature;

    // QString saveExpName //.arg(info.m_expCount+1);
    currentJSONobj[info.m_expName] = expObj;

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