#ifndef Radsteuerung_h
#define Radsteuerung_h

#include <Arduino.h>
#include <radSensor.h>


class Radsteuerung
{
public:
    Radsteuerung();
    Radsteuerung(int motorLinks, int motorRechts, int richtungLinks, int richtungRechts);
    void geradeFahren(int distanz); // Distanz in mm wird einmal aufgerufen
    void einzelRad(int distanz, int side);//wird auch einmal aufgerufen
    boolean liveGeradeFahren();
    void radDrehung(int deltaWinkel);
    boolean liveEinzelRad();
    void resetMotors();
private:
    int _motorLinks;
    int _motorRechts;
    int _richtungLinks;
    int _richtungRechts;
    int _geradeDistanz;
    RadSensor rad[2];
    long _setStep[2]; //bei welchem Step die geradeFahren Funktion aufgerufen wird 
    long _einzelRad[2]; //bei einem Rad der vergleichswert um die gefahrene Distanz zu überprüfen
    int _distanzEinzel[2];
    int _drehDistanz = 0;
};

#endif