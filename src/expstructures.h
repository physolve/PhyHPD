#pragma once
#include <QObject>
struct accumulationPoint{
    double p_p;
    double p_s;
    double absTemp;
    qreal t;
    //double q;
};

struct expParameters{ // sample
    Q_GADGET
    // it might be linked to json for import and multi-result log
    Q_PROPERTY (QString nameOfSample MEMBER m_nameOfSample)
    Q_PROPERTY (double volume MEMBER m_volume)
    Q_PROPERTY (double thickness MEMBER m_thickness)
    Q_PROPERTY (double diameter MEMBER m_diameter)
public:
    QString m_nameOfSample;
    double m_volume; // m3
    double m_thickness; // m
    double m_diameter;
};

struct expTiming{
    Q_GADGET
public:    
    unsigned int m_leakStart; // second elapsed when valve open
    unsigned int m_leakEnd; // second elapsed after valve open
    unsigned int m_steadyStateStart;
    Q_PROPERTY (int leakStart MEMBER m_leakStart)
    Q_PROPERTY (unsigned int leakEnd MEMBER m_leakEnd)
    Q_PROPERTY (unsigned int steadyStateStart MEMBER m_steadyStateStart)
};

struct expInfo{
    Q_GADGET
public:
    QString m_expName; 
    unsigned int m_expStart; // to expTiming
    unsigned int m_expEnd; // to expTiming
    double m_temperature;
    bool isExpWorking;
    QString m_lastExpDataFile;
    QString m_currentExpDataFile;
    int m_expCount;
    Q_PROPERTY (QString chExpName MEMBER m_expName)
    Q_PROPERTY (QString lastExpDataFile MEMBER m_lastExpDataFile)
    Q_PROPERTY (QString currentExpDataFile MEMBER m_currentExpDataFile)
    Q_PROPERTY (int expCount MEMBER m_expCount)
    Q_PROPERTY (double expTemperature MEMBER m_temperature)

    Q_PROPERTY (int expStart MEMBER m_expStart)
    Q_PROPERTY (int expEnd MEMBER m_expEnd)

};

struct expResults{
    Q_GADGET
public:
    double m_estPermeability;
    double m_estDiffusivity;
    double m_timeLagVal;
    // move time Lag here
    // Q_PROPERTY (double estPermeability MEMBER m_estPermeability)
    // Q_PROPERTY (double estDiffusivity MEMBER m_estDiffusivity)
    Q_PROPERTY(double timeLagVal MEMBER m_timeLagVal) //READ timeLagVal NOTIFY timeLagValChanged
};
