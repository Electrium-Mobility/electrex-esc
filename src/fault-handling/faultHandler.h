
#ifndef FAULTHANDLER
#define FAULTHANDLER
#include "Arduino.h"

// Fault
#define FAULT_REG_1 0x00
#define FAULT_REG_2 0x01

#define FAULT 0xA   // Same as reading from nFAULT pin ; i chose spi cause idrk where that pin is going or how to read from it
#define VDS_OCP 0x9 // VDS monitor overcurrent fault condition
#define GDF 0x8     // Gate drive fault condition
#define UVLO 0x7    // Undervoltage lockout fault condition
#define OTSD 0x6    // Overtemperature shutdown
#define VDS_HA 0x5  // VDS overcurrent fault on the A high-side MOSFET
#define VDS_LA 0x4  // VDS overcurrent fault on the A low-side MOSFET
#define VDS_HB 0x3  // VDS overcurrent fault on the B high-side MOSFET
#define VDS_LB 0x2  // VDS overcurrent fault on the B low-side MOSFET
#define VDS_HC 0x1  // VDS overcurrent fault on the C high-side MOSFET
#define VDS_LC 0x0  // VDS overcurrent fault on the C low-side MOSFET

#define SA_OC 0xA   // Overcurrent on phase A sense amplifier 
#define SB_OC 0x9   // Overcurrent on phase B sense amplifier 
#define SC_OC 0x8   // Overcurrent on phase C sense amplifier 
#define OTW 0x7     // Overtemperature warning
#define GDUV 0x6    // VCP charge pump and/or VGLS undervoltage fault
#define VGS_HA 0x5  // Gate drive fault on the A high-side MOSFET
#define VGS_LA 0x4  // Gate drive fault on the A low-side MOSFET
#define VGS_HB 0x3  // Gate drive fault on the B high-side MOSFET
#define VGS_LB 0x2  // Gate drive fault on the B low-side MOSFET
#define VGS_HC 0x1  // Gate drive fault on the C high-side MOSFET
#define VGS_LC 0x0  // Gate drive fault on the C low-side MOSFET


void faultHandlerTask();
void faultHandler(uint16_t response_fault_1, uint16_t response_fault_2);

#endif