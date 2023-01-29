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
            if (error_detection()) {
                current_state = ERROR;
            break;
            }

            if (AIRs_Request() == 3) {
                current_state = PRECHARGE;
            break;
            }

            external_CAN();
            internal_CAN();
            break;
        case PRECHARGE:
            if (error_detection()) {
                current_state = ERROR;
                break;
            }

            if (AIRs_Request() == 6) {
                current_state = OPERATION;
                break;
            }

            external_CAN();
            internal_CAN();
            AIRs_Control(3);
            break;
        case OPERATION:
            if (error_detection()) {
                current_state = ERROR;
                break;
            }

            if (balancing_enable()) {
                current_state = BALANCING;
                break;
            }

            external_CAN();
            internal_CAN();
            AIRs_Control(6);
            Fans_Control();
            break;
        case BALANCING:
            if (error_detection()) {
                current_state = ERROR;
                break;
            }

            if (!balancing_enable()) {
                current_state = OPERATION;
                break;
            }

            external_CAN();
            internal_CAN();
            AIRs_Control(6);
            Fans_Control();
            balancing();
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