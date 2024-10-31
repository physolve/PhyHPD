import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
Item{
    anchors.fill: parent
    
    TabBar{
        id: tabBar
        anchors.top: parent.top
        width: parent.width
        // anchors.left: parent.left
        // anchors.right: parent.right
        Repeater{
            id: barGraphRepeater
            model: ["Pressure", "Vacuum", "Flux", "Diffusivity", "Permeation"]
            TabButton{
                text: modelData
                width: Math.max(100, tabBar.width/5)
                font.pointSize: 12
            }
        }
    }
    StackLayout{
        id: tabLayout
        currentIndex: tabBar.currentIndex
        anchors.top: tabBar.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.topMargin: 20
        anchors.bottomMargin: 80

        Connections {
            target: pressureBack // backend?
            function onPressureChanged() { 
                timePressure.forceUpdate()
                timeVacuum.forceUpdate()
            }
        }
        CustomChart{
            id: timePressure
            plotName: "timePressure"
            isExp: false
            m_index: 0
            label: "Давление, бар"
            
        }
        CustomChart{
            id: timeVacuum
            plotName: "timeVacuum"
            isExp: false
            m_index: 0
            label: "Давление, бар"
        }
        Connections {
            target: expCalc // backend?
            function onResultChanged() { 
                timeFluxPlot.forceUpdate()
                timeDiffModelDiff.forceUpdate()
                timePermeation.forceUpdate()
            }
        }
        CustomChart{
            id: timeFluxPlot
            plotName: "timeFlux"
            isExp: true
            m_index: 0
            label: "Поток"
        }
        CustomChart{
            id: timeDiffModelDiff
            plotName: "timeDiffModelDiff"
            isExp: true
            m_index: 0
            label: "Диффузия"
        }
        CustomChart{
            id: timePermeation
            plotName: "timePermeation"
            isExp: true
            m_index: 0
            label: "Проницаемость"
        }
    }

}