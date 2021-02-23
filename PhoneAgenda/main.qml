import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQml 2.12

Window {
    width: 800
    height: 600
    visible: true
    title: qsTr("Phone Agenda")
    property bool startedRequest: false
    property int contactIndex: -1

    /*
    ListModel {
        id: agendaItemsModel
        ListElement { name: "Radu-Mihai Rotariu"; phoneNumber: "0754909690" }
        ListElement { name: "Claudiu-Serban Zalinca"; phoneNumber: "0777333111" }
        ListElement { name: "Laurentiu Erhan"; phoneNumber: "0755666999" }
    }
    */

    Connections {
        target: ModelManager
        onIsRequestStartedChanged:
        {
            startedRequest = ModelManager.isRequestStarted
            importLoadingIndicator.visible = startedRequest
        }
    }

    function checkPhoneNumber(testString)
    {
        if(testString.match(/[0-9]/) && testString.length === 10)
            return true
        else
            return false
    }

    function checkName(testString)
    {
        if(testString.match(/^[A-Z][a-z]+\s+[A-Z][a-z]+/g))
            return true
        else
            return false
    }

    function appendButtonFunction()
    {
        if(checkName(inputName.text) && checkPhoneNumber(inputPhoneNumber.text))
        {
            ModelManager.addItem(inputName.text, inputPhoneNumber.text)
            inputName.text = ""
            inputPhoneNumber.text = ""
        }
    }

    Dialog {
        id: confirmDeleteContactDialog

        title: "Delete contact?"
        width: 300
        height: 150
        anchors.centerIn: parent

        ColumnLayout {
            id: dialogWindowContainer

            anchors.fill: parent

            Text {
                id: confirmationText
                text: qsTr("Are you sure?")
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 20

                Layout.fillWidth: true
            }

            RowLayout {
                id: deleteContactOptionsContainer

                Layout.fillWidth: true

                Button {
                    id: dialogAcceptButton

                    text: "Yes"
                    font.pixelSize: 20
                    Layout.alignment: Qt.AlignCenter
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    onClicked: {
                        ModelManager.removeItem(contactIndex)
                        contactIndex = -1
                        confirmDeleteContactDialog.accept()
                    }
                }

                Button {
                    id: dialogCancelButton

                    text: "No"
                    font.pixelSize: 20
                    Layout.alignment: Qt.AlignCenter
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    onClicked: confirmDeleteContactDialog.close()
                }
            }
        }
    }

    Component {
        id: itemDelegate

        RowLayout {
            id: itemLayoutContainer

            width: parent.width
            height: 50

            Text {
                id: itemName

                text: name
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignLeft
                font.pointSize: 20
            }

            Text {
                id: itemPhoneNumber

                text: phoneNumber
                font.pointSize: 20
            }

            Button {
                id: deleteItemButton

                Layout.preferredWidth: 20
                Layout.preferredHeight: 20
                Layout.alignment: Qt.AlignRight
                text: "X"
                onClicked: {
                    contactIndex = index
                    confirmDeleteContactDialog.open()
                }
            }
        }
    }

    ColumnLayout {
        id: columnContainer

        anchors.fill: parent
        anchors.bottomMargin: 10
        spacing: 10

        Rectangle {
            id: menuBackgroundRectangle

            Layout.fillWidth: true
            Layout.preferredHeight: 50

            color: "lightgray"

            RowLayout {
                id: menuContainer

                anchors.fill: parent

                Button {
                    id: refresh

                    Layout.alignment: Qt.AlignLeft

                    text: "Refresh"
                    onClicked:
                    {
                        ModelManager.refresh()
                    }
                }

                BusyIndicator {
                    id: importLoadingIndicator

                    Layout.fillHeight: true
                    Layout.alignment: Qt.AlignLeft

                    visible: false
                }

                RowLayout {
                    id: databaseButtons

                    Layout.alignment: Qt.AlignCenter
                    Layout.fillHeight: true

                    Button {
                        id: importDb

                        Layout.alignment: Qt.AlignLeft

                        text: "Import Contacts"
                        onClicked: ModelManager.importDB()
                    }

                    Button {
                        id: exportDb

                        Layout.alignment: Qt.AlignCenter

                        text: "Export Contacts"
                        onClicked: ModelManager.exportDB()
                    }

                    Button {
                        id: clearDb

                        Layout.alignment: Qt.AlignRight

                        text: "Clear Local Cache"
                        onClicked: ModelManager.clearCache()
                    }
                }

                Button {
                    id: exitApp

                    Layout.alignment: Qt.AlignRight

                    text: "Exit Application"
                    onClicked: Qt.quit()
                }
            }
        }

        RowLayout {
            id: inputContainer

            Layout.fillWidth: true
            Layout.rightMargin: 10
            Layout.leftMargin: 10

            TextField {
                id: inputName

                Layout.fillWidth: true

                placeholderText: "Enter name here"
            }

            TextField {
                id: inputPhoneNumber

                placeholderText: "Enter phone number here"
            }

            Button {
                id: appendItemButton

                Layout.alignment: Qt.AlignRight

                text: "Add contact"
                onClicked: appendButtonFunction()
            }
        }

        RowLayout {
            id: sortFilterContainer

            Layout.fillWidth: true
            Layout.rightMargin: 10
            Layout.leftMargin: 10

            RowLayout {
                id: nameSortContainer

                TextField {
                    id: searchName

                    Layout.fillWidth: true

                    placeholderText: "Enter search name"
                    //inputMethodHints: Qt.ImhNoPredictiveText
                    onTextChanged:
                    {
                        ModelManager.searchProxy(0, searchName.text)
                    }
                }

                Button {
                    id: nameAZSortButton

                    Layout.preferredWidth: 40
                    text: "A-Z"
                    onClicked: ModelManager.sortProxy(0, true)
                }

                Button {
                    id: nameZASortButton

                    Layout.preferredWidth: 40
                    text: "Z-A"
                    onClicked: ModelManager.sortProxy(0, false)
                }
            }

            RowLayout {
                id: phoneNumberSortContainer

                TextField {
                    id: searchPhoneNumber

                    placeholderText: "Enter search phone number"
                    //inputMethodHints: Qt.ImhNoPredictiveText
                    onTextChanged:
                    {
                        ModelManager.searchProxy(1, searchPhoneNumber.text)
                    }
                }

                Button {
                    id: phoneNumberAZSortButton

                    Layout.alignment: Qt.AlignRight
                    Layout.preferredWidth: 40
                    text: "A-Z"
                    onClicked: ModelManager.sortProxy(1, true)
                }

                Button {
                    id: phoneNumberZASortButton

                    Layout.alignment: Qt.AlignRight
                    Layout.preferredWidth: 40
                    text: "Z-A"
                    onClicked: ModelManager.sortProxy(1, false)
                }
            }
        }

        Rectangle {
            id: listviewBackgroundRectangleContainer

            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.preferredHeight: 100
            Layout.preferredWidth: 100
            Layout.rightMargin: 10
            Layout.leftMargin: 10

            border.width: 1
            border.color: "black"

            ListView {
                id: contactsList

                anchors.fill: parent
                anchors.leftMargin: 5
                anchors.rightMargin: 5

                clip: true

                model: ModelManager.proxyModel
                delegate: itemDelegate
            }
        }
    }
}
