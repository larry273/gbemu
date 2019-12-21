import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3


ApplicationWindow {
    visible: false
    width: 800
    height: 480
    title: "Gameboy Emulator"

    //menu containing two menu items
    menuBar: MenuBar {
        Menu {
            title: qsTr("File")
            MenuItem {
                text: qsTr("&Load")
                onTriggered: console.log("Load action triggered");
            }
            MenuItem {
                text: qsTr("&Close File")
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
        anchors.fill: parent
        columns: 2



        Image {
            id: image
            width: 100
            height: 100
            fillMode: Image.PreserveAspectFit
            source: "qrc:/qtquickplugin/images/template_image.png"
        }

        ColumnLayout {
            TabBar {
                id: bar
                width: parent
                height: parent * 0.2
                TabButton {
                    text: qsTr("Memory")
                }

                TabButton {
                    text: qsTr("Registers")
                }
                TabButton {
                    text: qsTr("Video Memory")
                }

            }

            StackLayout {
                id: stackLay
                currentIndex: bar.currentIndex
                width: parent
                height: parent * 0.8
                Item {
                    id: homeTab
                    Rectangle {
                        color: 'red'
                        anchors.fill: parent
                    }
                }
                Item {
                    id: discoverTab
                    Rectangle {
                        color: 'blue'
                        anchors.fill: parent
                    }
                }
                Item {
                    id: activityTab
                    Rectangle {
                        color: 'green'
                        anchors.fill: parent
                    }
                }
            }
        }

    }
}
