#include "fsm.h"


void fsm_init(elevator* el){
    //Sørger for forbindelse til heisserver
    elevio_init();

    //Koden under skal kjøres hvis heisen ikke er i en etasje
    while(elevio_floorSensor() == -1){
        elevio_motorDirection(DIRN_DOWN);
    }
    if(elevio_floorSensor() != -1){
            //Elevator is in a defined floor
            elevio_motorDirection(DIRN_STOP);
            update_current_floor(el);
            el->state = IDLE;
    }
   /*  printf("Current floor: \n");
    printf("%d",el->currentFloor); */
}


void fsm_run(elevator* el){
    while (1){
        if(elevio_stopButton()){
            el->state = EMERGENCY_STOP;
        }
        switch (el->state)
        {
        case IDLE:
        fsm_idle(el);
            break;
        case EMERGENCY_STOP:
            fsm_emergency_stop(el);
            break;
        case MOVING:
            fsm_moving(el);
        default:
            break;
        }
    }
}


void fsm_emergency_stop(elevator* el) {
    printf("EMERGENCY STOP\n");

    elevio_motorDirection(DIRN_STOP);
    

    if(elevio_floorSensor() != -1){
        printf("OPENING DOOR");
        timer_start(el); 
        while(!times_up(el)){
            printf("WAITING");
        }
    }

    //Mulighet for å komme til IDLE
    if(!elevio_stopButton()){
        el->state = IDLE;
    }
}


void fsm_moving(elevator* el){
    update_queue(el);
    printf("MOVIING \n");

}

void fsm_idle(elevator* el){
    update_queue(el);
    if(elevator_has_order(el)){
        el->state = MOVING;
    }
}



