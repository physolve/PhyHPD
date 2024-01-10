import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
//import Qt.labs.settings

ApplicationWindow {
    id: main
    width: 1730
    height: 800
    visible: true
    title: qsTr("MHgrph")
    Material.theme: Material.Dark
    Material.accent: Material.Indigo
    ObjectModel
    {
        id: mainContainer
    }
    SplitView {
        id: grid
        anchors.fill: parent
        Item {
            implicitWidth: 1100
            SplitView.minimumWidth: 1000
            Rectangle{
                anchors.margins: 10
                anchors.fill: parent
                color: "transparent"
                Mnemo {
                    id: mainScreen
                }
            }
        }
        ColumnLayout{
            SplitView.minimumWidth: 500
            SplitView.preferredWidth: 670
            SplitView.maximumWidth: 700
            TabBar {
                id: barMain
                width: parent.width
                Repeater{
                    id: barMainRepeater
                    model: ["Graph", "Settings"]
                    TabButton{
                        text: modelData
                        width: Math.max(100, barMain.width/5)
                    }
                }
            }
            StackLayout {
                id: layoutMain
                Layout.fillWidth: true
                //width: 600
                //height: 800
                currentIndex: barMain.currentIndex
                Item{
                    Layout.fillHeight: true
                    Graphs{
                    }
                }   
                Item{
                    Layout.fillHeight: true
                    DataWindow{
                    }
                }
            } 
        }
    }
}