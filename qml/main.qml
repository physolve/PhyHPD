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
        object.testFunction(true)
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
            width: 600
            height: 400
            CustomPlotItem {
                id: customPlotPressure
                width: parent.width;  height: parent.height-50 // resize
                //width: 600
                //height: 400
                anchors.left: parent.left; anchors.top: parent.top
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
                id: resetPosBtn
                width: 40
                height: 40
                anchors.left: parent.left; anchors.top: customPlotPressure.bottom
                text: "⟳"
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
                text: "㍴"
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
                //Material.background:Material.Red
                //Material.roundedScale: Material.FullScale
                onClicked: detachPressure()
            }
            function testFunction(b){
                detachBtn.visible = b   
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
                    id: barRepeater
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
                    onItemAdded: {
                        //index
                        //item
                        console.log("item added")
                        //barRepeater.model.push(index)
                    }
                    onItemRemoved: {
                        //index
                        //item
                        console.log("item removed")
                        //barRepeater.model.splice(index)
                    }
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
                object.testFunction(false)
                container.append(object)
            }
            Pane {
                anchors.fill: parent
                Material.theme: Material.Dark
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
            }
            onClosing: {
                returnStack(container.get(0))
            }
        }
    }
    function detachPressure(){
        let window = plotComponent.createObject()
        //window.color = Material.color(Material.Red)
        baseContainer.remove(0)
        window.changeStack(baseContainer.get(0))
        window.show()
    }
}