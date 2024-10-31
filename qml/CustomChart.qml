import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import CustomPlot

Item {
    required property string plotName 
    // required property var sensorsList
    required property bool isExp
    required property int m_index
    property bool isDetachable: true
    required property string label
    //anchors.left: parent.left
    implicitWidth: 450
    implicitHeight: 350
    CustomPlotItem {
        id: customPlotPressure
        anchors.left: parent.left; anchors.right: parent.right; anchors.top: parent.top
        width: parent.width;  height: parent.height-50
        Component.onCompleted: {
            initCustomPlot(0)
            placePointerGraph(chartModel.getChartPtr(plotName), isExp)
            setCustomLabel(label)
        }
        // Component.onDestruction: testJSString(0)
        // function testJSString(num) {
        //     let text = "I have been destroyed_ %1"
        //     console.log(text.arg(num))
        // }
        
    }
    function forceUpdate(){
        customPlotPressure.updatePlot()
    }
    RoundButton {
        id: resetPosBtn
        width: 40
        height: 40
        anchors.left: parent.left; anchors.top: customPlotPressure.bottom
        text: "r"//"⟳"
        font.pixelSize: 18
        //Material.background: Material.Red
        //Material.roundedScale: Material.FullScale
        onClicked: customPlotPressure.resetPos()
    }
    RoundButton {
        id: changeUnitBtn
        width: 40
        height: 40
        anchors.left: resetPosBtn.right; anchors.top: customPlotPressure.bottom
        text: "b"//"㍴"
        font.pixelSize: 18
        font.bold: true
        font.hintingPreference: Font.PreferNoHinting
        //Material.background:Material.Red
        //Material.roundedScale: Material.FullScale
        onClicked: customPlotPressure.resetPos()
    }
    RoundButton {
        id: detachBtn
        width: 40
        height: 40
        anchors.left: changeUnitBtn.right; anchors.top: customPlotPressure.bottom
        text: "W"
        font.pixelSize: 18
        font.bold: true
        font.hintingPreference: Font.PreferNoHinting
    }
}  