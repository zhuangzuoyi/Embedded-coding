#ifndef __PN532_H__
#define __PN532_H__

void pn532_uart_init(void);
void pn532_wake_normal(void);
int pn532_req(void);

#endif

