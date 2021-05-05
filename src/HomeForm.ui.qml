import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

Page {
    id: mainPage
    title: qsTr("Home")

    ListModel {
        id: fruitModel

        ListElement {
            name: "Apple"
            cost: 2.45
        }
        ListElement {
            name: "Orange"
            cost: 3.25
        }
        ListElement {
            name: "Banana"
            cost: 1.95
        }
    }

    RowLayout {
        id: mainRow
        spacing: 2
        anchors.fill: parent

        ScrollView {
            id: scrollView
            Layout.fillHeight: true
            ScrollBar.vertical.policy: ScrollBar.AlwaysOff
            ColumnLayout {
                spacing: 20
                Repeater {
                    model: appManager.getFilterModel()
                    GroupBox {
//                        id: model.modelData
                        title: group
                        ColumnLayout{
                            Repeater {
                                model: tagcounts
                                CheckBox {
                                    text: modelData.getTagName() + " (" + modelData.getCount() + ")"
//                                        text: tagname + " (" + tagvalue + ")"
                                    onClicked: {
                                        switch (group)
                                        {
                                            case "OBJECT":
                                                if (checkState == Qt.Checked)
                                                {
                                                    appManager.getModel().addAcceptedObject(modelData.getTagName() )
                                                }
                                                else if (checkState == Qt.Unchecked)
                                                {
                                                    appManager.getModel().removeAcceptedObject(modelData.getTagName() )
                                                }
                                            break;
//                                            case "Dates":
//                                                if (checkState == Qt.Checked)
//                                                {
//                                                    appManager.getModel().addAcceptedObject(modelData.getTagName() )
//                                                }
//                                                else if (checkState == Qt.Unchecked)
//                                                {
//                                                    appManager.getModel().removeAcceptedObject(modelData.getTagName() )
//                                                }
//                                                break;
                                            case "INSTRUME":
                                                if (checkState == Qt.Checked)
                                                {
                                                    appManager.getModel().addAcceptedInstrument(modelData.getTagName() )
                                                }
                                                else if (checkState == Qt.Unchecked)
                                                {
                                                    appManager.getModel().removeAcceptedInstrument(modelData.getTagName() )
                                                }
                                            break;
                                            case "FILTER":
                                                if (checkState == Qt.Checked)
                                                {
                                                    appManager.getModel().addAcceptedFilter(modelData.getTagName() )
                                                }
                                                else if (checkState == Qt.Unchecked)
                                                {
                                                    appManager.getModel().removeAcceptedFilter(modelData.getTagName() )
                                                }
                                            break;
                                            case "FILEEXT":
                                                if (checkState == Qt.Checked)
                                                {
                                                    appManager.getModel().addAcceptedExtension(modelData.getTagName() )
                                                }
                                                else if (checkState == Qt.Unchecked)
                                                {
                                                    appManager.getModel().removeAcceptedExtension(modelData.getTagName() )
                                                }
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                    }
//                    Text {
//                        text: "I'm group " + index + ":" + model.modelData
//                        color: "white"
//                    }
                }
//                GroupBox {
//                    id: groupBox1
//                    title: qsTr("Group Box 1")
//                    ColumnLayout {
//                        anchors.fill: parent
//                        CheckBox {
//                            text: qsTr("E-mail")
//                        }
//                        CheckBox {
//                            text: qsTr("Calendar")
//                        }
//                        CheckBox {
//                            text: qsTr("Contacts")
//                        }
//                    }
//                }
            }
        }

        ColumnLayout{
        spacing: 20
        GridView {
            id: gridView
            clip: true
            Layout.fillHeight: true
            Layout.fillWidth: true
            cellWidth: zoomSlider.value * 2.5; cellHeight: zoomSlider.value * 2.5
            model: appManager.getModel()
//            function onRowsInserted() {console.log("Rows inserted")}
            Connections {
                target: model
                function onrowsInserted(parent, first, last) {console.log("Rows inserted")}
            }
//            currentIndex = index
            highlightFollowsCurrentItem: true
            highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
            focus: true

            delegate: Component {
                id: gridDelegate
                Item {
                    width: zoomSlider.value * 2.5
                    height: zoomSlider.value * 2.5

//                    ColumnLayout {
                        Image {
                            id: thumbnail
                            height: zoomSlider.value * 2
                            width: zoomSlider.value * 2
                            y: 20
                            fillMode: Image.PreserveAspectFit

                            source: decoration
                            anchors.horizontalCenter: parent.horizontalCenter
//                            anchors.centerIn: parent
                            Layout.alignment: Qt.AlignTop

                        }
                        Text {
                            text: display
                            color: "white"
                            anchors { top: thumbnail.bottom; horizontalCenter: parent.horizontalCenter }
//                            anchors.centerIn: parent

                        }
//                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked:
                        {
                        gridView.currentIndex = index
//                        detailsGrid.currentIndex = index

                        filenameLabel.text = display + " "
                        objectLabel.text = object + " "
                        instrumentLabel.text = instrument + " "
                        filterLabel.text = filter + " "
                        raLabel.text = ra + " "
                        decLabel.text = dec + " "
                        imagesizeLabel.text = imagexsize + " x " + imageysize
                        gainLabel.text = gain + " "
                        exposureLabel.text = exposure + " "
                        dateLabel.text = date + " "
                        tempLabel.text = ccd + " "
                        offsetLabel.text = offset + " "
                        bayerLabel.text = bayermode + " "
                        }
                    }
                }
            }
//            highlight: Rectangle {
//                        color: 'grey'
//                        Text {
//                            anchors.centerIn: parent
//                            text: 'Hello '
//                            color: 'white'
//                        }
//                    }
//            onCurrentItemChanged: console.log(model.get(list.currentIndex).name + ' selected')
            onCurrentItemChanged: console.log(' selected')
        }
        Slider {
            id: zoomSlider
            from: 60
            value: 100
            to: 200
            height: 50
        }
        }
//        Rectangle{
//            Layout.fillHeight: true
//            Layout.preferredWidth: 200
//            color: "blue"
//        }
        Rectangle { // This was "Item" instead of "Rctangle"
            id: detailsItem
            Layout.fillHeight: true
            Layout.preferredWidth: 200
            color: "transparent"
            border.width: 1
//            width: 200
            Grid {
                id: detailsGrid
//                model: appManager.getModel()
//            columns: 2
//                Button {
//                    text: qsTr("Click me")
//                    onClicked: appManager.hello("World")
//                }
//                Label {
//                    text: qsTr("You are on the home page.")
//                    color: "red"
//                }
//                delegate: Grid {
                columns: 2
                spacing: 5
                    Label {
                        text: qsTr("FileName: ")
                        color: "white"
                    }
                    Label {
                        id: filenameLabel
//                        text: "display"

                        color: "white"
                    }
                    Label {
                        text: qsTr("Object: ")
                        color: "white"
                    }
                    Label {
                        id: objectLabel
//                        text: " " + object
                        color: "white"
                    }
                    Label {
                        text: qsTr("Instrument: ")
                        color: "white"
                    }
                    Label {
                        id: instrumentLabel
//                        text: " " + instrument
                        color: "white"
                    }
                    Label {
                        text: qsTr("Filter: ")
                        color: "white"
                    }
                    Label {
                        id: filterLabel
//                        text: " " + filter
                        color: "white"
                    }
                    Label {
                        text: qsTr("RA: ")
                        color: "white"
                    }
                    Label {
                        id: raLabel
//                        text: " " + ra
                        color: "white"
                    }
                    Label {
                        text: qsTr("Dec: ")
                        color: "white"
                    }
                    Label {
                        id: decLabel
//                        text: " " + dec
                        color: "white"
                    }
                    Label {
                        text: qsTr("Image Size: ")
                        color: "white"
                    }
                    Label {
                        id: imagesizeLabel
//                        text: " " + imagexsize + " x " + imageysize
                        color: "white"
                    }
                    Label {
                        text: qsTr("Gain: ")
                        color: "white"
                    }
                    Label {
                        id: gainLabel
//                        text: " " + gain
                        color: "white"
                    }
                    Label {
                        text: qsTr("Exposure: ")
                        color: "white"
                    }
                    Label {
                        id: exposureLabel
//                        text: " " + exposure
                        color: "white"
                    }
                    Label {
                        text: qsTr("Date: ")
                        color: "white"
                    }
                    Label {
                        id: dateLabel
//                        text: " " + date
                        color: "white"
                    }
                    Label {
                        text: qsTr("Temp: ")
                        color: "white"
                    }
                    Label {
                        id: tempLabel
//                        text: " " + ccd
                        color: "white"
                    }
                    Label {
                        text: qsTr("Offset: ")
                        color: "white"
                    }
                    Label {
                        id: offsetLabel
//                        text: " " + offset
                        color: "white"
                    }
                    Label {
                        text: qsTr("Bayer: ")
                        color: "white"
                    }
                    Label {
                        id: bayerLabel
//                        text: " " + bayermode
                        color: "white"
                    }
            }
        }
    }
}
