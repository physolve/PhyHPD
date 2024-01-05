import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
//import Qt.labs.settings

import CustomPlot
ApplicationWindow {
    id: main
    width: 1400
    height: 800
    visible: true
    title: qsTr("MHgrph")
    Material.theme: Material.Dark
    Material.accent: Material.Indigo
    function returnStack(object){
        baseContainer.append(object)
    }
    ObjectModel
    {
        id: baseContainer
    }
    Component{
        id: plotPressure
        Item {
            //id: itemPlotPressure
            //width: 600
            //height: 400
             GridLayout {
                columns: 3
                rows: 2
                anchors.fill: parent
                columnSpacing: 0
                rowSpacing: 0

                CustomPlotItem {
                    id: customPlotPressure
                    //width: parent.width;  height: parent.height // resize
                    width: 600
                    height: 400
                    Layout.columnSpan: 3
                    Layout.rowSpan: 1
                    Component.onCompleted: initCustomPlot(0)
                    Component.onDestruction: testJSString(0)
                    function testJSString(num) {
                        let text = "I have been destroyed_ %1"
                        console.log(text.arg(num))
                    }
                    
                }
                Connections {
                    target: pressureBack 
                    onPointsChanged: {
                        customPlotPressure.backendData(x, y)
                    }
                }
                
                RoundButton {
                    width: 40
                    height: 40
                    text: "⟳"
                    font.pixelSize: 18
                    //Material.background: Material.Red
                    //Material.roundedScale: Material.FullScale
                    onClicked: customPlotPressure.resetPos()
                }
                RoundButton {
                    width: 40
                    height: 40
                    text: "㍴"
                    font.pixelSize: 18
                    font.bold: true
                    font.hintingPreference: Font.PreferNoHinting
                    //Material.background:Material.Red
                    //Material.roundedScale: Material.FullScale
                    onClicked: customPlotPressure.resetPos()
                }
                RoundButton {
                    width: 40
                    height: 40
                    text: "W"
                    font.pixelSize: 18
                    font.bold: true
                    font.hintingPreference: Font.PreferNoHinting
                    //Material.background:Material.Red
                    //Material.roundedScale: Material.FullScale
                    onClicked: detachPressure()
                }
            }
        }  
    }
    Component{
        id: plotVacuum
        Item {
            width: 600
            height: 400
            id: itemPlotVacuum
            CustomPlotItem {
                id: customPlotVacuum
                width: parent.width;  height: parent.height // resize
                Component.onCompleted: initCustomPlot(1)
                Component.onDestruction: testJSString(1)
                function testJSString(num) {
                    let text = "I have been destroyed_ %1"
                    console.log(text.arg(num))
                }
                RoundButton {
                    x:-40
                    y:10
                    width: 40
                    height: 40
                    text: "⟳"
                    font.pixelSize: 18
                    font.bold: true
                    //Material.background:Material.Red
                    //Material.roundedScale: Material.FullScale
                    onClicked: customPlotVacuum.resetPos()
                }
                RoundButton {
                    x:-40
                    y:50
                    width: 40
                    height: 40
                    text: "㍴"
                    font.pixelSize: 18
                    font.bold: true
                    font.hintingPreference: Font.PreferNoHinting
                    //Material.background:Material.Red
                    //Material.roundedScale: Material.FullScale
                    onClicked: customPlotVacuum.resetPos()
                }
            }
            Connections {
                target: vacuumBack 
                onPointsChanged: {
                    customPlotVacuum.backendData(x, y)
                }
            }
        }
    }
    GridLayout {
        id: grid
        anchors.fill: parent
        columnSpacing: 1
        flow: GridLayout.LeftToRight
        columns: 2
        Item {
            width: 700
            height: parent.height
            DataWindow{
            }
        }
        ColumnLayout{
            TabBar {
                id: bar
                width: parent.width
                Repeater{
                    model: ["Pressure", "Vacuum"]
                    TabButton{
                        text: modelData
                        width: Math.max(100, bar.width/5)
                    }
                }
            }
            StackLayout {
                id: layout
                width: 600
                height: 400
                currentIndex: bar.currentIndex
                Repeater {
                    model: baseContainer
                }
                Component.onCompleted: {
                    let pressure = plotPressure.createObject()
                    baseContainer.append(pressure)
                    let vacuum = plotVacuum.createObject()
                    baseContainer.append(vacuum)
                }
            }
        }   
    }
    Component{
        id: plotComponent
        Window{
            id: plotWindow
            width: 600
            height: 450
            function changeStack(object){
                container.append(object)
            }
            StackLayout {
                id: layout
                anchors.fill: parent
                Repeater {
                    model: ObjectModel
                    {
                        id: container
                    }
                }
            }
            onClosing: {
                returnStack(container.get(0))
            }
        }
    }
    function detachPressure(){
        let window = plotComponent.createObject()
        window.color = Material.color(Material.Red)
        window.changeStack(baseContainer.get(0))
        window.show()
    }
}