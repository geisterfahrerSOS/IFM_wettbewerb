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
    boolean liveGeradeFahren();
    void radDrehung(int deltaWinkel);
    void einzelRad(int distanz, int side);
    boolean liveEinzelRad();
    void resetMotors()
private:
    int _motorLinks;
    int _motorRechts;
    int _richtungLinks;
    int _richtungRechts;
    int _geradeDistanz;
    RadSensor rad[2];
    long _setStep[2]; //bei welchem Step wird die geradeFahren Funktion aufgerufen 
    long _einzelRad[2]; //bei einem Rad der vergleichswert um die gefahrene Distanz zu überprüfen
    int _distanzEinzel[2];
};

#endif