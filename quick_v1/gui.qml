import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import com.FileRW 1.0



Rectangle {
    id: widget
    height: 200
    width: 640
    color: "#ffecd2"
    border.width: 5
    border.color: "#c2e9fb"
    visible: true

    Rectangle{
        id: taskPlain
        width: widget.width - parent.border.width*2
        height: widget.height * 0.66
        x: widget.border.width
        y: widget.border.width
        visible: true
        clip: true
        gradient: Gradient{
            GradientStop { position: 0.0; color: "#fbc2eb" }
            GradientStop { position: 1.0; color: "#a6c1ee" }
        }


        ScrollView{
            id: scroll
            anchors.fill: parent
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
            ScrollBar.vertical.policy: ScrollBar.AsNeeded
            clip: true


            TextEdit {
                id: taskText
                x: taskPlain.x + widget.border.width
                y: taskPlain.y + widget.border.width
                width: taskPlain.width - widget.border.width*2
                height: taskPlain.height - widget.border.width*2
                font.pixelSize: 18
                wrapMode: TextEdit.WordWrap
                verticalAlignment: TextEdit.AlignTop
                ToolTip.visible: !focus
                ToolTip.text: qsTr("Введите новую задачу")

            }

        }

    }

    Label{
         id: lableDate
         x: widget.border.width
         anchors.top: taskPlain.bottom
         text: qsTr("Плановая дата:")

    }

    Rectangle{
        id:dateRec
        x: widget.border.width
        anchors.top: lableDate.bottom
        height: progress.height
        width: 150
        visible: true
        gradient: Gradient{
            GradientStop { position: 0.0; color: "#fad0c4" }
            GradientStop { position: 1.1; color: "#ffd1ff" }
        }

        TextInput{
            id: date
            anchors.fill: parent
            visible: true
            font.pixelSize: height/2
            verticalAlignment: TextInput.AlignVCenter
            horizontalAlignment: TextInput.AlignHCenter
            inputMethodHints: Qt.ImhDate
            inputMask: "99.99.9999"
            text: new Date().toLocaleDateString("dd.MM.yyyy")

        }
    }

    Label{
         id: lableProgress
         anchors.top: taskPlain.bottom
         anchors.left: progress.left
         text: qsTr("Прогресс:")

    }

    SpinBox{
        id: progress
        anchors.top: lableProgress.bottom
        anchors.right: taskPlain.right
        from: 0
        to: 10
        font.pointSize: height/2
        font.bold: true
    }

    Button{
        signal createdTask(string task, string date, string status)

        id: addTask
        objectName: "addButton"
        anchors.top: taskPlain.bottom
        anchors.bottom: dateRec.bottom
        anchors.left: dateRec.right
        anchors.right: progress.left
        text: qsTr("Добавить")
        font.pixelSize: height/2

        contentItem: Text {
            text: addTask.text
            font: addTask.font
            color: addTask.down ? "white" : "black"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
        }

        background: Rectangle {
            width: parent.width
            height: parent.height
            gradient: Gradient{
                GradientStop { position: 0.0; color: "#84fab0" }
                GradientStop { position: 1.1; color: "#8fd3f4" }
            }
        }

        onClicked: {
            if(taskText.text.length){
                createdTask(taskText.text, date.text, progress.value.toString())
                taskText.clear()
                date.text = new Date().toLocaleDateString("dd.MM.yyyy")
                progress.value = 0
            }
        }

    }

FileRW{
    id: file
}

}
