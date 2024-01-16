import QtQuick
//import QtQuick.Dialogs
import QtQuick.Controls
import QtQml.Models
import QtQuick.Layouts

Window {
    id: dialog
    title: qsTr("Settings")
    flags: Qt.Dialog
    //modality: Qt.WindowModal
    Material.theme: Material.Dark
    Material.accent: Material.Indigo
    color: "#303030"
    //color: active ? palette.active.window : palette.inactive.window 
    //palette.active.window: "peachpuff"
    //palette.windowText: "brown"
    width: 500
    height: 300
    required property string c_name

    Component.onCompleted:{
        let serialPortInfo = settingsDialog.serialPortList[c_name]
        if(serialPortInfo === undefined) {
            return;
        }
        descriptionLabel.text = "Description: " + serialPortInfo[1]
        manufacturerLabel.text = "Manufacturer: " + serialPortInfo[2]
        serialNumberLabel.text = "Serial number: " + serialPortInfo[3]
        locationLabel.text = "Location: " + serialPortInfo[4]
        vidLabel.text = "Vendor Identifier: " + serialPortInfo[5]
        pidLabel.text = "Product Identifier: " + serialPortInfo[6]
    }
    // CHECK if current Values from settings is set up
    RowLayout{
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 10
        spacing: 15
        ColumnLayout{
            Layout.fillWidth: true
            // ComboBox{
            //     id: serialPorntInfoCmb
            //     Layout.fillWidth: true
            //     model: Object.keys(settingsDialog.serialPortList)
            //     Component.onCompleted: {
            //         currentIndex = count - 1
            //         showPortInfo(currentText)
            //     }
            //     onActivated: {
            //         showPortInfo(currentText)
            //     }
            // }
            Label{
                id: nameLabel
                text: c_name
            }
            Label{
                id: descriptionLabel
                text: "Description: "
                Layout.preferredWidth: dialog.width/2
                elide: Text.ElideRight
            }
            Label{
                id: manufacturerLabel
                text: "Manufacturer: "
            }
            Label{
                id: serialNumberLabel
                text: "Serial number: "
            }
            Label{
                id: locationLabel
                text: "Location: "
            }
            Label{
                id: vidLabel
                text: "Vendor ID: "
            }
            Label{
                id: pidLabel
                text: "Product ID: "
            }
        }
        GridLayout{
            columns: 2
            Layout.fillWidth: true
            Label{
                text: qsTr("Parity:")
            }
            ComboBox{
                id: parityCombo
                Layout.fillWidth: true
                model: ["No", "Even", "Odd", "Space", "Mark"]
                currentIndex: 0
            }
            Label{
                text: qsTr("Baud Rate:")
            }
            ComboBox{
                id: baudCombo
                Layout.fillWidth: true
                model: ["1200", "2400", "4800", "9600", 
                "19200", "38400", "57600", "115200"]
                currentIndex: 3
            }
            Label{
                text: qsTr("Data Bits:")
            }
            ComboBox{
                id: dataBitsCombo
                Layout.fillWidth: true
                valueRole: "value"
                model: ["5", "6", "7", "8"]
                currentIndex: 3
            }
            Label{
                text: qsTr("Stop Bits:")
            }
            ComboBox{
                id: stopBitsCombo
                Layout.fillWidth: true
                valueRole: "value"
                model: ["1", "3", "2"] // ?
                currentIndex: 0
            }
        }
        Item{
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
    }
    Button{
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.margins: 10
        text: qsTr("Apply")
        onClicked: {
            let result = {}
            result["serialPortInfo"] = serialPorntInfoCmb.currentText
            result["baudRate"] = baudCombo.currentText
            result["dataBitsBox"] = dataBitsCombo.currentText
            result["parityBox"] = parityCombo.currentIndex
            result["stopBitsBox"] = stopBitsCombo.currentText
            settingsDialog.apply(result, c_name)
            dialog.close()
        }
    }
}

    // function showPortInfo(){ //index
    //     let serialPortInfo = settingsDialog.serialPortList[c_name]
    //     //console.log(Object.entries(serialPortInfo))
    //     //console.log(index)
    //     descriptionLabel.text = "Description: " + serialPortInfo[1]
    //     manufacturerLabel.text = "Manufacturer: " + serialPortInfo[2]
    //     serialNumberLabel.text = "Serial number: " + serialPortInfo[3]
    //     locationLabel.text = "Location: " + serialPortInfo[4]
    //     vidLabel.text = "Vendor Identifier: " + serialPortInfo[5]
    //     pidLabel.text = "Product Identifier: " + serialPortInfo[6]
    // }