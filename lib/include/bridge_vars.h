#include "saag.h"

#define MAX_IP_DEVICES 256
#define MAX_DEV_TTIMEOUT 2

#ifndef __CIP_LIST_H
#define __CIP_LIST_H
struct cip_list
{
    unsigned int       Qt_ipaddr;
    SAAGVersion        devVersion;
    unsigned short     devTimeout;
    int                index;
    int                isOpen;
};
#endif

FIX_EXTERN  volatile unsigned short ipDeviceCounter;
FIX_EXTERN  char device_ip_address[256];
FIX_EXTERN  unsigned int isRegisterd;
FIX_EXTERN  unsigned int isGlobalError;
FIX_EXTERN  unsigned char usExitAllThread;
FIX_EXTERN  SAAGVersion  devVersion;

FIX_EXTERN  volatile unsigned int isClientUDP_Running;
FIX_EXTERN  volatile unsigned int isBroadcastUDP_Running;
FIX_EXTERN  volatile unsigned int isServerUDP_Running;

FIX_EXTERN  volatile unsigned int isHeartBeat_Running;
FIX_EXTERN  volatile unsigned int isMainDevIOTask_Running;

FIX_EXTERN double x_Axis[11200];
FIX_EXTERN double xCh1_Array[11200];
FIX_EXTERN double xCh2_Array[11200];
FIX_EXTERN double signal_FIX_A[11200];
FIX_EXTERN double signal_FIX_B[11200];
FIX_EXTERN double xAxis_Array[4095];
FIX_EXTERN double y1_g1Axis_Array[4095];
FIX_EXTERN double y2_g1Axis_Array[4095];
FIX_EXTERN double y1_g2Axis_Array[4095];
FIX_EXTERN double y2_g2Axis_Array[4095];

FIX_EXTERN double adcCh1[256];
FIX_EXTERN double adcCh2[256];

FIX_EXTERN PlSignalDataPoint _shared_signal;
FIX_EXTERN PlSignalDataPoint _shared_scope;
FIX_EXTERN ScopeDataPoint   _pullSco;

FIX_EXTERN unsigned long wiegFacCode;
FIX_EXTERN unsigned long wiegCardNum;
FIX_EXTERN unsigned long wiegTimeout;
