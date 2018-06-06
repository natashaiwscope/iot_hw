netsh advfirewall firewall add rule name="BroadcastListen" protocol=UDP dir=in localport=5555 action=allow
netsh advfirewall firewall add rule name="IncommingMsg" protocol=UDP dir=in localport=5683 action=allow
