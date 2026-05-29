#include "header.h"

//all wheel arrays are: fl, fr, bl, br;
void calculateSlippage(){
    for(int i = 0; i < 4; i++){
        if((data.motorSpeed / GEAR_RATIO) > ((1+SLIP_RATIO_LIMIT) * data.wheelAngularSpeed[i])
        || (data.motorSpeed / GEAR_RATIO) < ((1-SLIP_RATIO_LIMIT) * data.wheelAngularSpeed[i])){
            data.wheelsSlipping[i] = true;
        }
        else data.wheelsSlipping[i] = false;
    }
}