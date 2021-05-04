unsigned int pixel_RecoverPosition(unsigned char posX, unsigned char posY)
{	// Var. Dec. :
    posType ui16_returnPos = 0;

    // Condition about starting from 1, instead of 0.
    //if (posX == 0)  posX = 1;
    //if (posY == 0)  posY = 1;

    switch(formatDisplay.matrixView)
    {
        case 0:
        case 360:
        case (-360):
            if( !(posY % 2) )   // Even lines on the matrix physical view.
                ui16_returnPos = (MAX_COLU - posX) + (posY - 1) * MAX_COLU;
            else
                ui16_returnPos = (posX - 1) + (posY - 1) * MAX_COLU;
            break;

        case 90:
        case (-270):
            if( !(posX % 2) )   // Even columns on the matrix physical view.
                ui16_returnPos = (MAX_LEDS - posX * MAX_LINE) + posY - 1;
            else
                ui16_returnPos = MAX_LEDS - posY - (posX - 1) * MAX_LINE;
            break;

        case 180:
        case (-180):
            if((posY % 2) == 0) // Even reverted lines on the matrix physical view.
                ui16_returnPos = MAX_LEDS - posX - (posY - 1) * MAX_COLU;
            else
                ui16_returnPos = MAX_LEDS - MAX_COLU + posX - 1 - (posY - 1) * MAX_COLU;
            break;

        case 270:
        case (-90):
            if( !(posX % 2) )   // Even reverted columns on the matrix physical view.
                ui16_returnPos = (posX - 1) * MAX_LINE + posY - 1;
            else
                ui16_returnPos = (posX * MAX_LINE - 1) - posY + 1;
            break;

            /* State of every undefined case, so not supposed to come here. */
        default :   break;
    }
    return ui16_returnPos; // Return Pos.
}