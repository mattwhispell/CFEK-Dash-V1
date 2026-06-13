#include "header.h"

//all wheel arrays are: fl, fr, bl, br;
void calculateSlippage(){
    for(int i = 0; i < 4; i++){
        if((data.motorSpeed / data.gearRatio) > ((1+SLIP_RATIO_LIMIT) * data.wheelAngularSpeed[i])
        || (data.motorSpeed / data.gearRatio) < ((1-SLIP_RATIO_LIMIT) * data.wheelAngularSpeed[i])){
            data.wheelsSlipping[i] = true;
        }
        else data.wheelsSlipping[i] = false;
    }
}