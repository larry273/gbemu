import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3

import Interface 1.0



ApplicationWindow {
    id: applicationWindow
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
        width: 800
        anchors.fill: parent
        columns: 2

        ColumnLayout{
            anchors.fill: parent

            Image {
                id: gbbg
                fillMode: Image.PreserveAspectFit
                source: "overlay.jpg"
            }

            Image {
                id: image
                property bool counter: false
                Layout.preferredHeight: 144
                Layout.preferredWidth: 160

                anchors.left: gbbg.right
                anchors.top: gbbg.top
                anchors.leftMargin: -232
                anchors.topMargin: 30

                z: 10
                source: "image://live/image"
                cache: false


                function reload() {
                    counter = !counter
                    source = "image://live/image?id=" + counter
                }
            }


            Connections{
                    target: imageProvider
                    onImageChanged: image.reload()
            }
        }








        ColumnLayout {
            TabBar {
                id: bar
                width: parent
                height: parent * 0.2

                TabButton {
                    text: qsTr("CPU")
                    width: implicitWidth
                }

                TabButton {
                    text: qsTr("Memory")
                    checked: false
                    width: implicitWidth

                }
                TabButton {
                    text: qsTr("Video Memory")
                    checked: false
                    width: implicitWidth
                }

            }

            StackLayout {
                id: stackLay
                currentIndex: bar.currentIndex
                width: parent
                height: parent * 0.8

                Item {
                    id: cpuTab


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

                    Label {
                        id: label2
                        x: 8
                        y: 113
                        width: 83
                        height: 19
                        text: qsTr("Flags:")
                        font.bold: true
                    }

                    Label {
                        id: flagVals
                        x: 72
                        y: 135
                        width: 108
                        height: 19
                        text: gameboy.flags
                        horizontalAlignment: Text.AlignLeft
                    }

                    Label {
                        id: label4
                        x: 8
                        y: 178
                        width: 88
                        height: 19
                        text: qsTr("Instruction:")
                        font.bold: true
                    }

                    Button {
                        id: buttonDebug
                        x: 363
                        y: 178
                        width: 124
                        height: 40
                        text: qsTr("Debug")
                        checkable: true
                        background: Rectangle {
                            color: buttonDebug.checked ? "red" : "grey"
                        }
                        onClicked: {
                            gameboy.debug = !gameboy.debug
                        }
                    }

                    Button {
                        id: buttonPC
                        x: 363
                        y: 224
                        width: 124
                        height: 40
                        text: qsTr("PC++")

                        onClicked: gameboy.isWait = !gameboy.isWait
                    }

                    Label {
                        id: opCode
                        x: 20
                        y: 214
                        width: 101
                        height: 19
                        text: gameboy.opcode
                        horizontalAlignment: Text.AlignLeft
                    }

                    Label {
                        id: opCodeMemory
                        x: 44
                        y: 245
                        width: 149
                        height: 19
                        text: "Memory: value"
                        horizontalAlignment: Text.AlignLeft
                    }
                }
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
                    id: videoMemory
                    ScrollView {
                        id: vidMemview
                        anchors.fill: parent
                        TextArea {
                            text: gameboy.vidMemory
                            font.family: "Consolas"
                            anchors.fill: parent
                            readOnly: true
                        }
                    }
                }
            }
        }
    }



}
