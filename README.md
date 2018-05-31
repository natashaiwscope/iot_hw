# emulator_v0
This hardware enables user to access TTL serial port and RS485 device via ethernet network,
its simple three step process to access, which is listed below.

Open a sample supplied QT/WPF terminal emulator
compile or use pre built executables.

Code is extreamly easy to understand, start_communication_library()
Register windows handle to receive asyncronous windows message about data arrival.
call uart_read to read incomming buffer,


similarly device is able to handle ADC/DAC/CAN/I2C and SPI from WPF/QT application
More sample code will be added as move forward.
