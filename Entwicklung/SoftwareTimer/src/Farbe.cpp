#include <Arduino.h>
#include <Farbe.h>
//Konstuktor
Farbe::Farbe()
{
    color = -1;
};
Farbe::Farbe(byte aRed, byte aGreen, byte aBlue, byte aClear)
{
    //setzt die Farben mit CLEAR
    red = aRed;
    green = aGreen;
    blue = aBlue;
    clear = aClear;
    color = -1;
};
Farbe::Farbe(byte aRed, byte aGreen, byte aBlue)
{
    //setzt die Farben ohne CLEAR
    red = aRed;
    green = aGreen;
    blue = aBlue;
    color = -1;
};
//setzt die Farbe ohne CLEAR
void Farbe::setColor(byte aRed, byte aGreen, byte aBlue)
{
    red = aRed;
    green = aGreen;
    blue = aBlue;
    // Serial.print("RED: "); Serial.println(red);
    // Serial.print("GREEN: "); Serial.println(green);
    // Serial.print("BLUE: "); Serial.println(blue);
    // Serial.println();
};
//setzt die Farbe mit CLEAR
void Farbe::setColor(byte aRed, byte aGreen, byte aBlue, byte aClear)
{
    red = aRed;
    green = aGreen;
    blue = aBlue;
    clear = aClear;
    // Serial.print("RED: "); Serial.println(red);
    // Serial.print("GREEN: "); Serial.println(green);
    // Serial.print("BLUE: "); Serial.println(blue);
    // Serial.print("CLEAR: "); Serial.println(clear);
    // Serial.println();
};
//gibt die Farbe zurück
int Farbe::getColor()
{
    // Serial.print("COLOR: "); Serial.println(color);
    // Serial.println();
    return color;
};
//Updatet die Farbe
void Farbe::checkColor()
{
    // Serial.print("RED: "); Serial.println(red);
    // Serial.print("GREEN: "); Serial.println(green);
    // Serial.print("BLUE: "); Serial.println(blue);
    // Serial.print("CLEAR: "); Serial.println(clear);
    // Serial.println();
    //Berechnet den Unterschied
    float avRed = red, avGreen = green, avBlue = blue, average = (red + green + blue) / 3;
    avRed /= average;
    avGreen /= average;
    avBlue /= average;
    // Serial.print("avRed: "); Serial.println(avRed);
    // Serial.print("avGreen: "); Serial.println(avGreen);
    // Serial.print("avBlue: ");Serial.println(avBlue); 
    

    //orange
    if(red > 60 && green > 10 && blue > 10 && clear > 115
        && avRed < 2 && avGreen < 0.80 && avBlue < 0.65
        && avRed > 1.60 && avGreen > 0.40 && avBlue > 0.35        
    )
    {
        color = 1;
    }
    //gelb
    else if(red > 75 && green > 70 && blue > 30 && clear > 150
        && avRed < 1.45 && avGreen < 1.25 && avBlue < 0.75
        && avRed > 1.25 && avGreen > 1.05 && avBlue > 0.45
    )
    {
        color = 2;
    }
    //schwarz
    else if(red < 100 && blue < 100 && green < 100
            && avBlue > 0.70 && avGreen > 0.70 && avGreen > 0.70
            && avBlue < 1.30 && avGreen < 1.30 && avGreen < 1.30
    )
    {
        color = 6;
    }
    //grau
    else if(red < 160 && blue < 160 && green < 160
            && avBlue > 0.70 && avGreen > 0.70 && avGreen > 0.70
            && avBlue < 1.30 && avGreen < 1.30 && avGreen < 1.30
    )
    {
        color = 7;
    }
    //weiß
    else if(red < 256 && blue < 256 && green < 256
            && avBlue > 0.70 && avGreen > 0.70 && avGreen > 0.70
            && avBlue < 1.30 && avGreen < 1.30 && avGreen < 1.30
    )
    {
        color = 8;
    }
    //grün
    else if(green > blue && green > red)
    {
        color = 3;
    }
    //blau
    else if(blue > green && blue > red)
    {
        color = 4;
    }
    //rot
    else if(red > blue && red > green)
    {
        color = 5;
    }
    //Wird nicht eintreten
    else
    {
        color = -1;
    }
};
//gibt die Farbe aus(Serial)
void Farbe::printColor()
{
    Serial.print("Farbe: ");
    switch(color)
    {
        case 1:
        Serial.println("Orange");
        break;
        case 2:
        Serial.println("Gelb");
        break;
        case 3:
        Serial.println("Grün");
        break;
        case 4:
        Serial.println("Blau");
        break;
        case 5:
        Serial.println("Rot");
        break;
        case 6:
        Serial.println("Schwarz");
        break;
        case 7:
        Serial.println("Grau");
        break;
        case 8:
        Serial.println("Weiß");
        break;
        default:
        Serial.println("Keine Farbe");
    }
    Serial.println();
};


/*
 * Schwarz
82    0,96 1 x
85    0,96 x 1,03
82    x 1 1,03
139

82    0,95 0,98 x
86    0,95 x 1,03
83    x 0,98 1,03
141

Weiß     RG RB GB

192      0,96 1,05 X
198      0,96 X 1,03
191      X 1,05 1,03
254

186      0,97 1,02 X
191      0,97 X 1,04
182      X 1,02 1,04
254

Rot
127  1,24 1,36 x
102  1,24 x 1,10
92   x 1,36 1,10
173

115  1,16 1,25 x
99   1,16 x 1,07
92   x 1,25 1,07
166

Grün
91   0,89 0,95 x
102  0,89 x 1,07
95   x 0,95 1,07
156

93   0,87 0,95 x
106  0,87 x 1,09
97   x 0,95 1,09
161

BLAU

92    0,87 0,74 x
105   0,87 x 0,84
124   x 0,74 0,84
168

91    0,88 0,75 x
103   0,88 x 0,85
121   x 0,75 0,85
166

Gelb

160   1,02 1,42 x
156   1,02 x   1,39
112   x 1,42 1,39
229

163   1,03 1,42 x
157   1,03 x 1,37
114   x 1,42 1,37
231

Orange

164 1,27 1,59 x
129 1,27 x 1,25
103 x 1,59 1,25
213

126 1,17 1,36 x
107 1,17 x 1,16
92  x 1,36 1,16
175

*/
