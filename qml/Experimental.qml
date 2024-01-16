import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

GroupBox {
    title: "Diffusion and Permeation"
    id: base
    width: parent.width
    height: parent.height
    ScrollView {
        anchors.fill: parent
        anchors.margins: 10
        contentWidth: gridExp.width    // The important part
        contentHeight: gridExp.height + calcCol.height + 10  // Same
        clip : true                   // Prevent drawing column outside the scrollview borders
        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
        ScrollBar.vertical.policy: ScrollBar.AsNeeded
        GridLayout{
            id: gridExp
            width: base.width - 40
            anchors.top: parent.top
            anchors.left: parent.left
            implicitHeight: 400
            flow: GridLayout.LeftToRight
            columns: base.width/220
            rowSpacing: 20
            Layout.preferredWidth: 200 
            Row{
                spacing: 10
                Label{
                    width: 80
                    text: "Log start"
                    font.pointSize: 12
                    anchors.verticalCenter: parent.verticalCenter
                }
                TextField{
                    id: logStart
                    width: 80
                    text : "00:00:00"
                    font.pointSize: 10
                    inputMask: "99:99:99"
                    inputMethodHints: Qt.ImhTime//Qt.ImhDigitsOnly
                    anchors.verticalCenter: parent.verticalCenter
                    horizontalAlignment: TextInput.AlignHCenter
                }
            }
            Row{
                spacing: 10
                Label{
                    width: 80
                    text: "Valve open"
                    font.pointSize: 12
                    anchors.verticalCenter: parent.verticalCenter
                }
                TextField{
                    id: valveOpen
                    width: 80
                    text : "00:00:00.000"
                    font.pointSize: 10
                    inputMask: "99:99:99.999"
                    inputMethodHints: Qt.ImhTime
                    anchors.verticalCenter: parent.verticalCenter
                    horizontalAlignment: TextInput.AlignHCenter
                }
            }
            Row{
                spacing: 10
                Label{
                    width: 80
                    text: "Valve close"
                    font.pointSize: 12
                    anchors.verticalCenter: parent.verticalCenter
                }
                TextField{
                    id: valveClose
                    width: 80
                    text : "00:00:00.000"
                    font.pointSize: 10
                    inputMask: "99:99:99.999"
                    inputMethodHints: Qt.ImhTime
                    anchors.verticalCenter: parent.verticalCenter
                    horizontalAlignment: TextInput.AlignHCenter
                }
            }
            Row{
                //Layout.preferredWidth: 160 
                spacing: 5
                Label{
                    width: 60
                    text: "Thickness"
                    font.pointSize: 12
                    anchors.verticalCenter: parent.verticalCenter
                }
                TextField{
                    id: thickness
                    width: 60
                    text : "50.000"
                    font.pointSize: 10
                    validator: DoubleValidator { bottom: 0.001; top: 10000; decimals: 3}
                    anchors.verticalCenter: parent.verticalCenter
                    horizontalAlignment: TextInput.AlignHCenter
                }
                ComboBox{
                    id: thickDimension
                    width: 100
                    height: 40
                    model: ["micron", "mm", "cm"]
                    font.pointSize: 10
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
            Row{
                //Layout.preferredWidth: 160 
                spacing: 10
                Label{
                    width: 80
                    text: "Diffusion @"
                    font.pointSize: 12
                    anchors.verticalCenter: parent.verticalCenter
                }
                ComboBox{
                    id: diffusDimension
                    width: 100
                    height: 40
                    model: ["m²/s", "cm²/s"]
                    font.pointSize: 10
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
            Row{
                //Layout.preferredWidth: 160 
                spacing: 5
                Button{
                    width: 120
                    text: "Log path"
                    font.pointSize: 10
                }
                TextField{
                    id: logPath
                    width: 80
                    text : ""
                    horizontalAlignment: TextInput.AlignHCenter
                }
            }
            Row{
                //Layout.preferredWidth: 160 
                spacing: 5
                Button{
                    width: 120
                    text: "Spectrum path"
                    font.pointSize: 10
                }
                TextField{
                    id: spectrumPath
                    width: 80
                    text : ""
                    horizontalAlignment: TextInput.AlignHCenter
                }
            }
            Row{
                //Layout.preferredWidth: 160 
                spacing: 5
                Label{
                    width: 100
                    text: "Now phase"
                    font.pointSize: 12
                    anchors.verticalCenter: parent.verticalCenter
                }
                Button{
                    id: experimentState
                    width: 120
                    text : "Diffusion"
                    font.pointSize: 10
                }
            }
        }
        ColumnLayout{
            id: calcCol
            width: base.width - 40
            anchors.top: gridExp.bottom
            anchors.left: parent.left
            implicitHeight: 200
            spacing: 10
            //Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            //Layout.fillWidth: true;
            //Layout.fillHeight: false;
            Row{
                spacing: 10
                Button{
                    width: 120
                    text: "Calculate"
                    font.pointSize: 10
                }
                TextField{
                    id: curSeconds
                    width: 40
                    text : avgSlider.value
                    horizontalAlignment: TextInput.AlignHCenter
                }
                Slider{
                    id: avgSlider
                    width: 200
                    from: 0
                    value: 25
                    to: 100
                    stepSize: 10
                    snapMode: Slider.SnapOnRelease
                    background: Rectangle {
                        x: avgSlider.leftPadding
                        y: avgSlider.topPadding + avgSlider.availableHeight / 2 - height / 2
                        implicitWidth: 200
                        implicitHeight: 4
                        width: avgSlider.availableWidth
                        height: implicitHeight
                        radius: 2
                        color: "#bdbebf"
                        Repeater {
                            model: (parent.to - parent.from) / parent.stepSize + 1
                            delegate: Column {
                                x: index * parent.width / (parent.to - parent.from) * parent.stepSize - width / 2
                                y: 0
                                spacing: 2
                                Rectangle {
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    width: 1
                                    height: 10
                                    color: "grey"
                                }
                                Text {
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    text: index * parent.stepSize + parent.from
                                }
                            }
                        }
                        Rectangle {
                            width: avgSlider.visualPosition * parent.width
                            height: parent.height
                            color: "#21be2b"
                            radius: 2
                        }
                    }

                    handle: Rectangle {
                        x: avgSlider.leftPadding + avgSlider.visualPosition * (avgSlider.availableWidth - width)
                        y: avgSlider.topPadding + avgSlider.availableHeight / 2 - height / 2
                        implicitWidth: 26
                        implicitHeight: 26
                        radius: 13
                        color: avgSlider.pressed ? "#f0f0f0" : "#f6f6f6"
                        border.color: "#bdbebf"
                    }
                }
            }
            TextArea {
                id: resultText
                //Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
                placeholderText: qsTr("Enter description")
                text: table_model.resultStr
            }
            TableView {
                id: tableView
                Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
                implicitWidth: 500
                height: 250
                columnWidthProvider: function (column) { return 140; }
                rowHeightProvider: function (column) { return 60; }
                leftMargin: rowsHeader.implicitWidth
                topMargin: columnsHeader.implicitHeight
                model: table_model
                ScrollBar.horizontal: ScrollBar{}
                ScrollBar.vertical: ScrollBar{}
                clip: true
                delegate: Rectangle {
                    Text {
                        text: display
                        anchors.fill: parent
                        anchors.margins: 10
                        color: 'black'
                        font.pixelSize: 15
                        verticalAlignment: Text.AlignVCenter
                    }
                }
                Rectangle { // mask the headers
                    z: 3
                    color: "#222222"
                    y: tableView.contentY
                    x: tableView.contentX
                    width: tableView.leftMargin
                    height: tableView.topMargin
                }

                Row {
                    id: columnsHeader
                    y: tableView.contentY
                    z: 2
                    Repeater {
                        model: tableView.columns > 0 ? tableView.columns : 1
                        Label {
                            width: tableView.columnWidthProvider(modelData)
                            height: 35
                            text: table_model.headerData(modelData, Qt.Horizontal)
                            color: '#aaaaaa'
                            font.pixelSize: 15
                            padding: 10
                            verticalAlignment: Text.AlignVCenter

                            background: Rectangle { color: "#333333" }
                        }
                    }
                }
                Column {
                    id: rowsHeader
                    x: tableView.contentX
                    z: 2
                    Repeater {
                        model: tableView.rows > 0 ? tableView.rows : 1
                        Label {
                            width: 60
                            height: tableView.rowHeightProvider(modelData)
                            text: table_model.headerData(modelData, Qt.Vertical)
                            color: '#aaaaaa'
                            font.pixelSize: 15
                            padding: 10
                            verticalAlignment: Text.AlignVCenter

                            background: Rectangle { color: "#333333" }
                        }
                    }
                }

                ScrollIndicator.horizontal: ScrollIndicator { }
                ScrollIndicator.vertical: ScrollIndicator { }
            }
        }
    }
}

/*
    ButtonGroup {
        id: btnGroup
        buttons: column.children
        exclusive: false
    }

    Column {
        id: column
        Button { text: "First" ; checkable: true }
        Button { text: "Second"; checkable: true }
        Button { text: "Third" ; checkable: true }
    }
    Button{
        anchors.bottom: parent.bottom
        text: "Apply"
        onClicked: console.log("Pressed: ", btnGroup.checkState)
    }
*/
