import QtQuick 6.2
import QtQuick.Controls 6.2
//import MnemoHydroP
import QtQuick.Controls.Material

Rectangle {
    id: rectangle
    width: 1080
    height: 760
    color: "transparent"

    function setPres(presValue){
        slider1.value = presValue
    }
    function setVacuum(vacValue){
        slider2.value = vacValue
    }
    function setTime(timeValue){
        console.log(timeValue)
    }

    Connections{
        target: dataModel
        function onDataChanged(topLeft, bottomRight, roles){
            setPres(dataModel.data(dataModel.index(bottomRight.row, 0), 258)) //fine for now
            setVacuum(dataModel.data(dataModel.index(bottomRight.row, 0), 259)) //fine for now
        }
    }

    // Connections {
    //     target: dataModel
    //     function onDataChanged(topLeft, bottomRight, roles) {
    //         let valuesArray = {}
    //         for(let i = 0; i < bottomRight.row; i++){
    //             valuesArray[dataModel.data(dataModel.index(i,0),256)] = dataModel.data(dataModel.index(i,0),260) // shit models
    //         }
    //         setPresValues(valuesArray) // there all (fuck)
    //     }
    // }
    //set-Pres-Values

    function sendValveToBack(name, checked){
        mnemostate.setValveState(name, checked)
    }
    //color: Constants.backgroundColor
    Material.theme: Material.Dark
    Material.accent: Material.Indigo
    Image {
        id: scheme
        x: 0
        y: 0
        width: 1080
        height: 760
        source: "qrc:/MHgrph/qml/images/scheme_1.jpg"
        fillMode: Image.PreserveAspectFit
    }
    RoundButton {
        id: valve1
        x: 400
        y: 556
        width: 46
        height: 46
        checkable: true
        Material.background: valve1.checked ? Material.Amber : Material.BlueGrey
        Material.elevation: 6
        Rectangle {
            anchors.centerIn: parent
            width: 36
            height: 36
            //Material.background: Material.Blue
            color: "transparent"
            radius: 18
            border.color: valve1.checked ? "#bac6cc" : "#4b90e4"
            border.width: 2
        }
        property string name: "vacuum"
        onToggled: sendValveToBack(name, checked)
    }
    RoundButton {
        id: valve2
        x: 669
        y: 556
        width: 46
        height: 46
        checkable: true
        Material.background: valve2.checked ? Material.Amber : Material.BlueGrey
        Material.elevation: 6
        Rectangle {
            anchors.centerIn: parent
            width: 36
            height: 36
            //Material.background: Material.Blue
            color: "transparent"
            radius: 18
            border.color: valve2.checked ? "#bac6cc" : "#4b90e4"
            border.width: 2
        }
        property string name: "bypass"
        onToggled: sendValveToBack(name, checked)
    }
    RoundButton {
        id: valve3
        x: 669
        y: 451
        width: 46
        height: 46
        checkable: true
        Material.background: valve3.checked ? Material.Amber : Material.BlueGrey
        Material.elevation: 6
        Rectangle {
            anchors.centerIn: parent
            width: 36
            height: 36
            //Material.background: Material.Blue
            color: "transparent"
            radius: 18
            border.color: valve3.checked ? "#bac6cc" : "#4b90e4"
            border.width: 2
        }
        property string name: "supply"
        onToggled: sendValveToBack(name, checked)
    }
    RoundButton {
        id: valve4
        x: 672
        y: 326
        width: 46
        height: 46
        checkable: true
        Material.background: valve4.checked ? Material.Amber : Material.BlueGrey
        Material.elevation: 6
        Rectangle {
            anchors.centerIn: parent
            width: 36
            height: 36
            //Material.background: Material.Blue
            color: "transparent"
            radius: 18
            border.color: valve4.checked ? "#bac6cc" : "#4b90e4"
            border.width: 2
        }
        property string name: "chamber"
        onToggled: sendValveToBack(name, checked)
    }

    Rectangle {
        id: sliderPlacer1
        x: 763
        y: 365
        width: 90
        height: 90
        radius: 45
        color: "lightgray"

        CircularSlider {
            anchors.centerIn: parent
            id: slider1 // pressure
            handle: Rectangle {
                id: handleItem1
                width: 6
                height: 6
                color: "#908990"
                radius: width / 2
                border.color: "#fefefe"
                border.width: 5
                antialiasing: true
                transform: [
                    Translate {
                        x: (slider1.handleWidth - width) / 2
                        y: (slider1.handleHeight - height) / 2
                    }
                ]
            }
            minValue: 0
            rotation: 180
            progressWidth: 10
            startAngle: 40
            //Layout.fillWidth: false
            trackWidth: 10
            value: 5
            maxValue: 10
            endAngle: 320

            Label {
                width: 40
                height: 20
                color: "black"
                text: slider1.value.toFixed(3) // changing to exp function
                anchors.verticalCenter: parent.verticalCenter
                horizontalAlignment: Text.AlignHCenter
                anchors.horizontalCenter: parent.horizontalCenter
                rotation: 180
                font.pointSize: 13
            }
        }
    }

    Rectangle {
        id: sliderPlacer2
        x: 462
        y: 248
        width: 90
        height: 90
        color: "#d3d3d3"
        radius: 45

        CircularSlider {
            anchors.centerIn: parent
            id: slider2 // vacuum
            handle: Rectangle {
                id: handleItem2
                width: 6
                height: 6
                color: "#908990"
                radius: width / 2
                border.color: "#fefefe"
                border.width: 5
                antialiasing: true
                transform: [
                    Translate {
                        x: (slider2.handleWidth - width) / 2
                        y: (slider2.handleHeight - height) / 2
                    }
                ]
            }
            minValue: 0
            rotation: 180
            progressWidth: 10
            startAngle: 40
            //Layout.fillWidth: false
            trackWidth: 10
            value: 1.5
            maxValue: 2
            endAngle: 320

            Label {
                width: 40
                height: 20
                color: "black"
                text: slider2.value.toFixed(3) // changing to exp function
                anchors.verticalCenter: parent.verticalCenter
                horizontalAlignment: Text.AlignHCenter
                anchors.horizontalCenter: parent.horizontalCenter
                rotation: 180
                font.pointSize: 13
            }
        }
    }
}
