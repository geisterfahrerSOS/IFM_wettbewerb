#include "Radsteuerung.h"
#include <Arduino.h>
#include <radSensor.h>
#define SPUR 130; //wie breit ist die Achse;

Radsteuerung::Radsteuerung(){};

Radsteuerung::Radsteuerung(int motorLinks, int motorRechts, int richtungLinks, int richtungRechts)
{
    _motorLinks = motorLinks;
    _motorRechts = motorRechts;
    _richtungLinks = richtungLinks;
    _richtungRechts = richtungRechts;
    rad[0] = RadSensor(0, 900, 20); //Man schaut wie weit sich jedes Rad dreht jedes mal wo geradeFahren bzw. einzelRad aufgrerufen wird nimmt man
                                    //Vergleichswerte um zu schauen wie weit sich das Rad gedereht hat
    rad[1] = RadSensor(1, 900, 20);
    _distanzEinzel[0] = 0;
    _distanzEinzel[1] = 0;
}

void Radsteuerung::radDrehung(int deltaWinkel) //WInkel von 0 bis 360 grad
{
    _drehDistanz = 0;

    if (deltaWinkel < 180)
    {
        _drehDistanz = (deltaWinkel / 360) * PI * SPUR;
        einzelRad(_drehDistanz, 0);
        einzelRad(-_drehDistanz, 1);
    }
    else
    {
        _drehDistanz = ((360 - deltaWinkel) / 360);
        einzelRad(-_drehDistanz, 0);
        einzelRad(_drehDistanz, 1);
    }
}

void Radsteuerung::geradeFahren(int distanz) //distanz in mm
{
    _geradeDistanz = distanz;
    _setStep[0] = rad[0].stepRead();
    _setStep[1] = rad[1].stepRead();
}

boolean Radsteuerung::liveGeradeFahren() //Annahme _motor___ HIGH ist an LOW ist aus; richtung_______ HIGH ist vorwärts LOW ist rückwärts;
{
    if (_geradeDistanz != 0)
    {
        if (((rad[0].umwandlungSteps(rad[0].stepRead()) - rad[0].umwandlungSteps(_setStep[0])) + (rad[1].umwandlungSteps(rad[1].stepRead()) - rad[1].umwandlungSteps(_setStep[1]))) / 2 < abs(_geradeDistanz))
        {
            if (_geradeDistanz > 0)
            {

                digitalWrite(_motorLinks, HIGH);
                digitalWrite(_motorRechts, HIGH);
                digitalWrite(_richtungLinks, HIGH);
                digitalWrite(_richtungRechts, HIGH);
            }
            else
            {

                digitalWrite(_motorLinks, HIGH);
                digitalWrite(_motorRechts, HIGH);
                digitalWrite(_richtungLinks, LOW);
                digitalWrite(_richtungRechts, LOW);
            }
        }
        else
        {
            _geradeDistanz = 0;
            resetMotors();
        }
    }
    else
    {
        return false;
    }
}

void Radsteuerung::einzelRad(int distanz, int side) //man setzt den anfangswert von _einzelRad[side] gleich der bereits gefahrenen Strecke, also als Vergleichswert;
{
    _distanzEinzel[side] = distanz;
    _einzelRad[side] = rad[side].stepRead();
}

boolean Radsteuerung::liveEinzelRad()//live funktionen sind da damit die Loop weiter geht während der Roboter sich bewegt;
{
    for (int i = 0; i < 2; i++)
    {
        if (_distanzEinzel[i] != 0)
        {
            if ((rad[i].umwandlungSteps(rad[i].stepRead()) - rad[i].umwandlungSteps(_einzelRad[i])) < abs(_distanzEinzel[i]))//Solange die distanz nicht erreicht wurde fährt der Roboter vorwärts
            {
                if (_distanzEinzel[i] > 0)
                {
                    if (i == 0)
                    {
                        digitalWrite(_motorLinks, HIGH);
                        digitalWrite(_richtungLinks, HIGH);
                    }
                    else
                    {
                        digitalWrite(_motorRechts, HIGH);
                        digitalWrite(_richtungRechts, HIGH);
                    }
                }
                else
                {
                    if (i == 0)
                    {
                        digitalWrite(_motorLinks, HIGH);
                        digitalWrite(_richtungLinks, LOW);
                    }
                    else
                    {
                        digitalWrite(_motorRechts, HIGH);
                        digitalWrite(_richtungRechts, LOW);
                    }
                }
                
            }else
            {
                _distanzEinzel[i] = 0;
                resetMotors();
            }
        }
    }
}

void Radsteuerung::resetMotors()
{
    digitalWrite(_motorLinks, LOW);
    digitalWrite(_motorRechts, LOW);
    digitalWrite(_richtungLinks, LOW);
    digitalWrite(_richtungRechts, LOW);
}
