import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import QtQuick.Dialogs
import QtQuick.Controls.Material 2.12

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
            Material.roundedScale: Material.ExtraSmallScale
            onClicked: {
                connectBtn.checked ? backend.openSerialPort() : backend.closeSerialPort()
            }
        }
        Button{
            id: testPressValve
            // width: 120
            text : "Test to press valve"
            font.pointSize: 11
            Material.roundedScale: Material.ExtraSmallScale
            onClicked: askToPress.open()
        }
        MessageDialog{
            id: askToPress
            buttons: MessageDialog.Ok
            modality: Qt.NonModal
            text: "Press Vacuum valve"
            onAccepted: if(!mnemostate.getValveState("vacuum")) askToPress.open()// check the state of valve
        }
        // ListView {
        //     Layout.fillWidth: true
        //     model: backend.data //backend? it somehow worked with backend
        //     implicitHeight: 240
        //     delegate: Rectangle{
        //         color: "lightgray"
        //         height: 24
        //         Text {
        //             text: modelData
        //             color: "white"
        //             font.pointSize: 14
        //             horizontalAlignment: Text.AlignHCenter
        //             verticalAlignment: Text.AlignVCenter
        //         }
        //     }
        // }
        Button{
            id: configurePressureBtn
            text: qsTr("Configure pressure")
            font.pointSize: 11
            Material.roundedScale: Material.ExtraSmallScale
            onClicked: {
                pressureDialog.show()
            }
        }
        Switch {
            id: startBtn
            text: qsTr("Start Read")
            font.pointSize: 14
            Material.roundedScale: Material.NotRounded
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
    SettingsDialog{
        id: pressureDialog
        c_name: "pressure"
        // transientParent: parent
    }
}