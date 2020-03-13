#ifndef Radsteuerung_h
#define Radsteuerung_h

#include <Arduino.h>
#include <radSensor.h> //Für die radSensor Daten bzw. schon gefajrene Distanz

class Radsteuerung
{
public:
    Radsteuerung();
    Radsteuerung(int motorLinks, int motorRechts, int richtungLinks, int richtungRechts);
    void geradeFahren(int distanz);        // Distanz in mm wird einmal aufgerufen
    void einzelRad(int distanz, int side); //zufahrende Distanz, motor: 0: Links, 1: Rechts
    void radDrehung(int deltaWinkel, int modus);//(Winkel 0-360 grad, modus: 0: drehung um den Mittelpunkt; 1: drehung um das jeweilge Rad; 2: drehung um einen spezifischen punkt
    void liveRad();//sollte im Loop() stehen und führt alle commands aus die über einzelRad bzw. taskManager ihm gegeben werden
    void resetMotors(int side);
    void taskManager(int distanz, int motorSeite, int step);
    int motorPin(int side);    //gibt den echten Motor Pin zurück
    int richtungPin(int side); //gibt den richtungs Pin zurück
    int _motorLinks;
    int _motorRechts;
    int _richtungLinks;
    int _richtungRechts;
private:
    RadSensor rad[2];
    int _tasks[10][3]; //ein Array zum koordinieren wie die Daten an die liveRad Funktion weitergegeben werden
    //_tasks[anzahl an simultanen Tasks][Motorseite(0==Links||1==Rechts), vorgegebene Distanz, gemessene vergleichsDistanz];
    //_tasks[anzahl][0: Motor, 1: Distanz, 2: Step]
};
#endif