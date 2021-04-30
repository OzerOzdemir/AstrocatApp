import QtQuick.Controls 2.5
import QtQuick 2.4
import QtQuick.Layouts 1.3

Dialog {
    title: qsTr('Astrocat')
    width: 500
    height: 350
    standardButtons: Dialog.Ok

ListModel {
    id: licenseModel

    ListElement {
        name: "CFITSIO"
        license: "qrc:/Licenses/resources/Licenses/CFITSIO.lic"
    }
    ListElement {
        name: "CMINPACK"
        license: "qrc:/Licenses/resources/Licenses/CMINPACK.lic"
    }
    ListElement {
        name: "LCMS"
        license: "qrc:/Licenses/resources/Licenses/LCMS.lic"
    }
    ListElement {
        name: "LZ4"
        license: "qrc:/Licenses/resources/Licenses/LZ4.lic"
    }
    ListElement {
        name: "PCL"
        license: "qrc:/Licenses/resources/Licenses/PCL.lic"
    }
    ListElement {
        name: "PTHREADS"
        license: "qrc:/Licenses/resources/Licenses/PTHREADS.lic"
    }
    ListElement {
        name: "QT"
        license: "qrc:/Licenses/resources/Licenses/QT.lic"
    }
    ListElement {
        name: "RFC6234"
        license: "qrc:/Licenses/resources/Licenses/RFC6234.lic"
    }
    ListElement {
        name: "ZLIB"
        license: "qrc:/Licenses/resources/Licenses/ZLIB.lic"
    }

}
    Page {
        anchors.fill: parent

        header: RowLayout
        {
            Label {
            text: "Version: " + "0.0.1" // TODO: Read this from the #define VERSION
            }
        }

        ColumnLayout
        {
            anchors.fill: parent
            Label {
                wrapMode: Text.WordWrap
                text: qsTr("Copyright 2021, Astrocat.app\nVisit https://astrocat.app\nAstrocat uses the following Libraries")
            }

            RowLayout
            {
                ListView {
                    id: libsList

                    model: licenseModel
                    clip: true
                    Layout.fillHeight: true
                    width: 100
//                    Layout.fillWidth: true
                    highlightFollowsCurrentItem: true
                    highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
                    focus: true

                    delegate: Row {
                        width: 80; height: 20
                        Text {
                            text: name
                            color: "white"
                        }
                        MouseArea {
                            anchors.fill: parent    //BUG: If we use this, we get a warning, if we don't, mouse does not work
                            onClicked: libsList.currentIndex = index
                        }
                    }
                }

                Label{
                    id: libsDetails
                    Layout.fillWidth: true
                    text: "Liense Text Here" // TODO: Read the license text from the resource file of the selected item
                }
            }
        }

    }
}
