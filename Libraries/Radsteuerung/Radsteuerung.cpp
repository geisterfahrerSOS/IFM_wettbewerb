#include "Radsteuerung.h"
#include <Arduino.h>
#include <radSensor.h>
#define SPUR 130 //wie breit ist die Achse;
#define LEFT 0   //code für linker Motor
#define RIGHT 1  //code für rechter Motor

Radsteuerung::Radsteuerung(){};

Radsteuerung::Radsteuerung(int motorLinks, int motorRechts, int richtungLinks, int richtungRechts)
{
    _motorLinks = motorLinks;
    _motorRechts = motorRechts;
    _richtungLinks = richtungLinks;
    _richtungRechts = richtungRechts;
    rad[0] = RadSensor(0, 900, 20); //Man schaut wie weit sich jedes Rad dreht jedes mal wo geradeFahren bzw. einzelRad aufgrerufen wird nimmt man
                                    //Vergleichswerte um zu schauen wie weit sich das Rad gedereht hat
    rad[1] = RadSensor(1, 900, 20); //RadSensor(PIN, THRESHOLD, Distanz proStrich)
}

void Radsteuerung::geradeFahren(int distanz) //distanz in mm
{
    einzelRad(distanz, LEFT);
    einzelRad(distanz, RIGHT);
}

void Radsteuerung::radDrehung(int deltaWinkel, int modus) //WInkel von 0 bis 360 grad; drehung um den Mittelpunkt
{
    switch (modus)
    {
    case 0:
    {
        int _drehDistanz = 0; //wie weit muss sich das entsprechende Rad drehen

        if (deltaWinkel < 180) // Winkel < 180 dreht sich der Roboter nach rechts else nach links
        {
            _drehDistanz = (deltaWinkel / 360) * PI * SPUR; //berechnung wie weit man fahren muss
            einzelRad(_drehDistanz, LEFT);
            einzelRad(-_drehDistanz, RIGHT);
        }
        else
        {
            _drehDistanz = ((360 - deltaWinkel) / 360);
            einzelRad(-_drehDistanz, LEFT);
            einzelRad(_drehDistanz, RIGHT);
        }
    }
    break;
    case 1:
    {
        int _drehDistanz = 0;
        _drehDistanz = 2 * (deltaWinkel / 360) * PI * SPUR; //berechnung wie weit man fahren muss
        if (deltaWinkel < 180)
        {
            _drehDistanz = 2 * (deltaWinkel / 360) * PI * SPUR;
            einzelRad(_drehDistanz, LEFT);
        }
        else
        {
            einzelRad(_drehDistanz, RIGHT);
        }
    }
    default:
        break;
    }
}

void Radsteuerung::einzelRad(int distanz, int side)
{
    taskManager(distanz, side, rad[side].umwandlungSteps());
}

void Radsteuerung::taskManager(int distanz, int motorSeite, int step) //verantwortlch für die task Koordinierung
{
    for (int i = 0; i < 10; i++)
    {
        if (_tasks[i][1] == 0) //wenn die Distanz gleich null, also wenn der Platz frei ist
        {
            _tasks[i][0] = motorSeite;
            _tasks[i][1] = distanz;
            _tasks[i][2] = step;
            return; //sobald ein geeigneter Ablegeplatz gefunden wurde aufhören zu suchen
        }
    }
}

void Radsteuerung::liveRad()
{
    for (int i = 0; i < 10; i++) //man geht durch alle tasks durch die man dann ausführen wird
    {
        if (_tasks[i][1] != 0) //wenn die distanz ungleich null
        {
            if((rad[_tasks[i][0]].umwandlungSteps()-_tasks[i][2]<abs(_tasks[i][1]))//solange die Distanz noch nicht erreicht wurde wird weiter gefahren
            {
                if (_tasks[i][1] > 0) //wenn die distanz größer null vorwärts fahren
                {

                    digitalWrite(motorPin(_tasks[i][0]), HIGH);
                    digitalWrite(richtungPin(_tasks[i][0]), HIGH);
                }
                else//wenn nicht rückwärts
                {
                    digitalWrite(motorPin(_tasks[i][0]), HIGH);
                    digitalWrite(richtungPin(_tasks[i][0]), LOW);
                }
                
            }else//Das Rad hat die vorgegebene Strecke erreicht, also werden die Motoren auf der seite ausgeschalten
            {
                resetMotors(_tasks[i][2]);  //sobald
                for (int j = 0; j < 3; j++) // die Werte werden resetet damit man ein neues Task hinzufügen kann
                {
                    _tasks[i][j] = 0;
                }
            }
        }
    }
}

void Radsteuerung::resetMotors(int side)
{
    if (side == 0)
    {
        digitalWrite(_motorLinks, LOW);
        digitalWrite(_motorRechts, LOW);
    }
    else
    {
        digitalWrite(_richtungLinks, LOW);
        digitalWrite(_richtungRechts, LOW);
    }
}

int Radsteuerung::motorPin(int side)
{
    if (side == 0)
    {
        return _motorLinks;
    }
    else
    {
        return _motorRechts;
    }
}
int Radsteuerung::richtungPin(int side)
{
    if (side == 0)
    {
        return _richtungLinks;
    }
    else
    {
        return _richtungRechts;
    }
}

// boolean Radsteuerung::liveRad() //live funktionen sind da damit die Loop weiter geht während der Roboter sich bewegt;
// {
//     for (int i = 0; i < 2; i++)
//     {
//         if (_distanzEinzel[i] != 0)
//         {
//             if ((rad[i].umwandlungSteps(rad[i].stepRead()) - rad[i].umwandlungSteps(_einzelRad[i])) < abs(_distanzEinzel[i])) //Solange die distanz nicht erreicht wurde fährt der Roboter vorwärts
//             {
//                 if (_distanzEinzel[i] > 0)
//                 {
//                     if (i == 0)
//                     {
//                         digitalWrite(_motorLinks, HIGH);
//                         digitalWrite(_richtungLinks, HIGH);
//                     }
//                     else
//                     {
//                         digitalWrite(_motorRechts, HIGH);
//                         digitalWrite(_richtungRechts, HIGH);
//                     }
//                 }
//                 else
//                 {
//                     if (i == 0)
//                     {
//                         digitalWrite(_motorLinks, HIGH);
//                         digitalWrite(_richtungLinks, LOW);
//                     }
//                     else
//                     {
//                         digitalWrite(_motorRechts, HIGH);
//                         digitalWrite(_richtungRechts, LOW);
//                     }
//                 }
//             }
//             else
//             {
//                 _distanzEinzel[i] = 0;
//                 resetMotors();
//             }
//         }
//     }
// }

// void Radsteuerung::geradeFahren(int distanz) //distanz in mm
// {
//     // _geradeDistanz = distanz;
//     // _setStep[0] = rad[0].umwandlungSteps();
//     // _setStep[1] = rad[1].stepRead();
//     einzelRad(distanz,_motorLinks);
//     einzelRad(distanz,_motorRechts);

// }

// boolean Radsteuerung::liveGeradeFahren() //Annahme _motor___ HIGH ist an LOW ist aus; richtung_______ HIGH ist vorwärts LOW ist rückwärts;
// {
//     if (_geradeDistanz != 0)
//     {
//         if (((rad[0].umwandlungSteps(rad[0].stepRead()) - rad[0].umwandlungSteps(_setStep[0])) + (rad[1].umwandlungSteps(rad[1].stepRead()) - rad[1].umwandlungSteps(_setStep[1]))) / 2 < abs(_geradeDistanz))
//         {
//             if (_geradeDistanz > 0)
//             {

//                 digitalWrite(_motorLinks, HIGH);
//                 digitalWrite(_motorRechts, HIGH);
//                 digitalWrite(_richtungLinks, HIGH);
//                 digitalWrite(_richtungRechts, HIGH);
//             }
//             else
//             {

//                 digitalWrite(_motorLinks, HIGH);
//                 digitalWrite(_motorRechts, HIGH);
//                 digitalWrite(_richtungLinks, LOW);
//                 digitalWrite(_richtungRechts, LOW);
//             }
//         }
//         else
//         {
//             _geradeDistanz = 0;
//             resetMotors();
//         }
//     }
//     else
//     {
//         return false;
//     }
// }