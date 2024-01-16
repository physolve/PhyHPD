import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item{
    width: parent.width
    height: parent.height
    //anchors.fill: parent
    ColumnLayout{
        anchors.centerIn: parent
        spacing: 10
        Button{
            id: connectBtn
            text: connectBtn.checked ? qsTr("Disconnect") : qsTr("Connect")
            checkable: true
            onClicked: {
                connectBtn.checked ? backend.openSerialPort() : backend.closeSerialPort()
            }
        }
        ListView {
            Layout.fillWidth: true
            model: backend.data
            implicitHeight: 240
            delegate: Rectangle{
                color: "lightgray"
                height: 24
                Text {
                    text: modelData
                    color: "white"
                    font.pointSize: 14
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }
        RowLayout{
            Button{
                id: configurePressureBtn
                text: qsTr("Configure pressure")
                onClicked: {
                    pressureDialog.show()
                }
            }
            Button{
                id: configureVacuumBtn
                text: qsTr("Configure vacuum")
                onClicked: {
                    vacuumDialog.show()
                }
            }
        }
        Switch {
            id: startBtn
            text: qsTr("Start Read")
            font.pointSize: 14
            onToggled: backend.onReadButtonClicked(startBtn.checked)
        }
        Label { // move to log view
            id: logTextDisplay
            color: "black"
            text: backend.logText
            font.pointSize: 14
            padding: 15
            background: Rectangle{
                radius: 20
                color: "yellow"
            }
        }        
    }
    Item{    
        SettingsDialog{
            id: pressureDialog
            c_name: "pressure"
            //padding: 0
            //anchors. centerIn: parent
            //x: 100
            //y: 500
        }
    }
    Item{    
        SettingsDialog{
            id: vacuumDialog
            c_name: "vacuum"
            //padding: 0
            //anchors. centerIn: parent
            //x: 100
            //y: 500
        }
    }
}