import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import CustomPlot
Item{
    anchors.fill: parent
    Component{
        id: plotPressure
        Item {
            required property string plotName 
            required property var sensorsList
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
                    for(const sensorName of sensorsList){
                        placePointerGraph(sensorName, dataModel.getSensor(sensorName))
                    }
                    //setCustomLabel(label)
                }
                Component.onDestruction: testJSString(0)
                function testJSString(num) {
                    let text = "I have been destroyed_ %1"
                    console.log(text.arg(num))
                }
                
            }
            Connections {
                target: dataModel
                function onDataChanged() { 
                    customPlotPressure.updatePlot()
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
                //onClicked: detachPressure(m_index)
            }
            // function testFunction(b){
            //     detachBtn.visible = b   
            // }
        }  
    }
    /*UNUSED*/
    Component { 
        id: tabButtonComponent

        TabButton {
            property color frameColor: "#999"
            property color fillColor: "#eee"
            property color nonSelectedColor: "#ddd"
            property string tabTitle: "New Tab"

            id: tabButton
            contentItem: Rectangle {
                id: tabRectangle
                color: tabButton.down ? fillColor : nonSelectedColor
                border.width: 1
                border.color: frameColor
                implicitWidth: Math.max(text.width + 30, 80)
                implicitHeight: Math.max(text.height + 10, 20)
                Rectangle { height: 1 ; width: parent.width ; color: frameColor}
                Rectangle { height: parent.height ; width: 1; color: frameColor}
                Rectangle { x: parent.width - 2; height: parent.height ; width: 1; color: frameColor}
                Text {
                    id: text
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.leftMargin: 6
                    text: tabButton.tabTitle
                    elide: Text.ElideRight
                    color: tabButton.down ? "black" : frameColor
                    width: parent.width - button.background.width
                }
                Button {
                    id: button
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.rightMargin: 4
                    height: 12
                    background: Rectangle {
                        implicitWidth: 12
                        implicitHeight: 12
                        color: button.hovered ? "#ccc" : tabRectangle.color
                        Text {text: "x"; anchors.centerIn: parent; color: "gray"}
                    }
                    onClicked: tabButton.closeTab()
                }
            }

            onClicked: addressBar.text = tabLayout.itemAt(TabBar.index).url;
            function closeTab() {
                tabBar.removeView(TabBar.index);
            }
        }
    }
    /*UNUSED*/
    Component{
        id: chartExpData
        ChartExpData{

        }
    }

    TabBar{
        id: tabBar
        anchors.top: parent.top
        width: parent.width
        // anchors.left: parent.left
        // anchors.right: parent.right
        Repeater{
            id: barGraphRepeater
            model: ["Pressure", "Vacuum", "Flux", "Diffusivity", "Permeation"]
            TabButton{
                text: modelData
                width: Math.max(100, tabBar.width/5)
                font.pointSize: 12
            }
        }
        Component.onCompleted: createTab() /*!!!*/
        function createTab() {
            plotPressure.createObject(tabLayout,{plotName: "Pressure", sensorsList: ["pressure"], m_index: 0, label: "Давление, бар"})
            plotPressure.createObject(tabLayout,{plotName: "Vacuum", sensorsList: ["vacuum"], m_index: 0, label: "Давление, бар"})
            // var newTabButton = tabButtonComponent.createObject(tabBar, {tabTitle: "Pressure"});
            // tabBar.addItem(newTabButton);
            // tabBar.setCurrentIndex(tabBar.count - 1);
            // return webview;
            chartExpData.createObject(tabLayout,{plotName: "Flux", sensorsList: ["flux"], m_index: 0, label: "Поток"})
            chartExpData.createObject(tabLayout,{plotName: "Diffusivity", sensorsList: ["diffusivity","modeldiffus"], m_index: 0, label: "Диффузия"})
            chartExpData.createObject(tabLayout,{plotName: "Permeation", sensorsList: ["permeation"], m_index: 0, label: "Проницаемость"})
        }
        function removeView(index) {
            tabBar.removeItem(index);
            if (tabBar.count > 1) {
                tabBar.removeItem(tabBar.itemAt(index)); /*!!!*/
                tabLayout.children[index].destroy(); /*!!!*/
            } else {
                browserWindow.close(); /*!!!*/
            }
        }
        
    }

    StackLayout{
        id: tabLayout
        currentIndex: tabBar.currentIndex
        anchors.top: tabBar.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.topMargin: 20
        anchors.bottomMargin: 80
    }


    // TabBar {
    //     id: barGraph
    //     width: parent.width
    //     Repeater{
    //         id: barGraphRepeater
    //         model: ["Pressure", "Vacuum"]
    //         TabButton{
    //             text: modelData
    //             width: Math.max(100, barGraph.width/5)
    //         }
    //     }
    // }
    
    // function detachPressure(){
    //     let window = plotComponent.createObject()
    //     //window.color = Material.color(Material.Red)
    //     //baseContainer.remove(0)
    //     window.changeStack(baseContainer.get(0))
    //     window.show()
    // }
    // function returnStack(object){
    //     object.testFunction(true)
    //     baseContainer.append(object)
    // }

    // Component{
    //     id: plotComponent
    //     Window{
    //         id: plotWindow
    //         width: 600
    //         height: 450
    //         function changeStack(object){
    //             object.testFunction(false)
    //             container.append(object)
    //         }
    //         Pane {
    //             anchors.fill: parent
    //             Material.theme: Material.Dark
    //             StackLayout {
    //                 id: layout
    //                 anchors.fill: parent
    //                 Repeater {
    //                     model: ObjectModel
    //                     {
    //                         id: container
    //                     }
    //                 }
    //             }
    //         }
    //         onClosing: {
    //             returnStack(container.get(0))
    //         }
    //     }
    // }
}