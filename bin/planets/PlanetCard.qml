import QtQuick 1.0

Rectangle {
    property alias title:        text1.text
    property alias house:        house.text
    property alias imageSrc:     imgPlanet.source
    property alias degreeStr:    degreeLabel.text
    property alias isRetro:      iconRetro.visible
    property int    sign: 0
    property alias ruler:         hLabel.text
    property int    dignityVal
    property int    deficientVal

    property int    assignedSign
    property real   assignedPower

    property string imageTip
    property string imageSignTip
    property string powerTip
    property string ruleTip
    property string degreeTip

    assignedSign:      setSign(sign)
    assignedPower:     setPower(dignityVal, deficientVal)

    id: card
    objectName: "card"
    width: 192; height: 260
    color: "#00000000"

    Rectangle {
       id: background
       radius: 22
       smooth: true
       gradient: Gradient {
           GradientStop {
               position: 0
               color: "#8f3313"
           }

           GradientStop {
               position: 0.500
               color: "#eb1212"
           }

           GradientStop {
               position: 1
               color: "#8f3313"
           }
       }

       z: -1
       anchors.fill: parent
       opacity: 0.540

       MouseArea {
           id: area0
           anchors.fill: parent
           hoverEnabled: true
           onEntered: highlightCard()
           onExited: resetHighlignt()
           onClicked: selectCard()
       }

    }

    Image {
       id: imgZodiac
       x: 56
       y: 70
       source: "../images/signs/leo.png"

       MouseArea {
           id: area1
           anchors.fill: parent
           hoverEnabled: true
           onEntered: {
               view.requestHelp(imageSignTip)
               highlightCard()
           }
           onExited: resetHighlignt()
           onClicked: selectCard()
       }
    }


    Image {
        id: imgPlanet
        x: 0
        y: 29
        smooth: true
        source: "../images/planets/mars.png"

        MouseArea {
            id: area2
            hoverEnabled: true
            anchors.fill: parent
            onEntered: {
                view.requestHelp(imageTip)
                highlightCard()
            }
            onExited: resetHighlignt()
            onClicked: selectCard()
        }
    }

    Rectangle {
        id: title
        x: 0
        y: 0
        width: 192
        height: 29
        color: "#000000"
        radius: 22
        smooth: true
        opacity: 1
        border.color: "#5d615a"
        z: 0

        Text {
            id: text1
            color: "#edf8c6"
            text: "PLANET"
			//smooth: true
            font.bold: true
            font.pointSize: 11
            anchors.fill: parent
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            z: 1
            font.family: "Lucida Sans Unicode"

            MouseArea {
                id: area3
                hoverEnabled: true
                anchors.fill: parent
                onEntered: {
                    view.requestHelp(text1.text)
                    highlightCard()
                }
                onExited: resetHighlignt()
                onClicked: selectCard()
            }
        }

        Image {
            id: image1
            x: 0
            y: 15
            smooth: true
            anchors.bottom: title.top
            anchors.bottomMargin: -43
            source: "corner0.png"
    }
    }

    Image {
        id: image2
        smooth: true
        anchors.right: parent.right
        anchors.rightMargin: 0
        source: "corner1.png"
        anchors.bottom: title.top
        anchors.bottomMargin: -43
    }

    Text {
        id: house
        y: 40
        width: 77
        height: 40
        color: "#edf8c6"
        text: "IV"
        anchors.left: parent.left
        anchors.leftMargin: 102
        verticalAlignment: Text.AlignTop
        anchors.right: parent.right
        anchors.rightMargin: 13
        anchors.top: parent.top
        anchors.topMargin: 40
        horizontalAlignment: Text.AlignRight
        opacity: 0.850
        font.bold: true
        style: Text.Normal
        font.pointSize: 17
        font.family: "Times New Roman"

        MouseArea {
            id: area4
            anchors.fill: parent
            hoverEnabled: true
            onEntered: {
                view.requestHelp(text1.text + "+" + house.text)
                highlightCard()
            }
            onExited: resetHighlignt()
            onClicked: selectCard()
        }
    }

    Text {
        id: dignityText
        x: 16
        y: 163
        color: "#edf8c6"
        text: "+10"
        opacity: 0.400
        font.italic: true
        font.family: "DejaVu Serif"
        font.pixelSize: 12

        MouseArea {
            id: area5
            x: -8
            y: -8
            width: 42
            height: 46
            anchors.topMargin: -8
            anchors.leftMargin: -8
            anchors.rightMargin: -8
            hoverEnabled: true
            anchors.bottomMargin: -24
            anchors.fill: parent
            onEntered: {
                view.requestHelp(powerTip)
                highlightCard()
            }
            onExited: resetHighlignt()
            onClicked: selectCard()
        }
    }

    Text {
        id: deficientText
        x: 16
        y: 182
        color: "#edf8c6"
        text: "-10"
        font.pixelSize: 12
        font.italic: true
        font.family: "DejaVu Serif"
        opacity: 0.400
    }

    Rectangle {
        id: hLine
        x: 84
        y: 120
        width: 24
        height: 192
        rotation: -90
        gradient: Gradient {
            GradientStop {
                position: 0
                color: "#14000000"
            }

            GradientStop {
                position: 0.500
                color: "#B4000000"
            }

            GradientStop {
                position: 1
                color: "#14000000"
            }
        }
    }

    Text {
        id: degreeLabel
        x: 0
        y: 204
        width: 192
        height: 24
        color: "#edf8c6"
        text: "22 3 leo"
        font.family: "Verdana"
        font.pointSize: 10
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter

        MouseArea {
            id: area6
            hoverEnabled: true
            anchors.fill: parent
            onEntered: {
                view.requestHelp(degreeTip)
                highlightCard()
            }
            onExited: resetHighlignt()
            onClicked: selectCard()
        }
    }

    Text {
        id: hLabel
        x: 44
        y: 230
        width: 98
        height: 28
        text: "22 3'' Sco"
        color: "#edf8c6"
        font.family: "DejaVu Sans Condensed"
        font.pointSize: 10
        opacity: 0
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter

        MouseArea {
            id: area7
            hoverEnabled: true
            anchors.fill: parent
            onEntered: {
                view.requestHelp(ruleTip)
                highlightCard()
            }
            onExited: resetHighlignt()
            onClicked: selectCard()
        }
    }

    Image {
        id: iconRetro
        x: 159
        y: 234
        width: 20
        height: 20
        source: "retrograde.png"

        MouseArea {
            id: area8
            hoverEnabled: true
            anchors.fill: parent
            onEntered: {
                view.requestHelp(text1.text + "+retro")
                highlightCard()
            }
            onExited: resetHighlignt()
            onClicked: selectCard()
        }
    }

    Gradient {
       id: blueGradient;

       GradientStop {
           position: 0
           color: "#13568f"
       }

       GradientStop {
           position: 0.500
           color: "#12abeb"
       }

       GradientStop {
           position: 1
           color: "#13568f"
       }
    }

    Gradient {
       id: redGradient;

       GradientStop {
           position: 0
           color: "#8f3313"
       }

       GradientStop {
           position: 0.500
           color: "#eb1212"
       }

       GradientStop {
           position: 1
           color: "#8f3313"
       }
    }

    Gradient {
       id: whiteGradient;

       GradientStop {
           position: 0
           color: "#f1fff5"
       }

       GradientStop {
           position: 0.500
           color: "#caf0e8"
       }

       GradientStop {
           position: 1
           color: "#f1fff5"
       }
    }

    Gradient {
       id: brownGradient;

       GradientStop {
           position: 0
           color: "#7c7a6a"
       }

       GradientStop {
           position: 0.500
           color: "#c7ccb6"
       }

       GradientStop {
           position: 1
           color: "#7c7a6a"
       }
    }

    Gradient {
        id: hGradient
        GradientStop {
            position: 0
            color: "#14000000"
        }

        GradientStop {
            position: 0.500
            color: "#90000000"
        }

        GradientStop {
            position: 1
            color: "#14000000"
        }
    }

    Gradient {
        id: nullGradient
        GradientStop {
            position: 0
            color: "#00000000"
        }
    }




    function setSign (signId) {

        switch (signId % 4) {
            case 0: background.gradient = redGradient;   break;
            case 1: background.gradient = brownGradient; break;
            case 2: background.gradient = whiteGradient; break;
            case 3: background.gradient = blueGradient;  break;
        }

        switch (signId) {
            case 0:  imgZodiac.source = "../images/signs/aries.png";       break;
            case 1:  imgZodiac.source = "../images/signs/taurus.png";      break;
            case 2:  imgZodiac.source = "../images/signs/gemini.png";      break;
            case 3:  imgZodiac.source = "../images/signs/cancer.png";      break;
            case 4:  imgZodiac.source = "../images/signs/leo.png";         break;
            case 5:  imgZodiac.source = "../images/signs/virgo.png";       break;
            case 6:  imgZodiac.source = "../images/signs/libra.png";       break;
            case 7:  imgZodiac.source = "../images/signs/scorpio.png";     break;
            case 8:  imgZodiac.source = "../images/signs/saggitarius.png"; break;
            case 9:  imgZodiac.source = "../images/signs/capricorn.png";   break;
            case 10: imgZodiac.source = "../images/signs/aquarius.png";    break;
            case 11: imgZodiac.source = "../images/signs/pisces.png";      break;
        }

        return signId;
    }

    function setPower (dignityValue, deficientValue) {
        if (dignityValue > 0)
            dignityText.text = "+" + dignityValue;
        else
            dignityText.text = "";

        if (deficientValue < 0)
            deficientText.text = "—" + (-deficientValue);
        else
            deficientText.text = "";

        var opacity = 1;

        if (dignityValue > 0 && deficientValue < 0)          // calculate custom opacity
            opacity = (dignityValue + 4 + deficientValue) / 17.0;
		if (objectName == "card_0") opacity += 0.3;		 	 // sun is always brighter
        if (opacity < 0.2) opacity = 0.2;

        imgPlanet.opacity = opacity;
        return opacity;
    }

    function highlightCard() {
        background.opacity = 0.7;
        hLabel.opacity = 0.75;
    }

    function resetHighlignt() {
        background.opacity = 0.54;
        hLabel.opacity = 0;
    }

    function selectCard() {
        view.objectClicked(card.objectName)
    }
}
