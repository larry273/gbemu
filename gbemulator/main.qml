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
        objectName: "gameboy"

    }

    Shortcut {
        sequence: "Right"
        onActivated: gameboy.isWait = !gameboy.isWait
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
                    checked: true
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
                        font.family: "Consolas"
                        font.bold: true
                    }

                    Label {
                        id: label1
                        x: 8
                        y: 25
                        width: 27
                        height: 19
                        text: qsTr("AF:\nBC:\nDE:")
                        font.family: "Consolas"
                        horizontalAlignment: Text.AlignRight
                    }

                    Label {
                        id: labelAF
                        x: 35
                        y: 25
                        width: 83
                        height: 19
                        text: gameboy.regSet1
                        font.family: "Consolas"
                        horizontalAlignment: Text.AlignLeft
                    }

                    ScrollView {
                        id: view
                        x: -300
                        y: 456
                        anchors.rightMargin: 8
                        anchors.leftMargin: 0
                        anchors.bottomMargin: 8
                        anchors.topMargin: 220
                        anchors.fill: parent
                        TextArea {
                            text: gameboy.memory
                            font.pointSize: 10
                            font.family: "Consolas"
                            anchors.fill: parent
                            readOnly: true
                        }
                    }

                    Label {
                        id: label7
                        x: 105
                        y: 25
                        width: 42
                        height: 19
                        text: qsTr("HL:\nSP:\nPC:")
                        font.family: "Consolas"
                        horizontalAlignment: Text.AlignRight
                    }

                    Label {
                        id: labelHL
                        x: 153
                        y: 25
                        width: 83
                        height: 19
                        text: gameboy.regSet2
                        font.family: "Consolas"
                        horizontalAlignment: Text.AlignLeft
                    }

                    Label {
                        id: label2
                        x: 8
                        y: 100
                        width: 83
                        height: 19
                        text: qsTr("Flags:")
                        font.family: "Consolas"
                        font.bold: true
                    }

                    Label {
                        id: flagVals
                        x: 8
                        y: 125
                        width: 108
                        height: 19
                        text: gameboy.flags
                        font.family: "Consolas"
                        horizontalAlignment: Text.AlignLeft
                    }

                    Label {
                        id: label4
                        x: 200
                        y: 100
                        width: 88
                        height: 19
                        text: qsTr("OpCodes:")
                        font.family: "Consolas"
                        font.bold: true
                    }

                    Button {
                        id: buttonDebug
                        x: 404
                        y: 17
                        width: 83
                        height: 40
                        text: qsTr("Debug")
                        font.family: "Consolas"
                        background: Rectangle {
                            color: gameboy.debug ? "red" : "grey"
                        }
                        onClicked: {
                            gameboy.debug = !gameboy.debug
                        }
                    }

                    Button {
                        id: buttonPC
                        x: 404
                        y: 58
                        width: 83
                        height: 40
                        text: qsTr("PC++")
                        font.family: "Consolas"
                        onClicked: gameboy.isWait = !gameboy.isWait
                    }

                    TextInput {
                        id: pcInput
                        x: 426
                        y: 124
                        width: 64
                        height: 26
                        text: gameboy.pcInput
                        font.pointSize: 12
                        onTextChanged: gameboy.pcInput = text

                        font.family: "Consolas"
                        horizontalAlignment: Text.AlignHCenter
                    }

                    ScrollView {
                        id: opCodeView
                        x: 201
                        y: 127
                        width: 207
                        height: 87
                        anchors.rightMargin: 94
                        anchors.bottomMargin: 196
                        anchors.leftMargin: 200
                        anchors.topMargin: 121
                        TextArea {
                            text: gameboy.opcode
                            anchors.rightMargin: -41
                            anchors.bottomMargin: -3
                            anchors.leftMargin: 0
                            anchors.topMargin: 0
                            font.family: "Consolas"
                            anchors.fill: parent
                            readOnly: true
                        }
                    }

                    Label {
                        id: label6
                        x: 3
                        y: 195
                        width: 88
                        height: 19
                        text: qsTr("Memory:")
                        font.bold: true
                    }

                    Label {
                        id: label8
                        x: 406
                        y: 101
                        width: 83
                        height: 19
                        text: qsTr("PC Break:")
                        font.bold: true
                        font.family: "Consolas"
                    }

                    Label {
                        id: labelregVid
                        x: 277
                        y: 25
                        width: 83
                        height: 69
                        text: gameboy.regVid
                        horizontalAlignment: Text.AlignLeft
                        font.family: "Consolas"
                    }

                    Label {
                        id: label10
                        x: 229
                        y: 25
                        width: 48
                        height: 69
                        text: qsTr("LCDC:\nSTAT:\nLY:\n")
                        horizontalAlignment: Text.AlignRight
                        font.family: "Consolas"
                    }

                }
                Item {
                    id: memoryTab

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
