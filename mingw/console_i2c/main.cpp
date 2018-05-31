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

#define RGB_APDS9960_SENSOR 1
//#define TEMP_SENSOR         1

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


unsigned char  b[256];

static int i2c_read(unsigned char chip_addr, unsigned short int_addr, unsigned short int_addr_len, unsigned char *buff, unsigned short dlen)
{
    unsigned char buffer[1024];

    // issue a i2c read command
    i2c_mem_read(chip_addr, int_addr, int_addr_len, buffer, dlen);

    // now wait for WM_I2C_READ message
    if (!WaitWiFiMsg(WM_I2C_READ, 500))
    {
        printf("Wait Time Failed\r\n");
        return -1;
    }
    i2c_read_buffer(buff, dlen);
    return 1;
}

static int i2c_write(unsigned char chip_addr, unsigned short int_addr, unsigned short int_addr_len, unsigned char *buff, unsigned short dlen)
{
    i2c_mem_write(chip_addr, int_addr, int_addr_len, buff, dlen);
    return 1;
}



int main(void)
{

    int iCntr = 0;
    float temp;
    unsigned short x;

    printf("this is console example how to access I2C device from  console\r\n");
    enter_eth_lib();
    //setit_as_esp8266();

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
    printf("Now Scanning I2C Bus\r\n");
    fun_i2c_scan();

    if (!WaitWiFiMsg(WM_I2C_SCAN_RES, 500))
    {
        printf("Wait Time Failed\r\n");
        return -1;
    }
    else
    {
        I2C_Scan_Result i2c_res;
        printf("I2C Scan result available\r\n");
        printf("I2C Scan Dev Count=%d\r\n", i2c_devices_cnt());
        if (i2c_devices_cnt())
        {
            QString valueAddr2 = QString("0x%1").arg(i2c_devices_addr(1), 0, 16);
            printf("I2C Scan Dev 8 BIT Addr1=0x%02X\r\n", i2c_devices_addr(0), 0, 16);
            printf("I2C Scan Dev 8 BIT Addr1=0x%02X\r\n", i2c_devices_addr(1), 0, 16);
            fflush(stdout);
        }
    }

#ifdef TEMP_SENSOR
    iCntr = 100;
    while (iCntr)
    {
        if (i2c_read(0xB4, 0x07, 1, b, 2))
        {
            x = ((0xff & b[1]) << 8) | (0xff & b[0]);
            temp = x;
            temp *= .02;
            temp  -= 273.15;

            printf(" %.3f\r\n", temp);
        }
        iCntr--;
    }

#endif

#ifdef RGB_APDS9960_SENSOR
    // CHIP ID A, APDS-9960
    if (i2c_read(0x72, 0x92, 1, b, 1))
    {
        printf("CHIP ID=[0x%02X]\r\n", 0xff & b[0]);
        fflush(stdout);
    }


    // Enable

    uint8_t enable = 0b00000001;
    uint8_t val;
    enable = enable << 4;
    val &= 0b11101111;
    val |= enable;
    b[0] = 0x03;
    i2c_write(0x72, 0x80, 1, b, 1);

    if (i2c_read(0x72, 0x80, 1, b, 1))
    {
        printf("Wait Time=[0x%02X]\r\n", 0xff & b[0]);
        fflush(stdout);
    }

    // Enable, Set GAIN
    b[0] = 0x03;
    i2c_write(0x72, 0x8f, 1, b, 1);

    if (i2c_read(0x72, 0x8f, 1, b, 1))
    {
        printf("GAIN Time=[0x%02X]\r\n", 0xff & b[0]);
        fflush(stdout);
    }

    iCntr = 100;
    while (iCntr)
    {
        if (i2c_read(0x72, 0x94, 1, b, 10))
        {
            unsigned short R;
            unsigned short G;
            unsigned short B;

            R = ((0xff & b[1]) << 8) | (0xff & b[0]);
            G = ((0xff & b[3]) << 8) | (0xff & b[2]);
            B = ((0xff & b[5]) << 8) | (0xff & b[4]);

            printf("RED=[0x%04X] [0x%04X] [0x%04X]\r\n", R, G, B);
            fflush(stdout);
        }
        iCntr--;
    }
#endif





    osDelay(100);
    exit_eth_lib();
    osDelay(100);
    return 0;
}
