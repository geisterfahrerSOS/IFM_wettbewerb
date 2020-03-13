#include <Arduino.h>
#include <Farbe.h>
//Konstuktor
Farbe::Farbe()
{
    color = -1;
};
Farbe::Farbe(byte red, byte green, byte blue, byte clear)
{
    //setzt die Farben mit CLEAR
    red = red;
    green = green;
    blue = blue;
    clear = clear;
    color = -1;
};
Farbe::Farbe(byte red, byte green, byte blue)
{
    //setzt die Farben ohne CLEAR
    red = red;
    green = green;
    blue = blue;
    color = -1;
};
//setzt die Farbe ohne CLEAR
void Farbe::setColor(byte red, byte green, byte blue)
{
    red = red;
    green = green;
    blue = blue;
    // Serial.print("RED: "); Serial.println(red);
    // Serial.print("GREEN: "); Serial.println(green);
    // Serial.print("BLUE: "); Serial.println(blue);
    // Serial.println();
};
//setzt die Farbe mit CLEAR
void Farbe::setColor(byte red, byte green, byte blue, byte clear)
{
    red = red;
    green = green;
    blue = blue;
    clear = clear;
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
/*ÜBERARBEITEN*/
void Farbe::checkColor()
{
    // Serial.print("RED: "); Serial.println(red);
    // Serial.print("GREEN: "); Serial.println(green);
    // Serial.print("BLUE: "); Serial.println(blue);
    // Serial.print("CLEAR: "); Serial.println(clear);
    // Serial.println();
    //Berechnet den Unterschied
    float differenceRG, differenceRB, differenceGB;
    differenceRG = float(red) / float(green);
    differenceRB = float(red) / float(blue);
    differenceGB = float(green) / float(blue);
    // Serial.print("DIFFERENCERG: "); Serial.println(differenceRG);
    // Serial.print("DIFFERENCERB: "); Serial.println(differenceRB);
    // Serial.print("DIFFERENCEGB: ");Serial.println(differenceGB);
    
    //Weiß
    if(differenceRG > 0.8 && differenceRG < 1.2
        && differenceRB > 0.8 && differenceRB < 1.2
        && differenceGB > 0.8 && differenceGB < 1.2
        && red > 127//&& red < 
        && green > 127//&& green < 
        && blue > 127//&& blue <
    )
    {
        color = 7;
    }
    //schwarz
    else if(differenceRG > 0.8 && differenceRG < 1.2
        && differenceRB > 0.8 && differenceRB < 1.2
        && differenceGB > 0.8 && differenceGB < 1.2 
        /*&& red > */&& red < 127
        /*&& green > */&& green < 127
        /*&& blue > */&& blue < 127
    )
    {
        color = 6;
    }
    //Gelb
    else if(differenceRG > 0.95 && differenceRG < 1.1
        && differenceRB > 1.35 && differenceRB < 1.5
        && differenceGB > 1.35 && differenceGB < 1.55 
        && red > 90//&& red < 
        && green > 90//&& green < 
        && blue > 90//&& blue < 
    )
    {
        color = 2;
    }
    //Orange
    else if(differenceRG > 1.2 && differenceRG < 1.35
        && differenceRB > 1.5 && differenceRB < 1.7
        && differenceGB > 1.1 && differenceGB < 1.35 
        && red > 60//&& red < 
        && green > 60//&& green < 
        && blue > 60//&& blue < 
    )
    {
        color = 1;
    }
    //Rot
    else if(red > green && red > blue
        /*&& red > *///&& red < 100
        /*&& green > *///&& green < 100
        /*&& blue > *///&& blue < 100
    )
    {
        color = 5;
    }
    //Grün
    else if(green > red && green > blue
        /*&& red > *///&& red < 100
        /*&& green > *///&& green < 100
        /*&& blue > *///&& blue < 100
    )
    {
        color = 3;
    }
    //BLAU
    else if(blue > green && blue > red
        //&& red > 50//&& red < 100
        //&& green > 50//&& green < 100
        //&& blue > 50//&& blue < 100
    )
    {
        color = 4;
    }  
    //keine der obigen Farben
    else
    {
        color = -1;
    } 
    // Serial.print("COLOR: "); Serial.println(color);
    // Serial.println();
};
//gibt die Farbe aus(Serial)
void Farbe::printColor()
{
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
