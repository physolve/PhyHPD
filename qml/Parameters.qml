import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

GroupBox {
    title: "Experimental parameters"
    id: parameters
    width: parent.width
    height: parent.height
    SampleLoader {
        id: sampleLoader
        source: "file:///C:/Users/mitya/source/repos/MHgrph/build/Debug/data/pseudoSample.json"
    }
    function toHHMMSS(secs) { // seems working
        let newDate = new Date(secs * 1000).toISOString().slice(11, 19);
        return newDate
    }
    function setSampleParameters(sampleName){
        let sampleParameters = sampleLoader.jsonObject[sampleName]
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
                    text: "00:00:00.000"
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
                    text: "Leak start"
                    font.pointSize: 12
                    anchors.verticalCenter: parent.verticalCenter
                }
                TextField{
                    id: leakStart
                    width: 80
                    text: toHHMMSS(expCalc.leakStart)
                    font.pointSize: 12
                    // inputMask: "99:99:99"
                    // inputMethodHints: Qt.ImhTime//Qt.ImhDigitsOnly
                    anchors.verticalCenter: parent.verticalCenter
                    horizontalAlignment: TextInput.AlignHCenter
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
                    text : toHHMMSS(expCalc.leakEnd)
                    font.pointSize: 12
                    // inputMask: "99:99:99"
                    // inputMethodHints: Qt.ImhTime//Qt.ImhDigitsOnly
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
            rowSpacing: 10
            Layout.preferredWidth: 200 

            ComboBox{
                id: sampleChoose
                width: 210
                height: 40
                model: sampleLoader.jsonObject ? Object.keys(sampleLoader.jsonObject) : 0
                font.pointSize: 10
                onActivated: setSampleParameters(currentText)
                Component.onCompleted: currentIndex = -1
            }
            Button{
                id: applyParameters
                width: 130
                text : "Apply parameters"
                font.pointSize: 10
                onClicked: {
                    let parameters = {}
                    parameters["sideVolume"] = sideVolume.text //m3
                    parameters["thickness"] = thickness.text //["micron", "mm", "cm"] -> m or only mm?
                    parameters["diameter"] = diameter.text; //m
                    expCalc.setConstants(parameters)
                }
            }
            Row{
                spacing: 10
                Label{
                    width: 120
                    text: "Thickness, mm"
                    font.pointSize: 12
                    anchors.verticalCenter: parent.verticalCenter
                }
                TextField{
                    id: thickness
                    width: 80
                    text : "1.000"
                    font.pointSize: 12
                    validator: DoubleValidator { bottom: 1e-12; top: 10000; decimals: 2}
                    selectByMouse: true
                    anchors.verticalCenter: parent.verticalCenter
                    horizontalAlignment: TextInput.AlignHCenter
                }
                // ComboBox{
                //     id: thickDimension
                //     width: 100
                //     height: 40
                //     model: ["micron", "mm", "cm"]
                //     currentIndex: 1
                //     font.pointSize: 12
                //     anchors.verticalCenter: parent.verticalCenter
                // }
            }
            Row{
                spacing: 10
                Label{
                    width: 120
                    text: "Diameter, m"
                    font.pointSize: 12
                    anchors.verticalCenter: parent.verticalCenter
                }
                TextField{
                    id: diameter
                    width: 80
                    text : "1.000"
                    font.pointSize: 12
                    validator: DoubleValidator { bottom: 1e-12; top: 10000; decimals: 2}
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
                    validator: DoubleValidator { bottom: 1e-12; top: 10000; decimals: 2}
                    selectByMouse: true
                    anchors.verticalCenter: parent.verticalCenter
                    horizontalAlignment: TextInput.AlignHCenter
                }
            }
        }
    }
}
