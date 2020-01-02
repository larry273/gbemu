import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3

import Interface 1.0



ApplicationWindow {
    visible: true
    width: 800
    height: 480
    title: "Gameboy Emulator"

    Interface {
        id: gameboy
    }



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
                    text: qsTr("CPU")
                    checked: false
                }
                TabButton {
                    text: qsTr("Video Memory")
                    checked: false
                }

            }

            StackLayout {
                id: stackLay
                currentIndex: bar.currentIndex
                width: parent
                height: parent * 0.8
                Item {
                    id: memoryTab
                    ScrollView {
                        id: view
                        anchors.fill: parent
                        TextArea {
                            text: gameboy.memory
                            font.family: "Consolas"
                            anchors.fill: parent
                            readOnly: true
                        }
                    }

                }
                Item {
                    id: cpuTab
                    TextArea {
                        anchors.fill: parent
                        text: gameboy.regVals
                        anchors.topMargin: 124
                        readOnly: true
                        wrapMode: TextEdit.Wrap
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                gameboy.test();
                            }
                        }
                    }

                    Label {
                        id: label
                        x: 8
                        y: 0
                        width: 83
                        height: 19
                        text: qsTr("Registers:")
                        font.bold: true
                    }

                    Label {
                        id: label1
                        x: 8
                        y: 25
                        width: 83
                        height: 19
                        text: qsTr("AF")
                        horizontalAlignment: Text.AlignRight
                    }

                    Label {
                        id: labelAF
                        x: 97
                        y: 25
                        width: 83
                        height: 19
                        text: gameboy.regAF
                        horizontalAlignment: Text.AlignHCenter
                    }

                    Label {
                        id: label3
                        x: 8
                        y: 50
                        width: 83
                        height: 19
                        text: qsTr("BC")
                        horizontalAlignment: Text.AlignRight
                    }

                    Label {
                        id: labelBC
                        x: 97
                        y: 50
                        width: 83
                        height: 19
                        text: gameboy.regBC
                        horizontalAlignment: Text.AlignHCenter
                    }

                    Label {
                        id: label5
                        x: 8
                        y: 75
                        width: 83
                        height: 19
                        text: qsTr("DE")
                        horizontalAlignment: Text.AlignRight
                    }

                    Label {
                        id: labelDE
                        x: 97
                        y: 75
                        width: 83
                        height: 19
                        text: gameboy.regDE
                        horizontalAlignment: Text.AlignHCenter
                    }

                    Label {
                        id: label7
                        x: 186
                        y: 25
                        width: 83
                        height: 19
                        text: qsTr("HL")
                        horizontalAlignment: Text.AlignRight
                    }

                    Label {
                        id: labelHL
                        x: 275
                        y: 25
                        width: 83
                        height: 19
                        text: gameboy.regHL
                        horizontalAlignment: Text.AlignHCenter
                    }

                    Label {
                        id: label9
                        x: 186
                        y: 50
                        width: 83
                        height: 19
                        text: qsTr("SP")
                        horizontalAlignment: Text.AlignRight
                    }

                    Label {
                        id: labelSP
                        x: 275
                        y: 50
                        width: 83
                        height: 19
                        text: gameboy.regSP
                        horizontalAlignment: Text.AlignHCenter
                    }

                    Label {
                        id: label11
                        x: 186
                        y: 75
                        width: 83
                        height: 19
                        text: qsTr("PC")
                        horizontalAlignment: Text.AlignRight
                    }

                    Label {
                        id: labelPC
                        x: 275
                        y: 75
                        width: 83
                        height: 19
                        text: gameboy.regPC
                        horizontalAlignment: Text.AlignHCenter
                    }
                }
                Item {
                    id: videoMemory
                    TextArea {
                        anchors.fill: parent
                        text: "Insert Video Memory Here"
                        readOnly: true
                    }
                }
            }
        }

    }
}
