#include "faultHandler.h"
#include "Arduino.h"

void faultHandlerTask(){
    while(1){
        uint16_t response_fault_1 = dummySPIRead(FAULT_REG_1);
        uint16_t response_fault_2 = dummySPIRead(FAULT_REG_2);

        if (!(response_fault_1 & FAULT)) {
            // Shut down processes
            dummyShutDownProcesses();

            // Find and handle faults
            faultHandler(response_fault_1, response_fault_2);
        }

        delay(1000);
    }
}

// Brute search and handle all faults
void faultHandler(uint16_t fault1, uint16_t fault2 ){
    // Find and handle faults
    if (fault1 & VDS_OCP) { /* Handle VDS monitor overcurrent */ }
    if (fault1 & GDF) { /* Handle gate drive fault */ }
    if (fault1 & UVLO) { /* Handle undervoltage lockout */ }
    if (fault1 & OTSD) { /* Handle overtemperature shutdown */ }
    if (fault1 & VDS_HA) { /* Handle VDS overcurrent on A high-side MOSFET */ }
    if (fault1 & VDS_LA) { /* Handle VDS overcurrent on A low-side MOSFET */ }
    if (fault1 & VDS_HB) { /* Handle VDS overcurrent on B high-side MOSFET */ }
    if (fault1 & VDS_LB) { /* Handle VDS overcurrent on B low-side MOSFET */ }
    if (fault1 & VDS_HC) { /* Handle VDS overcurrent on C high-side MOSFET */ }
    if (fault1 & VDS_LC) { /* Handle VDS overcurrent on C low-side MOSFET */ }
    if (fault2 & SA_OC) { /* Handle overcurrent on phase A sense amplifier */ }
    if (fault2 & SB_OC) { /* Handle overcurrent on phase B sense amplifier */ }
    if (fault2 & SC_OC) { /* Handle overcurrent on phase C sense amplifier */ }
    if (fault2 & OTW) { /* Handle overtemperature warning */ }
    if (fault2 & GDUV) { /* Handle VCP charge pump and/or VGLS undervoltage fault */ }
    if (fault2 & VGS_HA) { /* Handle gate drive fault on the A high-side MOSFET */ }
    if (fault2 & VGS_LA) { /* Handle gate drive fault on the A low-side MOSFET */ }
    if (fault2 & VGS_HB) { /* Handle gate drive fault on the B high-side MOSFET */ }
    if (fault2 & VGS_LB) { /* Handle gate drive fault on the B low-side MOSFET */ }
    if (fault2 & VGS_HC) { /* Handle gate drive fault on the C high-side MOSFET */ }
    if (fault2 & VGS_LC) { /* Handle gate drive fault on the C low-side MOSFET */ }
}

// To be replaced appropriately.
uint16_t dummySPIRead(uint16_t addr){ return 0; }

// To be replaced appropriately.
void dummyShutDownProcesses(){ }


