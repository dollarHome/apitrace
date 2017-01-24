import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.1
import ApiTrace 1.0

Item {
    property QTexturesModel texturesModel
    RowLayout {
        anchors.fill: parent
        TableView {
            Layout.fillWidth: true
            Layout.minimumWidth: parent.width/3
            Layout.fillHeight: true
            model: texturesModel.textures
            TableViewColumn {
                role: "name"
                title: "Texture Id"
                width: parent.width
            }
        }
        Item {
            Layout.fillWidth: true
            Layout.minimumWidth: (parent.width/3) * 2
            Layout.fillHeight: true
            Layout.minimumHeight: parent.height
            Rectangle {
                anchors.fill: parent
                Button {
                    id: texButton
                    width: parent.width
                    height: 25
                    text: "Texture"
                }

                ColumnLayout {
                   anchors.top: texButton.bottom
                   anchors.topMargin: 10
                   anchors.horizontalCenter: parent.horizontalCenter
                Rectangle {
                   id: textureImage
                   width: 300
                   height: 300
                   visible: true
                   border.width: 2
                }

                Item {
                   id: textureDetails
                   anchors.top: textureImage.bottom
                   anchors.topMargin: 10
                   ColumnLayout {
                    Text {
                       text: "Texture Dimensions:"
                    }

                    Text {
                       text: "Texture Format:"
                    }

                     RowLayout {
                        Text {
                           text: "MipMap Level:"
                        }

                        ComboBox {
                           id: mipLevel
                           model: [0, 1, 2, 3]
                        }
                     }
                   }

                }
                }
            }
        }
    }
}
