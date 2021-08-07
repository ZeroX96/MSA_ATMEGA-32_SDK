/************************************************************************/
/*                                                                      */
/************************************************************************/

//#include "AppGateWay.h"
#include "KeyPad.h"
#include <util/delay.h>
KeypadT MSA;
int main(void){
    DDRD = 0xff;
    DDRC = 0x0f;
    MSA.ColsPort = BASE_C;
    MSA.ColsPosition = PIN_FOUR;
    MSA.RowsPort = BASE_C;
    MSA.RowsPosition = PIN_ZERO;
    KeypadInit(&MSA);

    while(1){
        //KeypadUpdate();
        //_delay_ms(10);
        PORTD = KeypadGetKey(&MSA);
        //_delay_ms(10);
        
    }
    return 0;
}