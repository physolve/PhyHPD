#pragma once

#include <QDebug>

struct Valve{
    QString m_name = "";
    bool m_state = false;
    void setState(bool s){
        m_state = s;
    }
    bool getState(){
       return m_state; 
    }
    Valve(const QString &name = "unknown", const bool &state = false): m_name(name), m_state(state) { }
};


class MnemoState : public QObject
{
    Q_OBJECT
public:

    explicit MnemoState(QObject *parent = nullptr);

    Q_INVOKABLE void setValveState(const QString & valveName, const bool &valveState);
    Q_INVOKABLE bool getValveState(const QString & valveName);

private:

    QMap<QString, Valve> m_valves;
};