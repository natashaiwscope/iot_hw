IOT developers/Embedded Engineer felt need of wireless device
which let them easily access remote gpio input/output/analog input/I2C/or 
TTL UART interface.
Today introducing a simple to use device.

We had developed physical ethernet version of this device, Although Ethernet
was lightning fast comparison to ESP8266 wireless but adding RJ45 cable adds
extra complexity.

This Wirelss device comes in two flevours regular (fits raspberry PI enclosure)
and miniature (which fits in raspberry PI zero enclosure).

WiFi Device has capabilility capability to talk to variety of sensors with plenty
of C++ console mode examples along with Linux(QT and C++ Console)/Windows(QT WPF 
and C++ Console) sources, all examples are completly open except underlaying 
commnication library which we provide for Linux/Windows/Adroid and Raspberry PI.


  To join WiFi network you need WiFi login and password,connect device
  via Micro USB cable to Windows/Linux PC, you will see new COM port 
  popping up, open this port using hyper terminal or teraterm or any other
  text based console.

  type help command and you will see all ESP8266 commands list. Using example 
  you can join WiFi network, once you join device saves login and password for future

  Issue a reset to device and device is ready to use..

  Now start open source QT application it will locate device on same WiFi local network.

  You can click and individual LED buttons and see on board LED will turn ON
  and OFF, it indicates that device is communicating via wireless.


  There are 4 general purpose PINS which can be used Digital Input(Default) or Analog or Digital Output.

  Also there is I2C connector which lets user to scan I2C decvice or I2C Read/Write.
 

  You can see we trying to toggle GPIO and its showing on board.

  Very first sensor is soil moisture sensor where we will read directly water level
  in soil,  higher the number is more the humidity and lower number means lower humidity.


  Second sensor is APD 9260 I2C RGB light sensor, First we scan and here you can see.
  Now we initialize and reading RGB light value.


  Third example is where we reading temperature using I2C infrared sensor,
  here we trying to read on cold water bottle or human finger.


  We are adding more sensor to it, like finger print(UART)/PM2.5(UART)/
  humidity(I2C)/GAS (Analog) or Motion sensor(which digital).

  if you want us to add and include new type of sensor please let us know
  we will include examples in demo.
