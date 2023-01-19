#ifndef BMS_MASTER_FSM_H
#define BMS_MASTER_FSM_H

enum BMS_Master_State {
    INITIALIZATION,
    IDLE,
    PRECHARGE,
    OPERATION,
    BALANCING,
    ERROR
};

extern BMS_Master_State current_state;

void BMS_Master_FSM();
void initialization();
void external_CAN();
void internal_CAN();
void Fans_Control();
void AIRs_Control(int);
bool error_detection();
void error_handler();
bool AIRs_Request();
bool Fans_Request();
bool synchronism();
bool balancing_enable();
void balancing();
bool balancing_done();

#endif