import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item{
    anchors.fill: parent
    TabBar {
        id: barExp
        width: parent.width
        anchors.top: parent.top
        // anchors.left: parent.left
        //anchors.right: parent.right
        //height: 100
        Repeater{
            id: barExpRepeater
            model: ["Parameters", "Results"]
            TabButton{
                text: modelData
                width: Math.max(120, barExp.width/2)
                font.pointSize: 12
            }
        }
    }
    StackLayout {
        id: layoutExp
        anchors.top: barExp.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        //width: parent.width
        anchors.topMargin: 20
        anchors.bottomMargin: 30
        currentIndex: barExp.currentIndex
        Item{
            Layout.fillHeight: true
            Parameters{
            }
        }   
        Item{
            Layout.fillHeight: true
            Results{
            }
        }
    } 
}