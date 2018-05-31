// APDS-9960  RGB Sensor module Read
#include <QCoreApplication>
#include <QStringList>
#include <iostream>
#include <stdio.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <QMutex>
#include <QThread>
#ifdef WINDOWS_WAY
#include <windows.h>
#endif
#include <stdio.h>

#include "ext_udp.h"

using namespace std;

#define WAIT_TIME_OUT 100

void DELAY(unsigned short i)
{
    // 10 millisecond delay
#if defined(__WIN32__) || defined(_WIN32) || defined(WIN32) || defined(__WINDOWS__) || defined(__TOS_WIN__)
    Sleep(i);
#else  /* presume POSIX */
    usleep(i * 1000);
#endif
}

static unsigned short WaitWiFiMsg(unsigned short iMsg, unsigned short waitTime)
{
    unsigned short iReady = 0;
    unsigned short winMsg, wPar, lPar;
    float          fVal;
    char           strMsg[1023];

    while (waitTime)
    {
        iReady = usPullStrWinQ(winMsg, wPar, lPar, fVal, strMsg);
        if (iReady)
        {
            if (iMsg == winMsg)
            {
                return 1;
            }
        }

        if (!waitTime)
        {
            return 0;
        }

        DELAY(100);

        waitTime--;
    }
    return 0;
}

int main(void)
{

    int iCntr = 0;

    printf("this is console example how to access I2C device from  console\r\n");
    enter_eth_lib();
    setit_as_esp8266();

#ifdef LINUX_WAY
    RegisterWin(0);
#else
    RegisterWin(NULL);
#endif

    if (!WaitWiFiMsg(WM_CONNECTED, 5000))
    {
        printf("Wait Time Failed\r\n");
        return -1;
    }
    else
    {
        // Following function is must call for wireless module
        // for every startup
        fn_get_ver();
        DELAY(10);
    }

    printf("------------------\r\n");
    printf("WiFi Device Connected\r\n");
    printf("Now Reading GPIO PINS\r\n");
    iCntr = 100;

    while (iCntr)
    {
        usADIOPort(0, 0, 0, 0);
        if (!WaitWiFiMsg(WM_GEN_ADIO, 500))
        {
            printf("Wait Time Failed\r\n");
            return -1;
        }
        else
        {
            printf("DIGITAL INPUT=%d %d %d %d\r\n", get_gen_val(0), get_gen_val(1), get_gen_val(2), get_gen_val(3));
        }
        iCntr--;
    }


    osDelay(100);
    exit_eth_lib();
    osDelay(100);
    return 0;
}
