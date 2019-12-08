import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3


ApplicationWindow {
    id: applicationWindow
    visible: false
    width: 640
    height: 480
    title: qsTr("Gameboy Emulator")

    //menu containing two menu items
    menuBar: MenuBar {
        Menu {
            title: qsTr("File")
            MenuItem {
                text: qsTr("&Load")
                onTriggered: console.log("Load action triggered");
            }
            MenuItem {
                text: qsTr("&Close")
                onTriggered: console.log("close action triggered");
            }
            MenuItem {
                text: qsTr("Exit")
                onTriggered: Qt.quit();
            }
        }
        Menu {
            title: qsTr("Debug")
            MenuItem {
                text: qsTr("&Memory")
                onTriggered: console.log("memory action triggered");
            }
            MenuItem {
                text: qsTr("&Debug")
                onTriggered: console.log("debug action triggered");
            }
            MenuItem {
                text: qsTr("Performance")
                onTriggered: console.log("performance action triggered");
            }
        }
    }

    GridLayout {
        id: gridLayout
        x: 237
        y: 149
        width: 210
        height: 166
    }




}
