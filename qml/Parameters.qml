import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

GroupBox {

    title: "Experimental parameters"
    id: parameters
    width: parent.width
    height: parent.height
    SampleLoader {
        id: expLoader
        source: "file:///C:/Users/mitya/source/repos/MHgrph/build/Debug/data/pseudoSampleReport.json"
    }
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
    function setExpParameters(sampleName){
        let expParameters = expLoader.jsonObject[sampleName]
        leakStart.text = toHHMMSS(expParameters.leakStart_s)
        leakEnd.text = toHHMMSS(expParameters.leakEnd_s)
        steadyStateStart.text = toHHMMSS(expParameters.steadyStateStart_s)
        let sampleParameters = expParameters.sample
        thickness.text = sampleParameters.thickness_mm
        diameter.text = sampleParameters.diameter_m.toExponential(3)
        sideVolume.text = sampleParameters.sideVolume_m3.toExponential(3)
    }
    ScrollView {
        anchors.fill: parent
        anchors.margins: 10
        contentWidth: gridExp.width + 10
        contentHeight: gridExp.height + gridSample.height + 10  // Same
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
            columns: parameters.width/220
            rowSpacing: 20
            Layout.preferredWidth: 200
            ComboBox{
                id: sampleChoose
                width: 130
                // height: 40
                model: expLoader.jsonObject ? Object.keys(expLoader.jsonObject) : 0
                font.pointSize: 11
                onActivated: setExpParameters(currentText)
                Component.onCompleted: currentIndex = -1
            }
            Button{
                id: applySample
                width: 130
                text : "Apply parameters"
                font.pointSize: 11
                onClicked: {
                    expCalc.expParametersStruct.thickness = thickness.text
                    expCalc.expParametersStruct.diameter = diameter.text //.toExponential(3)
                    expCalc.expParametersStruct.volume = sideVolume.text //.toExponential(3)

                    expCalc.expTimingStruct.leakStart = fromHHMMSS(leakStart.text)
                    expCalc.expTimingStruct.leakEnd = fromHHMMSS(leakEnd.text)
                    expCalc.expTimingStruct.steadyStateStart = fromHHMMSS(steadyStateStart.text)
                }
            }
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
                    width: 80
                    text: "Valve open"
                    font.pointSize: 12
                    anchors.verticalCenter: parent.verticalCenter
                }
                TextField{
                    id: valveOpen
                    width: 80
                    text : "00:00:00"
                    font.pointSize: 12
                    inputMask: "99:99:99"
                    inputMethodHints: Qt.ImhTime
                    anchors.verticalCenter: parent.verticalCenter
                    horizontalAlignment: TextInput.AlignHCenter
                    selectByMouse: true
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
                    text: "00:00:00"
                    font.pointSize: 12
                    inputMask: "99:99:99"
                    inputMethodHints: Qt.ImhTime
                    anchors.verticalCenter: parent.verticalCenter
                    horizontalAlignment: TextInput.AlignHCenter
                    selectByMouse: true
                }
            }
            Row{
                spacing: 10
                Label{
                    width: 80
                    text: "Leak start"
                    font.pointSize: 12
                    anchors.verticalCenter: parent.verticalCenter
                }
                TextField{
                    id: leakStart
                    width: 80
                    text: "00:00:00" //toHHMMSS(expCalc.expTimingStruct.leakStart)
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
                    width: 80
                    text: "Leak end"
                    font.pointSize: 12
                    anchors.verticalCenter: parent.verticalCenter
                }
                TextField{
                    id: leakEnd
                    width: 80
                    text : "00:00:00" //toHHMMSS(expCalc.expTimingStruct.leakEnd)
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
                    width: 80
                    text: "Steady State"
                    font.pointSize: 12
                    anchors.verticalCenter: parent.verticalCenter
                }
                TextField{
                    id: steadyStateStart
                    width: 80
                    text : "00:00:00" //toHHMMSS(expCalc.expTimingStruct.steadyStateStart)
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
                    width: 80
                    text: "Log end"
                    font.pointSize: 12
                    anchors.verticalCenter: parent.verticalCenter
                }
                TextField{
                    id: logEnd
                    width: 80
                    text : "00:00:00"
                    font.pointSize: 12
                    inputMask: "99:99:99"
                    inputMethodHints: Qt.ImhTime//Qt.ImhDigitsOnly
                    anchors.verticalCenter: parent.verticalCenter
                    horizontalAlignment: TextInput.AlignHCenter
                }
            }
        }
        GridLayout {
            id: gridSample
            width: parameters.width - 40
            anchors.top: gridExp.bottom
            anchors.left: parent.left
            anchors.topMargin: 10
            implicitHeight: 200
            flow: GridLayout.LeftToRight
            columns: parameters.width/220
            rowSpacing: 20
            Layout.preferredWidth: 200 

            Row{
                spacing: 10
                Label{
                    width: 120
                    text: "Thickness, mm"
                    font.pointSize: 12
                    anchors.verticalCenter: parent.verticalCenter
                }
                //
                TextField{
                    id: thickness
                    width: 80
                    text : "1.000"
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
                    width: 80
                    text: "Diameter, m"
                    font.pointSize: 12
                    anchors.verticalCenter: parent.verticalCenter
                }
                TextField{
                    id: diameter
                    width: 80
                    text : "1.000"
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
                    text: "Side Volume, m3"
                    font.pointSize: 12
                    anchors.verticalCenter: parent.verticalCenter
                }
                TextField{
                    id: sideVolume
                    width: 80
                    text : "1.000"
                    font.pointSize: 12
                    validator: DoubleValidator { bottom: 1e-12; top: 10000} //; decimals: 2
                    selectByMouse: true
                    anchors.verticalCenter: parent.verticalCenter
                    horizontalAlignment: TextInput.AlignHCenter
                }
            }
        }
    }
}
