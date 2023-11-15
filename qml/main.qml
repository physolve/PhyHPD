import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.settings
import QtQuick.Dialogs

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
        Item {
            width: 600
            height: 400
            CustomPlotItem {
                id: customPlot
                width: parent.width;  height: parent.height // resize
                Component.onCompleted: initCustomPlot(0)
                Component.onDestruction: testJSString(0)
                function testJSString(num) {
                    var text = "I have been destroyed_ %1"
                    console.log(text.arg(num))
                }
            }
            // Connections {
            //     target: dataView.model    // EDIT: I drew the wrong conclusions here, see text below!
            //     function onDataChanged() {
            //         customPlot.backendData(model.x, model.y)
            //         //gRAMsMnemoForm.setVal(model.cv, model.index) // it's Working
            //     }
            // }
            Connections {
                target: backend 
                onPointsChanged: {
                    customPlot.backendData(x, y)
                }
            }
        }
        // Loader {
        //     id: myLoader
        //     source: "qrc:/MHgrph/dataWindow.qml"
        // }

        // Connections {
        //     target: myLoader.item
        //     function onMessage(msg) { console.log(msg) }
        // }
    }

}