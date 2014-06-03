#include "stm32f0xx.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_rcc.h"
#include "enc28j60.h"
#include "stm32f0xx_exti.h"
#include "usart.h"
#include "ipstack.h"
#include "websocket.h"
#include "gpio.h"

int main(void) {
	usartInit();
	IPstackInit();
	ledInit();
	initPulsioximeter();

	uint8_t i;
	uint8_t packet[MAXPACKETLEN];
	uint8_t wsMsgLen = 0;
	char *wsMsg;
	char *bpmString = "TEST";

	while (1) {

		if (tcpState == tcpCLOSED) {
			wsHandshake(packet, 50000);
		} else if (wsState == wsOPEN) {

			//Receive websocket message
			wsMsg = receiveWSmsg(packet, 50000, &wsMsgLen);
			if(memcmp ( wsMsg, "getBPM", 6) == 0){


				uint16_t BMPint = getBPM();

				char BMPs[10] = "          ";
				sprintf(BMPs, "%d", BMPint);

				sendWSmsg(packet, 50000, BMPs, wsMsgLen);

			}
			if(memcmp ( wsMsg, "getCO2", 6) == 0){

				uint16_t CO2int = getOxygenSaturation();

				char CO2s[10] = "          ";
				sprintf(CO2s, "%d", CO2int);

				sendWSmsg(packet, 50000, CO2s, wsMsgLen);

			}
			if(memcmp ( wsMsg, "discovery", 9) == 0){

				char all[] = "getBPM, getC02";
				sendWSmsg(packet, 50000, all, 14);
			}


		} else {
			IPstackIdle();
		}


	}

}



void EXTI2_3_IRQHandler(void) {
	if (EXTI_GetITStatus(EXTI_Line2) != RESET) {
		readPulsioximeter();
		EXTI_ClearITPendingBit(EXTI_Line2);
	}
}



