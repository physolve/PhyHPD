import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

GroupBox {
    title: "Experimantal results"
    id: base
    width: parent.width
    height: parent.height
    ScrollView{
        anchors.fill: parent
        anchors.margins: 10
        contentWidth: gridResult.width + 10
        contentHeight: gridResult.height + 10  //+ calcCol.height Same
        clip: true
        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
        ScrollBar.vertical.policy: ScrollBar.AsNeeded
        GridLayout {
            id: gridResult
            width: base.width - 40
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.topMargin: 10
            implicitHeight: 200
            flow: GridLayout.LeftToRight
            columns: base.width/220
            rowSpacing: 20
            Layout.preferredWidth: 200 
            Row{
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
                    font.pointSize: 11
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
            Row{
                spacing: 10
                Label{
                    width: 140
                    text: "Time lag result"
                    font.pointSize: 12
                    anchors.verticalCenter: parent.verticalCenter
                }
                TextField{
                    id: timeLagVal
                    width: 80
                    text : expCalc ? expCalc.expResultsStruct.timeLagVal.toFixed(3) : "" // format decimals
                    font.pointSize: 12
                    anchors.verticalCenter: parent.verticalCenter
                    horizontalAlignment: TextInput.AlignHCenter
                    // validator: DoubleValidator { bottom: 1e-12; top: 10000; decimals: 3}
                    selectByMouse: true
                    readOnly: true
                }
            }
            Row{
                spacing: 10
                Layout.columnSpan: 2
                Button{
                    id: applyCorTimeLagVal
                    width: 150
                    text : "Apply new time lag"
                    font.pointSize: 11
                    onClicked: {
                        expCalc.reCalculateDiffusFit(resultTimeLagSlider.value) //change to corrTimeLagVal
                    }
                }
                Slider{
                    id: resultTimeLagSlider
                    width: 200
                    property double timeLag: expCalc ? expCalc.expResultsStruct.timeLagVal : 1
                    from: (timeLag*0.1).toFixed(2)//0
                    to: (timeLag*2).toFixed(2)//10000
                    value: timeLag//25
                    // stepSize: 10
                    // snapMode: Slider.SnapOnRelease
                    // Connections {
                    //     target: expCalc
                    //     function onTimeLagValChanged() {
                    //         let val = expCalc.expResultsStruct.timeLagVal
                    //         resultTimeLagSlider.from = (val*0.8).toFixed(2)
                    //         resultTimeLagSlider.to = (val*1.2).toFixed(2)
                    //         resultTimeLagSlider.value = val
                    //         // resultTimeLagSlider.stepSize = (resultTimeLagSlider.value*0.02).toFixed(3)
                    //     }
                    // }
                }
                TextField{
                    id: corrTimeLagVal
                    width: 80
                    text : resultTimeLagSlider.value.toFixed(3) // format decimals
                    font.pointSize: 12
                    anchors.verticalCenter: parent.verticalCenter
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator { bottom: 1e-12; decimals: 3} //is it necessary? 
                    selectByMouse: true
                }
            }
            Row{
                spacing: 10
                Layout.columnSpan: 2
                Button{
                    width: 120
                    text: "Calculate"
                    font.pointSize: 11
                }
                Button{
                    width: 150
                    text: "Test set constants"
                    font.pointSize: 11
                    onClicked: backend.preapreExpCalc()
                }
            }
            Row{
                spacing: 10
                Layout.columnSpan: 2
                TextField{
                    id: runName
                    // property int curRunCnt:  flowToVolume ? flowToVolume.runCnt : 0
                    text: "Запуск #" //+ curRunCnt
                    font.pointSize: 11
                    // anchors.verticalCenter: parent.verticalCenter
                    horizontalAlignment: TextInput.AlignHCenter
                }
                Button {
                    // id: startExp
                    checkable: true
                    text: checked ? qsTr("Exp working") : qsTr("Exp waiting")  
                    font.pointSize: 11
                    onToggled: backend.beginExp(runName.text, checked)
                }
            }
            Row{
                spacing: 10
                Button{
                    width: 120
                    text: "Set Leak Start"
                    checkable: true
                    font.pointSize: 11
                    onClicked: checked = expCalc.setLeakStart(checked)
                }
                Button{
                    width: 120
                    text: "Set Leak End"
                    checkable: true
                    font.pointSize: 11
                    onClicked: checked = expCalc.setLeakEnd(checked)
                }
            }
            Row{
                spacing: 10
                Button{
                    width: 150
                    text: "Set Steady State"
                    checkable: true
                    font.pointSize: 11
                    onClicked: checked = expCalc.setSteadyStateStart(checked)
                }
                Button{
                    width: 150
                    text: "Pass accum points"
                    font.pointSize: 11
                    // onClicked: backend.preapreExpCalc()
                }
            }
            TextArea {
                id: resultText
                //Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
                placeholderText: qsTr("Enter description")
                text: expCalc ? expCalc.resultStr : ""
            }
            Row{
                spacing: 10
                Button{
                    width: 120
                    text: "Log path"
                    font.pointSize: 11
                }
                TextField{
                    id: logPath
                    width: 80
                    text : ""
                    horizontalAlignment: TextInput.AlignHCenter
                }
            }
            Row{
                spacing: 10
                Button{
                    width: 120
                    text: "Spectrum path"
                    font.pointSize: 11
                }
                TextField{
                    id: spectrumPath
                    width: 80
                    text : ""
                    horizontalAlignment: TextInput.AlignHCenter
                }
            }
            Row{
                spacing: 10
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
                    font.pointSize: 11
                }
            }
            Row{
                Layout.columnSpan: 2
                spacing: 10
                Label{
                    width: 100
                    text: "Averaging"
                    font.pointSize: 12
                    anchors.verticalCenter: parent.verticalCenter
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
                    // background: Rectangle {
                    //     x: avgSlider.leftPadding
                    //     y: avgSlider.topPadding + avgSlider.availableHeight / 2 - height / 2
                    //     implicitWidth: 200
                    //     implicitHeight: 4
                    //     width: avgSlider.availableWidth
                    //     height: implicitHeight
                    //     radius: 2
                    //     color: "#bdbebf"
                    //     Repeater {
                    //         model: (parent.to - parent.from) / parent.stepSize + 1
                    //         delegate: Column {
                    //             x: index * parent.width / (parent.to - parent.from) * parent.stepSize - width / 2
                    //             y: 0
                    //             spacing: 2
                    //             Rectangle {
                    //                 anchors.horizontalCenter: parent.horizontalCenter
                    //                 width: 1
                    //                 height: 10
                    //                 color: "grey"
                    //             }
                    //             Text {
                    //                 anchors.horizontalCenter: parent.horizontalCenter
                    //                 text: index * parent.stepSize + parent.from
                    //             }
                    //         }
                    //     }
                    //     Rectangle {
                    //         width: avgSlider.visualPosition * parent.width
                    //         height: parent.height
                    //         color: "#21be2b"
                    //         radius: 2
                    //     }
                    // }
                    // handle: Rectangle {
                    //     x: avgSlider.leftPadding + avgSlider.visualPosition * (avgSlider.availableWidth - width)
                    //     y: avgSlider.topPadding + avgSlider.availableHeight / 2 - height / 2
                    //     implicitWidth: 6
                    //     implicitHeight: 26
                    //     // radius: 13
                    //     color: avgSlider.pressed ? "#f0f0f0" : "#f6f6f6"
                    //     border.color: "#bdbebf"
                    // }
                }
            }
        }

        // ColumnLayout{
        //     id: calcCol
        //     width: base.width - 40
        //     anchors.top: gridResult.bottom
        //     anchors.left: parent.left
        //     anchors.topMargin: 10
        //     implicitHeight: 200
        //     spacing: 10
            //Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            //Layout.fillWidth: true;
            //Layout.fillHeight: false;
            
            // TableView {
            //     id: tableView
            //     Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
            //     implicitWidth: 500
            //     height: 250
            //     columnWidthProvider: function (column) { return 140; }
            //     rowHeightProvider: function (column) { return 60; }
            //     leftMargin: rowsHeader.implicitWidth
            //     topMargin: columnsHeader.implicitHeight
            //     model: table_model
            //     ScrollBar.horizontal: ScrollBar{}
            //     ScrollBar.vertical: ScrollBar{}
            //     clip: true
            //     delegate: Rectangle {
            //         Text {
            //             text: display
            //             anchors.fill: parent
            //             anchors.margins: 10
            //             color: 'black'
            //             font.pixelSize: 15
            //             verticalAlignment: Text.AlignVCenter
            //         }
            //     }
            //     Rectangle { // mask the headers
            //         z: 3
            //         color: "#222222"
            //         y: tableView.contentY
            //         x: tableView.contentX
            //         width: tableView.leftMargin
            //         height: tableView.topMargin
            //     }
            //     Row {
            //         id: columnsHeader
            //         y: tableView.contentY
            //         z: 2
            //         Repeater {
            //             model: tableView.columns > 0 ? tableView.columns : 1
            //             Label {
            //                 width: tableView.columnWidthProvider(modelData)
            //                 height: 35
            //                 text: table_model.headerData(modelData, Qt.Horizontal)
            //                 color: '#aaaaaa'
            //                 font.pixelSize: 15
            //                 padding: 10
            //                 verticalAlignment: Text.AlignVCenter

            //                 background: Rectangle { color: "#333333" }
            //             }
            //         }
            //     }
            //     Column {
            //         id: rowsHeader
            //         x: tableView.contentX
            //         z: 2
            //         Repeater {
            //             model: tableView.rows > 0 ? tableView.rows : 1
            //             Label {
            //                 width: 60
            //                 height: tableView.rowHeightProvider(modelData)
            //                 text: table_model.headerData(modelData, Qt.Vertical)
            //                 color: '#aaaaaa'
            //                 font.pixelSize: 15
            //                 padding: 10
            //                 verticalAlignment: Text.AlignVCenter
            //                 background: Rectangle { color: "#333333" }
            //             }
            //         }
            //     }
            //     ScrollIndicator.horizontal: ScrollIndicator { }
            //     ScrollIndicator.vertical: ScrollIndicator { }
            // }
        // }
    }
}
