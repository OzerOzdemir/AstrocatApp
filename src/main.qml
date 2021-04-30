import QtQuick 2.12
import QtQuick.Controls 2.5

ApplicationWindow {
    id: window
    width: 640
    height: 480
    visible: true
    title: qsTr("Stack")

    AboutForm {
            id: about_form
        }

    menuBar: MenuBar {
            Menu {
                title: qsTr("&File")
                Action { text: qsTr("&New...") }
                Action { text: qsTr("&Open...") }
                Action { text: qsTr("&Save") }
                Action { text: qsTr("Save &As...") }
                MenuSeparator { }
                Action { text: qsTr("&Quit") }
            }
            Menu {
                title: qsTr("&Edit")
                Action { text: qsTr("Cu&t") }
                Action { text: qsTr("&Copy") }
                Action { text: qsTr("&Paste") }
            }
            Menu {
                title: qsTr("&Help")
                MenuItem {
                    text: qsTr('&About')
                    onTriggered: about_form.open()
                }
            }
        }

    header: ToolBar {
        contentHeight: toolButton.implicitHeight

        ToolButton {
            id: toolButton
            text: stackView.depth > 1 ? "\u25C0" : "\u2630"
            font.pixelSize: Qt.application.font.pixelSize * 1.6
            onClicked: {
                if (stackView.depth > 1) {
                    stackView.pop()
                } else {
                    drawer.open()
                }
            }
        }

        Label {
            text: stackView.currentItem.title
            anchors.centerIn: parent
        }
    }

    Drawer {
        id: drawer
        width: Math.max(200, window.width * 0.6)
        height: window.height

        Column {
            anchors.fill: parent

            ItemDelegate {
                text: qsTr("Page 1")
                width: parent.width
                onClicked: {
                    stackView.push("Page1Form.ui.qml")
                    drawer.close()
                }
            }
            ItemDelegate {
                text: qsTr("Page 2")
                width: parent.width
                onClicked: {
                    stackView.push("Page2Form.ui.qml")
                    drawer.close()
                }
            }
        }
    }

    StackView {
        id: stackView
        initialItem: "HomeForm.ui.qml"
        anchors.fill: parent
    }

//    Component {
//        id: fruitDelegate
////        Row {
////            spacing: 10
////            Text {
////                text: id
////                color: "gray"
////            }
////            Text {
////                text: fullpath
////                color: "white"
////            }
////        }
//        Item {
////                width: 350
////                height: width

//                Column {
//                    Image {
//                        height: 200
//                        width: 200
//                        source: decoration
//                    }
//                    Text {
//                        text: display
//                    }
//                }
//                MouseArea {
//                                    anchors.fill: parent
//                                    onClicked: currentIndex = index
//                                }
//            }
//    }
//    ListModel {
//        id: fruitModel

//        ListElement {
//            name: "Apple"
//            cost: 2.45
//            attributes: [
//                ListElement { description: "Core" },
//                ListElement { description: "Deciduous" }
//            ]
//        }
//        ListElement {
//            name: "Orange"
//            cost: 3.25
//            attributes: [
//                ListElement { description: "Citrus" }
//            ]
//        }
//        ListElement {
//            name: "Banana"
//            cost: 1.95
//            attributes: [
//                ListElement { description: "Tropical" },
//                ListElement { description: "Seedless" }
//            ]
//        }
//    }
}
