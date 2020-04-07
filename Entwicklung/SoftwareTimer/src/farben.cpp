actionArray[FARBEERKENNEN] == 1
farbe = RED;
richtung = 1; //1 == Vorwärts / 2 == rückwärts


if(actionArray[FARBEERKENNEN == 1])
{
    if(farbe == farbeFarbsensor1)
    //Farbe stimmt überein
    {
        actionArray[FARBEERKENNEN] = 0;
        switch (richtung)
        {
        case 1:
            actionArray[VORWAERTS] = 0;
            break;
        
        case 2:
            actionArray[RUECKWARTS] = 0;
            break;
        default:
            actionArray[VORWAERTS] = 0;
            actionArray[RUECKWARTS] = 0;
        }
        stop();
    }
    else
    {
        switch (richtung)
        {
        case 1:
            actionArray[VORWAERTS] = 1;
            break;
        case 2:
            actionArray[RUECKWARTS] = 0;
        default:
        //richtung ist falsch
            actionArray[FARBEERKENNEN] = 0;
            actionArray[VORWAERTS] = 0;
            actionArray[RUECKWARTS] = 0;
            stop();
            break;
        }
    }
}