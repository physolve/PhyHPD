import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
//import Qt.labs.settings
import QtQuick.Controls.Material 2.12
import "mnemo"

ApplicationWindow {
    id: main
    width: 1730
    height: 800
    visible: true
    title: qsTr("MHgrph")
    // Material.theme: Material.Dark
    // Material.accent: Material.Indigo
    // font.capitalization: Font.MixedCase
    // ObjectModel
    // {
    //     id: mainContainer
    // }
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
        Item{
            SplitView.minimumWidth: 550
            SplitView.preferredWidth: 670
            TabBar {
                id: barMain
                width: parent.width
                anchors.top: parent.top
                anchors.left: parent.left
                //anchors.right: parent.right
                //height: 100
                Repeater{
                    id: barMainRepeater
                    model: ["Graph", "Experiment", "Settings"]
                    TabButton{
                        text: modelData
                        width: Math.max(120, barMain.width/3)
                        font.pointSize: 12
                    }
                }
            }
            StackLayout {
                id: layoutMain
                //Layout.fillWidth: true
                //width: 600
                //height: 800
                anchors.top: barMain.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                //width: parent.width
                currentIndex: barMain.currentIndex
                Item{
                    //Layout.fillHeight: true
                    Charts{
                    }
                }   
                Item{
                    //Layout.fillHeight: true
                    Experimental{
                    }
                }
                Item{
                    //Layout.fillHeight: true
                    DataWindow{
                    }
                }
            } 
        }
    }
}