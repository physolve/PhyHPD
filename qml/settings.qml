import QtQuick
import QtQuick.Controls
import QtQml.Models
import QtQuick.Layouts

Dialog {
    id: dialog
    modal: false
    standardButtons: Dialog.Apply

    ColumnLayout{
        anchors.fill: parent
        spacing: 5
        
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

            Label{
                text: qsTr("Response Timeout:")
            }
            SpinBox {
                id: timeoutSpinner
                value: 200
                stepSize: 20
                displayText: String(timeoutSpinner.value) + " ms"
                from: 10
                to: 5000
            }
            Label{
                text: qsTr("Number of retries:")
            }
            SpinBox {
                id: retriesSpinner
                value: 3
                stepSize: 1
                from: 0
                to: 99
            }
        }

        Item{
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
    }

    onApplied: {
        backend.mySettings.parity = parityCombo.currentIndex
        if(parityCombo.currentIndex>0)
            backend.mySettings.parity++
        backend.mySettings.baud = parityCombo.currentIndex
        backend.mySettings.dataBits = dataBitsCombo.currentValue
        backend.mySettings.stopBits = stopBitsCombo.currentValue
        backend.mySettings.responseTime = timeoutSpinner.value
        backend.mySettings.numberOfRetries = m_numberOfRetries.value
    }
}