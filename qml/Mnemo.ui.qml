import QtQuick 6.2
import QtQuick.Controls 6.2
//import MnemoHydroP
import QtQuick.Controls.Material

Rectangle {
    id: rectangle
    width: 1080
    height: 790
    color: "transparent"
    Connections {
        target: pressureBack 
        function onLastChanged(y) { slider1.value = y }
    }
    Connections {
        target: vacuumBack 
        function onLastChanged(y) { slider2.value = y }
    }
    //color: Constants.backgroundColor
    Material.theme: Material.Dark
    Material.accent: Material.Indigo
    Image {
        id: scheme
        width: 1072
        height: 785
        source: "qrc:/MHgrph/qml/scheme.jpg"
        fillMode: Image.PreserveAspectFit
    }
    RoundButton {
        id: valve1
        x: 680
        y: 320
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
    }
    RoundButton {
        id: valve2
        x: 680
        y: 444
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
    }
    RoundButton {
        id: valve3
        x: 680
        y: 444
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
    }
    RoundButton {
        id: valve4
        x: 680
        y: 561
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
    }
    RoundButton {
        id: valve5
        x: 354
        y: 561
        width: 46
        height: 46
        checkable: true
        Material.background: valve5.checked ? Material.Amber : Material.BlueGrey
        Material.elevation: 6
        Rectangle {
            anchors.centerIn: parent
            width: 36
            height: 36
            //Material.background: Material.Blue
            color: "transparent"
            radius: 18
            border.color: valve5.checked ? "#bac6cc" : "#4b90e4"
            border.width: 2
        }
    }

    Rectangle {
        id: sliderPlacer1
        x: 444
        y: 248
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
            value: 10
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
        x: 817
        y: 355
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
            value: 10
            maxValue: 1100
            endAngle: 320

            Label {
                width: 40
                height: 20
                color: "black"
                text: slider2.value.toFixed(2) // changing to exp function
                anchors.verticalCenter: parent.verticalCenter
                horizontalAlignment: Text.AlignHCenter
                anchors.horizontalCenter: parent.horizontalCenter
                rotation: 180
                font.pointSize: 13
            }
        }
    }
}
