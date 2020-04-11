actionArray[drehen] = 1;
drehen = 180;




//bestimmt den Winkel, in den der Roboter fahren soll // -X -> links / +X -> rechts
int drehen = 0, int overflow = 0;
bool neuGestartet = true, fertig = false;

if(actionArray[drehen] == 1)
{
    //sollte die Funtion neu aufgerufen werden, so wird der zielwinkel gesetzt
    if(neuGestartet)
    {
        richtung = winkelkompass11 + drehen; //Ziel des Drehens
        if(richtung < 0)
        {
            richtung += 360;
            overflow = 1;
        }
        else if(richtung > 360)
        {
            overflow = -1;
            richtung -= 360;
        }
        neuGestartet = false;
        fertig = false;
    }
    if(!fertig)
        switch (overflow)
        {
            case -1:
                if((winkelkompass11 >= 0) && (winkelkompass11 < richtung) || winkelkompass11 > ziel + wiggle)
                {
                    links();
                }
                else
                {
                    fertig = true;
                }
                break;
            case 0:
                if (drehen > 0 && winkelkompass11 < richtung - wiggle)
                {
                    rechts();
                }
                else
                {
                    fertig = true;
                }
                else
                {
                    if(winkelkompass11 > richtung + wiggle)
                    {
                        links();
                    }
                    else
                    {
                        fertig = true;
                    }
                }
                break;
            case 1:
                if((winkelkompass11 <= 360) && (winkelkompass11 > richtung) || winkelkompass11 < richtung - wiggle)
                {
                    rechts();
                }
                else
                {
                    fertig = true;
                }
                break;
        default:
            if (drehen > 0 && winkelkompass11 < richtung - wiggle)
            {
                rechts();
            }
            else
            {
                fertig = true;
            }
            else
            {
                if(winkelkompass11 > richtung + wiggle)
                {
                    links();
                }
                else
                {
                    fertig = true;
                }
            }
        }

    }
    else
    //Ziel ereicht
    {
        fertig = false;
        neuGestartet = true;
        richtung = 0;
        actionArray[drehen] = 0;
        stop();
        neueAktion();
    }
}

void stop()
{
    radAusgabeBool(0, 0);
}



    if (richtung + winkel < 0) // angenommen winkel = -40 richtung = 20 dann Ziel = 340
    {
        ziel = 360 + richtung + winkel; //340
        while ((kompass1.winkel() > 0) && (kompass1.winkel() < richtung))
        {
            links();
        }
        while (kompass1.winkel() > ziel)
        {
            links();
        }
    }
    else if (richtung + winkel > 360)
    {
        ziel = richtung + winkel - 360;
        while ((kompass1.winkel() < 360) && (kompass1.winkel() > richtung))
        {
            rechts();
        }
        while (kompass1.winkel() < ziel)
        {
            rechts();
        }
    }
    else
    {
        ziel = richtung + winkel;
        if (winkel > 0)
        {
            while (kompass1.winkel() < ziel)
            {
                rechts();
            }
        }
        else
        {
            while (kompass1.winkel() > ziel)
            {
                rechts();
            }
        }
    }