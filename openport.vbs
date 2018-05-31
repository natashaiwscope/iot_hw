'******************************************************************************
'OpenPorts-com.vbs
'Author: Peter Costantini, The Microsoft Scripting Guys
'Date: 8/27/04
'Version: 1.0
'This script opens specified ports in Windows Firewall.
'******************************************************************************

'Set constants.
Const NET_FW_IP_PROTOCOL_TCP = 6
Const NET_FW_IP_PROTOCOL_UDP = 17
Const NET_FW_SCOPE_ALL = 0
Const NET_FW_SCOPE_LOCAL_SUBNET = 1
'First dimension of arrNewPorts must equal # of ports to be added minus 1.
Dim arrNewPorts(2,4)

'Edit this list to add ports to the exceptions list.
'Scope and Enabled are optional properties.
'Default for Scope is NET_FW_SCOPE_ALL. Default for Enabled is True.

arrNewPorts(0,0) = "FPS" 'Name
arrNewPorts(0,1) = 5683 'Port
arrNewPorts(0,2) = NET_FW_IP_PROTOCOL_TCP 'Protocol
arrNewPorts(0,3) = NET_FW_SCOPE_ALL 'Scope - default is NET_FW_SCOPE_ALL
arrNewPorts(0,4) = True 'Enabled - default is True

arrNewPorts(1,0) = "FPS1"
arrNewPorts(1,1) = 5555
arrNewPorts(1,2) = NET_FW_IP_PROTOCOL_TCP
arrNewPorts(1,3) = NET_FW_SCOPE_LOCAL_SUBNET
arrNewPorts(1,4) = True

On Error Resume Next
'Create the firewall manager object.
Set objFwMgr = CreateObject("HNetCfg.FwMgr")
If Err <> 0 Then
  WScript.Echo "Unable to connect to Windows Firewall."
  WScript.Quit
End If
'Get the current profile for the local firewall policy.
Set objProfile = objFwMgr.LocalPolicy.CurrentProfile
Set colOpenPorts = objProfile.GloballyOpenPorts

WScript.Echo VbCrLf & "New open ports added:"
For i = 0 To UBound(arrNewPorts)
'Create an FWOpenPort object
  Set objOpenPort = CreateObject("HNetCfg.FWOpenPort")
  objOpenPort.Name = arrNewPorts(i, 0)
  objOpenPort.Port = arrNewPorts(i, 1)
  objOpenPort.Protocol = arrNewPorts(i, 2)
  objOpenPort.Scope = arrNewPorts(i, 3)
  objOpenPort.Enabled = arrNewPorts(i, 4)
  colOpenPorts.Add objOpenPort
  If Err = 0 Then
    WScript.Echo VbCrLf & "Name: " & objOpenPort.Name
    WScript.Echo "  Protocol: " & objOpenPort.Protocol
    WScript.Echo "  Port Number: " & objOpenPort.Port
    WScript.Echo "  Scope: " & objOpenPort.Scope
    WScript.Echo "  Enabled: " & objOpenPort.Enabled
  Else
    WScript.Echo VbCrLf & "Unable to add port: " & arrNewPorts(i, 0)
    WScript.Echo "  Error Number:" & Err.Number
    WScript.Echo "  Source:" & Err.Source
    WScript.Echo "  Description:" & Err.Description
  End If
  Err.Clear
Next

Set colOpenPorts = objProfile.GloballyOpenPorts
WScript.Echo VbCrLf & "All listed ports after operation:"
For Each objPort In colOpenPorts
  WScript.Echo VbCrLf & "Name: " & objPort.Name
  WScript.Echo "  Protocol: " & objPort.Protocol
  WScript.Echo "  Port Number: " & objPort.Port
  WScript.Echo "  Scope: " & objPort.Scope
  WScript.Echo "  Enabled: " & objPort.Enabled
Next

