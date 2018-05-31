How to access remote serial port on Ethernet device.

1. Start UDP Listening and Client by calling function library start_lib_interface_task();
   (C# used please see ext_udp.cs function Marshal wrapper around C++ function)

2. Install Windows message handler to handle asyncronous windows messages.

3. Wait for WM_CONNECTED message which indicates device is been found on ethernet network.
   caller can call function usGetConnectedDeviceIP(deviceNum, textbuff) to readout IP Address 

   assume 5 devices exists on network then usGetConnectedDeviceIP will return 5 and 

   char ip1st[256],ip2st[256],ip3st[256],ip4st[256],ip4st[256];

   usGetConnectedDeviceIP(0,ip1st); // First IP address), function returns 5 => (5 devices)
   usGetConnectedDeviceIP(1,ip1st); // Second IP address), function returns 5 => (5 devices)
   usGetConnectedDeviceIP(2,ip1st); // Third IP address), function returns 5 => (5 devices)
   usGetConnectedDeviceIP(3,ip1st); // Forth IP address), function returns 5 => (5 devices)
   usGetConnectedDeviceIP(4,ip1st); // Fifth IP address), function returns 5 => (5 devices)


4. unsigned short uart_read(unsigned char bus_num, unsigned char *buff);
   This function reads uart data, 
   Example
   
   unsigned short len;
   unsigned char ibuff[1024];
   
   len=uart_read(2,ibuff); // => reading second port
   // uart_read(3,ibuff);=> Third uart 
   // uart_read(1,ibuff);=> RS485 port
   // len== number of read bytes
   // ibuff uart data

5. unsigned short uart_write(unsigned char bus_num, unsigned char *buff,unsigned short len);

   This function write uart data, 
   Example
   
   unsigned short len;
   unsigned char ibuff[1024];

   #define TEST_STRING "THIS IS TEST WRITE"
   strcpy(ibuff,TEST_STRING);
   
   len=uart_write(2,TEST_STRING,strlen(TEST_STRING)); // => writing second port
   // uart_write(3,TEST_STRING,strlen(TEST_STRING);=> Third uart 
   // uart_write(1,TEST_STRING,strlen(TEST_STRING);=> RS485 port
   // len== number of read bytes

5. What if multiple devices are on network how to pick particular board.

   If multiple devices are connected on network then
   => usSelectDev(0); // Selects first device
   => usSelectDev(2); // Selects second device

   => usSelectDev(4); // Selects 5th device
   
   now uart_write will write corresponding network devices.

    
6. How to modify UART baud rate

   PREFIX int fun_uart_term(unsigned short uartNum, unsigned int uartBaud, unsigned int uartStop, unsigned int uartDataBits, unsigned int parity);


   There are three set of working example for UART are provided with sample. 

   WPF/.Net and QT samples are provided, user can test loop back by connecting RX/TX or Tx+/Tx-
   and Rx+/Rx- when using RS485 device.

   When new data arrives on UART port 

        arrival of WM_CONSOLE1 windows message indicates data available on RS485 port.
        arrival of WM_CONSOLE2 windows message indicates data available on TTL UART2 port.
        arrival of WM_CONSOLE3 windows message indicates data available on TTL UART3 port.


