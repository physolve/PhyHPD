import QtQuick
//import QtQuick.Dialogs
import QtQuick.Controls
import QtQml.Models
import QtQuick.Layouts

Dialog {
    id: dialog
    //modal: false
    //focus: true
    standardButtons: Dialog.Apply
    width: 500
    height: 300
    function showPortInfo(index){
        let serialPortInfo = settingsDialog.serialPortList
        console.log(Object.entries(serialPortInfo))
        console.log(index)
        descriptionLabel.text = "Description: " + serialPortInfo[index][1]
        manufacturerLabel.text = "Manufacturer: " + serialPortInfo[index][2]
        serialNumberLabel.text = "Serial number: " + serialPortInfo[index][3]
        locationLabel.text = "Location: " + serialPortInfo[index][4]
        vidLabel.text = "Vendor Identifier: " + serialPortInfo[index][5]
        pidLabel.text = "Product Identifier: " + serialPortInfo[index][6]
    }
    RowLayout{
        anchors.fill: parent
        anchors.margins: 10
        spacing: 15
        ColumnLayout{
            Layout.fillWidth: true
            ComboBox{
                id: serialPorntInfoCmb
                Layout.fillWidth: true
                model: Object.keys(settingsDialog.serialPortList)
                Component.onCompleted: {
                    currentIndex = count - 1
                    showPortInfo(currentText)
                }
                onActivated: {
                    showPortInfo(currentText)
                }
            }
            Label{
                id: descriptionLabel
                text: "Description: "
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
            }
            Label{
                text: qsTr("Baud Rate:")
            }
            ComboBox{
                id: baudCombo
                Layout.fillWidth: true
                model: ["1200", "2400", "4800", "9600", 
                "19200", "38400", "57600", "115200"]
            }
            Label{
                text: qsTr("Data Bits:")
            }
            ComboBox{
                id: dataBitsCombo
                Layout.fillWidth: true
                valueRole: "value"
                model: ["5", "6", "7", "8"]
            }
            Label{
                text: qsTr("Stop Bits:")
            }
            ComboBox{
                id: stopBitsCombo
                Layout.fillWidth: true
                valueRole: "value"
                model: ["1", "3", "2"] // ?
            }
        }
        Item{
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
    }

    onApplied: {
        let result = {}
        result["serialPortInfo"] = serialPorntInfoCmb.currentText
        result["baudRate"] = baudCombo.currentValue
        result["dataBitsBox"] = dataBitsCombo.currentValue
        result["parityBox"] = parityCombo.currentValue
        result["stopBitsBox"] = stopBitsCombo.currentValue
        settingsDialog.apply(result)
    }
}