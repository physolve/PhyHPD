import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

GroupBox {
    title: "Experimental parameters"
    id: parameters
    width: parent.width
    height: parent.height
    function toHHMMSS(secs) { // seems working
        let newDate = new Date(secs * 1000).toISOString().slice(11, 19);
        // debug if need, Date use 1970 thing
        return newDate
    }
    function fromHHMMSS(str){
        // let date = new Date(str);
        let a = str.split(':'); // split it at the colons
        // minutes are worth 60 seconds. Hours are worth 60 minutes.
        let seconds = (+a[0]) * 60 * 60 + (+a[1]) * 60 + (+a[2]); 
        return seconds; //
    }
    function saveParameters(){
        expCalc.expInfoStruct.expTemperature = temperatureValue.text

        expCalc.expParametersStruct.nameOfSample = textSampleName.text
        expCalc.expParametersStruct.thickness = textThickness.text
        expCalc.expParametersStruct.diameter = textDiameter.text //.toExponential(3)
        expCalc.expParametersStruct.volume = textVolume.text //.toExponential(3)

        expCalc.expTimingStruct.leakStart = fromHHMMSS(textLeakStart.text)
        expCalc.expTimingStruct.leakEnd = fromHHMMSS(textLeakEnd.text)
        expCalc.expTimingStruct.steadyStateStart = fromHHMMSS(textSteadyStateStart.text)
    }
    ScrollView {
        anchors.fill: parent
        anchors.margins: 10
        contentWidth: gridExp.width + 10
        contentHeight: gridExp.height + 10 + 50  // Same
        clip: true
        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
        ScrollBar.vertical.policy: ScrollBar.AsNeeded
        GridLayout { // move to results
            id: gridExp
            width: parameters.width - 40
            anchors.top: parent.top
            anchors.left: parent.left
            implicitHeight: 200
            flow: GridLayout.LeftToRight
            columns: parameters.width/260
            rowSpacing: 20
            // Layout.preferredWidth: 350
            Row{
                spacing: 10
                Label{
                    width: 100
                    text: "Exp list:"
                    font.pointSize: 12
                    anchors.verticalCenter: parent.verticalCenter
                }
                ComboBox{
                    id: sampleChoose
                    width: 150
                    // Layout.minimumWidth: 200
                    // height: 40
                    model: expCalc ? expCalc.expNames : 0//expLoader.jsonObject ? Object.keys(expLoader.jsonObject) : 0
                    font.pointSize: 11
                    onActivated: {
                        expCalc.applyExpFromJSON(currentText)
                        // exportName.text = currentText
                        calculateLastData.enabled = expCalc.isLastDataAvailable(currentText)
                    }
                    Component.onCompleted: {
                        currentIndex = 0
                        expCalc.applyExpFromJSON(currentText)
                    }
                }
                
            }
            Row{
                spacing: 10

                DelayButton{
                    Layout.preferredWidth: 150
                    id: calculateLastData
                    text: "Hold to view last results"
                    enabled: false
                    onActivated: expCalc.calculateLastData()
                }
            }
            Row{
                spacing: 10
                Label{
                    width: 100
                    text: "Exp name:"
                    font.pointSize: 12
                    anchors.verticalCenter: parent.verticalCenter
                }
                TextField{
                    id: exportName
                    // property int curRunCnt:  flowToVolume ? flowToVolume.runCnt : 0
                    width: 140
                    text: expCalc ? expCalc.expInfoStruct.chExpName : "blank" //+ curRunCnt
                    font.pointSize: 11
                    placeholderText: "Эксперимент..."
                    // anchors.verticalCenter: parent.verticalCenter
                    horizontalAlignment: TextInput.AlignHCenter
                }
            }
            Row{
                spacing: 10
                Label{
                    width: 100
                    text: "Sample name:"
                    font.pointSize: 12
                    anchors.verticalCenter: parent.verticalCenter
                }
                TextField{
                    id: textSampleName
                    // property int curRunCnt:  flowToVolume ? flowToVolume.runCnt : 0
                    width: 140
                    text: expCalc ? expCalc.expParametersStruct.nameOfSample : "" //+ curRunCnt
                    font.pointSize: 11
                    // anchors.verticalCenter: parent.verticalCenter
                    placeholderText: "Образец ..."
                    horizontalAlignment: TextInput.AlignHCenter
                }
            }
            
            Row{
                spacing: 10
                Label{
                    width: 120
                    text: "Exp start:"
                    font.pointSize: 12
                    anchors.verticalCenter: parent.verticalCenter
                }
                TextField{
                    id: logStart
                    width: 100
                    property int logStartVal: expCalc ? expCalc.expInfoStruct.expStart : 0
                    text: toHHMMSS(logStartVal)
                    font.pointSize: 12
                    inputMask: "99:99:99"
                    inputMethodHints: Qt.ImhTime//Qt.ImhDigitsOnly
                    anchors.verticalCenter: parent.verticalCenter
                    horizontalAlignment: TextInput.AlignHCenter
                }
            }
            Row{
                spacing: 10
                Label{
                    width: 120
                    text: "Leak start:"
                    font.pointSize: 12
                    anchors.verticalCenter: parent.verticalCenter
                }
                TextField{
                    // id: leakStart
                    id: textLeakStart
                    width: 100
                    property int leakStartVal: expCalc ? expCalc.expTimingStruct.leakStart : 0
                    text: toHHMMSS(leakStartVal)
                    font.pointSize: 12
                    inputMask: "99:99:99"
                    // inputMethodHints: Qt.ImhTime//Qt.ImhDigitsOnly
                    anchors.verticalCenter: parent.verticalCenter
                    horizontalAlignment: TextInput.AlignHCenter
                    selectByMouse: true
                }
            }
            Row{
                spacing: 10
                Label{
                    width: 120
                    text: "Leak end:"
                    font.pointSize: 12
                    anchors.verticalCenter: parent.verticalCenter
                }
                TextField{
                    // id: leakEnd
                    id: textLeakEnd
                    width: 100
                    property int leakEndVal: expCalc ? expCalc.expTimingStruct.leakEnd : 0
                    text: expCalc ? toHHMMSS(leakEndVal) : "00:00:00"
                    font.pointSize: 12
                    inputMask: "99:99:99"
                    // inputMethodHints: Qt.ImhTime//Qt.ImhDigitsOnly
                    anchors.verticalCenter: parent.verticalCenter
                    horizontalAlignment: TextInput.AlignHCenter
                }
            }
            Row{
                spacing: 10
                Label{
                    width: 120
                    text: "Steady State:"
                    font.pointSize: 12
                    anchors.verticalCenter: parent.verticalCenter
                }
                TextField{
                    // id: steadyStateStart
                    id: textSteadyStateStart
                    width: 100
                    text: expCalc ? toHHMMSS(expCalc.expTimingStruct.steadyStateStart) : "00:00:00" //toHHMMSS(expCalc.expTimingStruct.steadyStateStart)
                    font.pointSize: 12
                    inputMask: "99:99:99"
                    // inputMethodHints: Qt.ImhTime//Qt.ImhDigitsOnly
                    anchors.verticalCenter: parent.verticalCenter
                    horizontalAlignment: TextInput.AlignHCenter
                    selectByMouse: true
                }
            }
            Row{
                spacing: 10
                Label{
                    width: 120
                    text: "Exp end:"
                    font.pointSize: 12
                    anchors.verticalCenter: parent.verticalCenter
                }
                TextField{
                    id: logEnd
                    width: 100
                    property int logEndVal: expCalc ? expCalc.expInfoStruct.expEnd : 0
                    text: toHHMMSS(logEndVal)
                    font.pointSize: 12
                    inputMask: "99:99:99"
                    inputMethodHints: Qt.ImhTime//Qt.ImhDigitsOnly
                    anchors.verticalCenter: parent.verticalCenter
                    horizontalAlignment: TextInput.AlignHCenter
                }
            }
            Row{
                spacing: 10
                Label{
                    width: 120
                    property string statusStr: expCalc ? expCalc.expInfoStruct.isExpWorking ? "Exp started" : "Exp waiting" : ""
                    text: "Current status: " + statusStr
                    font.pointSize: 12
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
            Row{
                spacing: 10
                Label{
                    width: 120
                    text: "Thickness, mm:"
                    font.pointSize: 12
                    anchors.verticalCenter: parent.verticalCenter
                }
                //
                TextField{
                    id: textThickness
                    width: 100
                    text: expCalc ? expCalc.expParametersStruct.thickness : 0
                    font.pointSize: 12
                    validator: DoubleValidator { bottom: 1e-12; top: 10000} //; decimals: 3
                    selectByMouse: true
                    anchors.verticalCenter: parent.verticalCenter
                    horizontalAlignment: TextInput.AlignHCenter
                }
            }//
            Row{
                spacing: 10
                Label{
                    width: 120
                    text: "Diameter, m:"
                    font.pointSize: 12
                    anchors.verticalCenter: parent.verticalCenter
                }
                TextField{
                    id: textDiameter
                    width: 100
                    text: expCalc ? expCalc.expParametersStruct.diameter.toExponential(3) : 0
                    font.pointSize: 12
                    validator: DoubleValidator { bottom: 1e-12; top: 10000} //; decimals: 3
                    selectByMouse: true
                    anchors.verticalCenter: parent.verticalCenter
                    horizontalAlignment: TextInput.AlignHCenter
                }
            }
            Row{
                spacing: 10
                Label{
                    width: 120
                    text: "Side Volume, m3:"
                    font.pointSize: 12
                    anchors.verticalCenter: parent.verticalCenter
                }
                TextField{
                    id: textVolume
                    width: 100
                    text: expCalc ? expCalc.expParametersStruct.volume.toExponential(3) : 0
                    font.pointSize: 12
                    validator: DoubleValidator { bottom: 1e-12; top: 10000} //; decimals: 2
                    selectByMouse: true
                    anchors.verticalCenter: parent.verticalCenter
                    horizontalAlignment: TextInput.AlignHCenter
                }
            }
            Row{
                spacing: 10
                Label{
                    width: 120
                    text: "Temperature, °C:"
                    font.pointSize: 12
                    anchors.verticalCenter: parent.verticalCenter
                }
                TextField{
                    id: temperatureValue
                    width: 100
                    text: expCalc ? expCalc.expInfoStruct.expTemperature : 0
                    font.pointSize: 12
                    validator: DoubleValidator { bottom: 1e-12; top: 10000} //; decimals: 2
                    selectByMouse: true
                    anchors.verticalCenter: parent.verticalCenter
                    horizontalAlignment: TextInput.AlignHCenter
                }
            } 
        }

        Button{
            id: exportCurrentToJson
            width: 130
            anchors.top: gridExp.bottom
            anchors.right: applySample.left
            anchors.rightMargin: 20
            text: "Export"
            font.pointSize: 11
            onClicked: {
                expCalc.expInfoStruct.chExpName = exportName.text
                saveParameters()
                expCalc.applyExpToJSON(exportName.text)
                sampleChoose.currentIndex = sampleChoose.indexOfValue(exportName.text)
            }
        }
        Button{
            id: applySample
            width: 130
            anchors.top: gridExp.bottom
            anchors.right: parent.right
            anchors.rightMargin: 20
            text : "Save"
            font.pointSize: 11
            onClicked: saveParameters()
        }
    }
}
