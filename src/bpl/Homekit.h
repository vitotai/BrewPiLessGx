#ifndef HOMEKIT_H
#define HOMEKIT_H


#define HomekitStatus_Inactive 0
#define HomekitStatus_Pairing 1
#define HomekitStatus_Paired 2
#define HomekitStatus_Connected 3

extern void homekit_setup(void);
extern void homekit_reboot(void);
extern bool homekit_reset_pairing(void);
extern void homekit_restart_pairing(void);
extern uint8_t homekit_status(void);
#endif