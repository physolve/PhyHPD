import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material 2.12
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
                    width: 100
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
                    // width: 160
                    text : "Apply new time lag"
                    font.pointSize: 11
                    Material.roundedScale: Material.SmallScale
                    onClicked: {
                        expCalc.reCalculateDiffusFit(resultTimeLagSlider.value) //change to corrTimeLagVal
                    }
                }
                Slider{
                    id: resultTimeLagSlider
                    width: 250
                    property double timeLag: expCalc ? expCalc.expResultsStruct.timeLagVal : 1
                    from: (timeLag*0.1).toFixed(2)//0
                    to: (timeLag*2).toFixed(2)//10000
                    value: timeLag//25
                }
                TextField{
                    id: corrTimeLagVal
                    width: 100
                    text : resultTimeLagSlider.value.toFixed(3) // format decimals
                    font.pointSize: 12
                    anchors.verticalCenter: parent.verticalCenter
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator { bottom: 1e-12; decimals: 3} //is it necessary? 
                    selectByMouse: true
                }
            }
            
            Button {
                // id: startExp
                Layout.columnSpan: 2
                Layout.preferredWidth: 250
                Layout.alignment: Qt.AlignHCenter
                checkable: true
                text: expCalc ? expCalc.expInfoStruct.isExpWorking ? qsTr("Exp working. Push to end") : qsTr("Exp waiting. Push to start") : "I don't know" 
                font.pointSize: 11
                Material.roundedScale: Material.SmallScale
                onToggled: backend.beginExp(runName.text, checked) // unknown behavior
            }
            // Row{
            //     spacing: 10
            //     Layout.columnSpan: 2
            //     TextField{
            //         id: runName
            //         // property int curRunCnt:  flowToVolume ? flowToVolume.runCnt : 0
            //         text: "Запуск #" //+ curRunCnt
            //         font.pointSize: 11
            //         // anchors.verticalCenter: parent.verticalCenter
            //         horizontalAlignment: TextInput.AlignHCenter
            //     }
            // }
            
            Row{
                spacing: 50
                Layout.columnSpan: 2
                Layout.alignment: Qt.AlignHCenter
                Button{
                    width: 180
                    text: "Set Leak Start"
                    checkable: true
                    font.pointSize: 11
                    Material.roundedScale: Material.SmallScale
                    onClicked: checked = expCalc.setLeakStart(checked)
                }
                Button{
                    width: 180
                    text: "Set Leak End"
                    checkable: true
                    font.pointSize: 11
                    Material.roundedScale: Material.SmallScale
                    onClicked: checked = expCalc.setLeakEnd(checked)
                }
            }
            Row{
                spacing: 50
                Layout.columnSpan: 2
                Layout.alignment: Qt.AlignHCenter
                Button{
                    width: 180
                    text: "Set Steady State"
                    checkable: true
                    font.pointSize: 11
                    Material.roundedScale: Material.SmallScale
                    onClicked: checked = expCalc.setSteadyStateStart(checked)
                }
                Button{
                    width: 180
                    text: "Pass accum points"
                    font.pointSize: 11
                    Material.roundedScale: Material.SmallScale
                    // onClicked: backend.preapreExpCalc()
                }
            }
            TextArea {
                id: resultText
                //Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
                Layout.columnSpan: 2
                Layout.alignment: Qt.AlignHCenter
                placeholderText: qsTr("Results")
                text: expCalc ? expCalc.resultStr : ""
            }
            
            Row{
                spacing: 10
                Button{
                    width: 150
                    text: "Spectrum path"
                    font.pointSize: 11
                    Material.roundedScale: Material.SmallScale
                }
                TextField{
                    id: spectrumPath
                    width: 100
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
                    width: 150
                    text : "Diffusion"
                    font.pointSize: 11
                    Material.roundedScale: Material.SmallScale
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
                Slider{
                    id: avgSlider
                    width: 250
                    from: 0
                    value: 25
                    to: 100
                    stepSize: 10
                    snapMode: Slider.SnapOnRelease
                }
                TextField{
                    id: curSeconds
                    width: 100
                    text : avgSlider.value
                    horizontalAlignment: TextInput.AlignHCenter
                }
            }
            Row{
                spacing: 30
                Layout.columnSpan: 2 
                Layout.alignment: Qt.AlignHCenter
                TextField{
                    width: 250
                    id: lastExpDataFilePath
                    readOnly: true
                    placeholderText: "Path to last Exp"
                    text: expCalc ? expCalc.expInfoStruct.lastExpDataFile : ""
                }
                TextField{
                    width: 250
                    id: currentExpDataFilePath
                    readOnly: true
                    placeholderText: "Path to current Exp"
                    text: expCalc ? expCalc.expInfoStruct.currentExpDataFile : ""
                }
            }
        }
    }
}
