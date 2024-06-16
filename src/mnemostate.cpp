#include "mnemostate.h"

MnemoState::MnemoState(QObject *parent) : QObject(parent)
{
    QStringList valveNames = {"vacuum", "bypass", "supply", "chamber"};
    for(auto valveName : valveNames){
        m_valves.insert(valveName, Valve(valveName));
    }
}

void MnemoState::setValveState(const QString & valveName, const bool &valveState){
    m_valves[valveName].setState(valveState);
}

bool MnemoState::getValveState(const QString & valveName){
    return m_valves[valveName].m_state;
}