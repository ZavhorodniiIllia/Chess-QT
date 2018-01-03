import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.2
import QtQuick.Window 2.2

ApplicationWindow {
    title: qsTr("Chess")
    id: screen_1
    visible: true
    width: 800
    height: 600

    property int squareSize: 70


    Item {
        id: gameBoard1
        x: 0
        y: 0
        width : logic.boardSize * squareSize
        height: logic.boardSize * squareSize

        Image {
            source: "/images/chess_board.jpg"
            height: gameBoard1.height
            width: gameBoard1.width
        }
    }


    Button {
        id: startButton
        anchors.left: gameBoard1.right
        anchors.right: parent.right
        anchors.leftMargin: 10
        anchors.rightMargin: 10

        text: "Start"

        onClicked: {
            logic.set_figures();
            screen_1.hide()
            screen_2.show()
        }
    }

    Button{
        id: loadButton
        y: 30
        anchors.left: gameBoard1.right
        anchors.right: parent.right
        anchors.leftMargin: 10
        anchors.rightMargin: 10

        text: "Load"

        onClicked: {
            logic.set_figures();
            screen_3.show()
            screen_1.hide()
        }
    }

    ApplicationWindow {
        id: screen_2
        visible: false
        width: 800
        height: 600
        property int squareSize: 70

        property var images: [
          {'imgPath' : "/images/white_king.svg"},
          {'imgPath' : "/images/white_queen.svg"},
          {'imgPath' : "/images/white_bishop.svg"},
          {'imgPath' : "/images/white_knight.svg"},
          {'imgPath' : "/images/white_rook.svg"},
          {'imgPath' : "/images/white_pawn.svg"},

          {'imgPath' : "/images/black_king.svg"},
          {'imgPath' : "/images/black_queen.svg"},
          {'imgPath' : "/images/black_bishop.svg"},
          {'imgPath' : "/images/black_knight.svg"},
          {'imgPath' : "/images/black_rook.svg"},
          {'imgPath' : "/images/black_pawn.svg"},    ]

        Item {
          id: gameBoard
          x: 0
          y: 0
          width : logic.boardSize * squareSize
          height: logic.boardSize * squareSize

          Image {
            source: "/images/chess_board.jpg"
            height: gameBoard.height
            width: gameBoard.width
          }

          Repeater {

            model: logic

            Image {

              height: squareSize
              width : squareSize

              x: squareSize * positionX
              y: squareSize * positionY

              source: screen_2.images[type].imgPath

              MouseArea {
                anchors.fill: parent
                drag.target: parent

                property int startX: 0
                property int startY: 0

                onPressed: {
                  startX = parent.x;
                  startY = parent.y;
                }

                onReleased: {
                  var fromX = (startX / squareSize)+0.1;
                  var fromY = (startY / squareSize)+0.1;
                  var toX   = (parent.x + mouseX) / squareSize;
                  var toY   = (parent.y + mouseY) / squareSize;
                  if (!logic.move(fromX, fromY, toX, toY, type)) {
                    parent.x = startX+1;
                    parent.y = startY+1;
                  }
                }
              }
            }
          }
        }


        Button {
          id: stopButton
          anchors.left: gameBoard.right
          anchors.right: parent.right
          anchors.leftMargin: 10
          anchors.rightMargin: 10

          text: "Stop"

          onClicked: {
            screen_1.show()
            screen_2.close()
          }
        }

        Button{
            id: saveButton
            y: 30
            anchors.left: gameBoard.right
            anchors.right: parent.right
            anchors.leftMargin: 10
            anchors.rightMargin: 10

            text: "Save"

            onClicked: {
                logic.save();
            }
        }

    }
    Window {
        id: screen_3
        //visible: false
        width: 800
        height: 600

        property int squareSize: 70

        property var images: [
          {'imgPath' : "/images/white_king.svg"},
          {'imgPath' : "/images/white_queen.svg"},
          {'imgPath' : "/images/white_bishop.svg"},
          {'imgPath' : "/images/white_knight.svg"},
          {'imgPath' : "/images/white_rook.svg"},
          {'imgPath' : "/images/white_pawn.svg"},

          {'imgPath' : "/images/black_king.svg"},
          {'imgPath' : "/images/black_queen.svg"},
          {'imgPath' : "/images/black_bishop.svg"},
          {'imgPath' : "/images/black_knight.svg"},
          {'imgPath' : "/images/black_rook.svg"},
          {'imgPath' : "/images/black_pawn.svg"},    ]

        Item {
            id: gameBoard2
            x: 0
            y: 0
            width : logic.boardSize * squareSize
            height: logic.boardSize * squareSize

            Image {
                source: "/images/chess_board.jpg"
                height: gameBoard2.height
                width: gameBoard2.width
            }
            Repeater {

                model: logic
                Image {
                    id: img
                    height: squareSize
                    width : squareSize

                    x: squareSize * positionX
                    y: squareSize * positionY

                    source: screen_3.images[type].imgPath

                }

            }
        }

        Button {
            id: startButton2
            anchors.left: gameBoard2.right
            anchors.right: parent.right
            anchors.leftMargin: 10
            anchors.rightMargin: 10

            text: "Start"

            onClicked: {
                logic.set_figures();
                screen_3.close()
                screen_2.show()
            }
        }
        Button{
            id: loadButton2
            y: 30
            anchors.left: gameBoard2.right
            anchors.right: parent.right
            anchors.leftMargin: 10
            anchors.rightMargin: 10

            text: "Load"

            onClicked: {

            }
        }
        Button{
            id: prevButton
            y: 60
            anchors.left: gameBoard2.right
            anchors.right: parent.right
            anchors.leftMargin: 10
            anchors.rightMargin: 150

            text: "Previous step"

            onClicked: {
                logic.prev_step();
            }
        }
        Button{
            id: nextButton
            y: 60
            anchors.left: gameBoard2.right
            anchors.right: parent.right
            anchors.leftMargin: 150
            anchors.rightMargin: 10

            text: "Next step"


            onClicked: {
                logic.next_step();

            }
        }

    }
}
