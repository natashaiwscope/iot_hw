
1. System turns on and discovers its status on network
2. Waits for user to connect device.
3. User Reads "C" script, should single "C" script be used across
   micro and PC Application side


   List of functions.

/*********************************
Max Baudrate  RS485    11.25 MBPS
              USART2    5.62 MBPS
              USART3    5.62 MBPS
**********************************/

#include <stdio.h>

int main(void)
{
   /* Open UART2,UART3 */
   RS485_setDMASize(dmaSize);
   RS485_open(baudRate,stopBit);

   Link_Window1(RS485_Rx);
   Link_Window2(RS485_Tx);

   Link_File(RS485_Rx,"rs485_rx.txt");
   Link_File(RS485_Tx,"rs485_tx.txt");

   /* setDMA size, higher is always better,
   remember that Open UART2,UART3 */

   TTL_UART2_open(baudRate,stopBit);
   TTL_UART2_setDMASize(dmaSize);

   TTL_UART3_open(baudRate,stopBit);
   TTL_UART3_setDMASize(dmaSize);

   Link_Window1(TTL_UART2_RX)
   Link_Window2(TTL_UART3_RX)


   return 0;
}


