import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
//import Qt.labs.settings

import CustomPlot
ApplicationWindow {
    id: main
    width: 1400
    height: 800
    visible: true
    title: qsTr("MHgrph")
    Material.theme: Material.Dark
    Material.accent: Material.Indigo
    GridLayout {
        id: grid
        anchors.fill: parent
        columnSpacing: 1
        flow: GridLayout.LeftToRight
        columns: 2
        Item {
            width: 700
            height: parent.height
            DataWindow{
            }
        }
        ColumnLayout{
            Item {
                width: 600
                height: 400
                CustomPlotItem {
                    id: customPlotPressure
                    width: parent.width;  height: parent.height // resize
                    Component.onCompleted: initCustomPlot(0)
                    Component.onDestruction: testJSString(0)
                    function testJSString(num) {
                        var text = "I have been destroyed_ %1"
                        console.log(text.arg(num))
                    }
                }
                Connections {
                    target: pressureBack 
                    onPointsChanged: {
                        customPlotPressure.backendData(x, y)
                    }
                }
            }
            Item {
                width: 600
                height: 400
                CustomPlotItem {
                    id: customPlotVacuum
                    width: parent.width;  height: parent.height // resize
                    Component.onCompleted: initCustomPlot(0)
                    Component.onDestruction: testJSString(0)
                    function testJSString(num) {
                        var text = "I have been destroyed_ %1"
                        console.log(text.arg(num))
                    }
                }
                Connections {
                    target: vacuumBack 
                    onPointsChanged: {
                        customPlotVacuum.backendData(x, y)
                    }
                }
            }
        }
        
    }

}