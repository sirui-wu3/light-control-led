#ifndef __ESP8266_H__
#define __ESP8266_H__

void ESP8266_Init(void);
void ESP8266_SendCmd(unsigned char *pbuf);
void ESP8266_SendData(unsigned char *pbuf);

#endif
