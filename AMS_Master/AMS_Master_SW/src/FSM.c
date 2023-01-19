enum BMS_Master_State {
    INITIALIZATION,
    IDLE,
    PRECHARGE,
    OPERATION,
    BALANCING,
    ERROR
};

BMS_Master_State current_state = INITIALIZATION;

void BMS_Master_FSM() {
    switch (current_state) {
        case INITIALIZATION:
            initialization();
            current_state = IDLE;
            break;
        case IDLE:
        	external_CAN();
        	internal_CAN();
            if (AIRs_Request() == 3) {
                current_state = PRECHARGE;
            } else if (error_detection()) {
                current_state = ERROR;
            }
            break;
        case PRECHARGE:
        	external_CAN();
        	internal_CAN();
            AIRs_Control(3);
            if (AIRs_Request() == 6) {
                current_state = OPERATION;
            } else if (error_detection()) {
                current_state = ERROR;
            }	
            break;
        case OPERATION:
        	external_CAN();
        	internal_CAN();
        	Fans_Control();
        	AIRs_Control(6);
            if (balancing_enable()) {
                current_state = BALANCING;
            } else if (error_detection()) {
                current_state = ERROR;
            }
            break;
        case BALANCING:
        	external_CAN();
        	internal_CAN();
        	Fans_Control();
        	balancing();
        	if (!balancing_enable()) {
                current_state = OPERATION;
            } else if (error_detection()) {
                current_state = ERROR;
            }
            break;
        case ERROR:
        	external_CAN();
        	internal_CAN();
            error_handler();
            if (error_detection()) {
                current_state = ERROR;
            } else {
                current_state = IDLE;
            }
            break;
    }
}