// ****************************************************************************
//      - - - /   x_chip_uid.ino
//    -( W-W )        www.wood-walker.org - Rainer Radow
//      - - - \          Jan 22, 2017, version 1.v0
// ****************************************************************************
String ReadUniqueID()
{
    unsigned int status ;
    char buf[9];
    String uid;

    WDT_Disable(WDT);

    // Send the Start Read unique Identifier command (STUI)
    // by writing the Flash Command Register with the STUI command.
    EFC1->EEFC_FCR = (0x5A << 24) | EFC_FCMD_STUI;
    do  {
        status = EFC1->EEFC_FSR ;
    } while ((status & EEFC_FSR_FRDY) == EEFC_FSR_FRDY);

    // The Unique Identifier is located in the first 128 bits of the Flash memory mapping.
    // So, at the address 0x400000-0x400003. */
    for (byte b = 0 ; b < 4 ; b++) {
        snprintf(buf, sizeof(buf), "%08x", *(uint32_t *)(IFLASH1_ADDR + b * 4));
        uid += buf;
    }
    // To stop the Unique Identifier mode, the user needs to send the Stop Read unique Identifier
    // command (SPUI) by writing the Flash Command Register with the SPUI command.
    EFC1->EEFC_FCR = (0x5A << 24) | EFC_FCMD_SPUI;

    // When the Stop read Unique Unique Identifier command (SPUI) has been performed, the
    // FRDY bit in the Flash Programming Status Register (EEFC_FSR) rises.
    do  {
        status = EFC1->EEFC_FSR ;
    } while ((status & EEFC_FSR_FRDY) != EEFC_FSR_FRDY);
    return(uid);
}
//=================================================================================
//=================================================================================